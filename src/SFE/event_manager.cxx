#include <algorithm>
#include <set>

#include "SFE/event_manager.hxx"

namespace sfe
{

    Listener::Listener(Callback f)
        :
        callback_(f)
    {}

    Listener::~Listener()
    {
        // Remove the listener from all event managers.
        for (auto const & p : registers_)
        {
            p.first->remove(this, p.second);
        }
    }

    void Listener::remove(EventManager* event_manager, Event const & event)
    {
        // Remove the event manager from the internal list.
        registers_.erase(
            std::remove(registers_.begin(), registers_.end(), std::pair<EventManager*, Event>(event_manager, event)),
            registers_.end()
        );
    }

    void Listener::set_callback(Callback f)
    {
        callback_ = f;
    }

    void Listener::notify(Event const & event) const
    {
        callback_(event);
    }

    EventManager & EventManager::global()
    {
        static EventManager instance;
        return instance;
    }

    EventManager::~EventManager()
    {
        // Remove the event manager from all listeners.
        for (auto const & p : listeners_)
        {
            for (auto & listener : p.second)
            {
                listener->remove(this, p.first);
            }
        }
    }

    void EventManager::register_listener(Listener & listener, Event const & event)
    {
#ifdef CHECKEVENTTYPE
        if (registered_events_.count(event) == 0)
            throw std::runtime_error("EventManager::register_listener(): Tried to register a listener for an unregistered event.");
#endif
        listener.registers_.push_back({ this, event });
        listeners_[event].push_back(&listener);
    }

    void EventManager::unregister_listener(Listener & listener, Event const & event)
    {
        listener.remove(this, event);
        remove(&listener, event);
    }

    void EventManager::enqueue(Event const & event)
    {
        event_queue_.push(event);
    }

    void EventManager::enqueue(Event && event)
    {
#ifdef CHECKEVENTTYPE
        if (registered_events_.count(event) == 0)
            throw std::runtime_error("EventManager::enqueue(): Tried to post an unregistered event.");
#endif
        event_queue_.push(std::forward<Event>(event));
    }

    void EventManager::dispatch()
    {
        while (!event_queue_.empty())
        {
            auto const & ev = event_queue_.front();

            // Iterate over a copy , so listeners can be removed from inside the loop.
            auto listeners = listeners_[ev];
            for (auto l : listeners)
            {
                l->notify(ev);
            }
            event_queue_.pop();
        }
    }

    void EventManager::register_event(Event const & event)
    {
        registered_events_.insert(event);
    }

    void EventManager::remove(Listener* listener, Event const & event)
    {
        // Remove the listener from the internal list.
        auto it = listeners_.find(event);
        if (it != listeners_.end())
        {
            it->second.erase(
                std::remove(it->second.begin(), it->second.end(), listener),
                it->second.end()
            );
        }
    }

} // namespace sfe
