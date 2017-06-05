#include <SFE/event_manager.hxx>

#include <algorithm>

namespace sfe
{

    Listener::Listener(Callback f)
        :
        callback_(std::move(f))
    {}

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
        callback_ = std::move(f);
    }

    void Listener::notify(Event const& event) const
    {
        if (callback_)
            callback_(event);
    }

    EventManager & EventManager::global()
    {
        static EventManager instance;
        return instance;
    }

    std::shared_ptr<Listener> EventManager::register_listener(Event const & event, Listener::Callback callback)
    {
#ifdef CHECKEVENTTYPE
        if (registered_events_.count(event) == 0)
            throw EventException("EventManager::register_listener(): Tried to register a listener for an unregistered event.");
#endif
        auto listener = std::make_shared<Listener>(callback);
        listeners_[event].emplace_back(listener);
        return listener;
    }

    void EventManager::enqueue(Event const & event)
    {
        event_queue_.push(event);
    }

    void EventManager::enqueue(Event && event)
    {
#ifdef CHECKEVENTTYPE
        if (registered_events_.count(event) == 0)
            throw EventException("EventManager::enqueue(): Tried to post an unregistered event.");
#endif
        event_queue_.push(std::forward<Event>(event));
    }

    void EventManager::dispatch()
    {
        // Erase all listeners that were destroyed.
        for (auto & event_listeners : listeners_)
        {
            auto & listener_vector = event_listeners.second;
            auto const it = std::remove_if(
                listener_vector.begin(),
                listener_vector.end(),
                [](std::weak_ptr<Listener> const& l)
            {
                return l.expired();
            });
            listener_vector.erase(it, listener_vector.end());
        }

        while (!event_queue_.empty())
        {
            auto const & ev = event_queue_.front();

            // Iterate over a copy because the callbacks may add new listeners,
            // and we do not want to change a container while iterating.
            auto event_listeners = listeners_[ev];
            for (auto const& l : event_listeners)
            {
                if (auto s = l.lock())
                {
                    s->notify(ev);
                }
            }
            event_queue_.pop();
        }
    }

    void EventManager::register_event(Event const & event)
    {
        registered_events_.insert(event);
    }

} // namespace sfe
