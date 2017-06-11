#include <SFE/event_manager.hxx>

#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <vector>

namespace sfe
{

    Event::Event(std::string const& name)
        :
        name_{ name }
    {}

    bool Event::operator<(Event const& other) const
    {
        return name_ < other.name_;
    }

    bool Event::operator==(Event const& other) const
    {
        return name_ == other.name_;
    }

    Listener::Listener(Callback const& f)
        :
        callback_{ f }
    {}

    void Listener::set_callback(Callback const& f)
    {
        callback_ = f;
    }

    void Listener::notify(Event const& event) const
    {
        if (callback_)
            callback_(event);
    }

    class EventManager::impl
    {
    public:

        std::shared_ptr<Listener> register_listener(
            Event const& event,
            Listener::Callback const& callback
        );

        void enqueue(Event const& event);

        void dispatch();

        void register_event(Event const& event);

    private:

        ////////////////////////////////////////////////////////////
        /// The registered listeners.
        ////////////////////////////////////////////////////////////
        std::map<Event, std::vector<std::weak_ptr<Listener>>> listeners_;

        ////////////////////////////////////////////////////////////
        /// The enqueued events.
        ////////////////////////////////////////////////////////////
        std::queue<Event> event_queue_;

        ////////////////////////////////////////////////////////////
        /// The list with the registered events.
        ////////////////////////////////////////////////////////////
        std::set<Event> registered_events_;

    };

    EventManager::EventManager()
        :
        impl_{ std::make_unique<impl>() }
    {}

    EventManager::~EventManager() = default;

    EventManager::EventManager(EventManager && other) = default;

    EventManager& EventManager::operator=(EventManager && other) = default;
    
    std::shared_ptr<Listener> EventManager::register_listener(
        Event const & event,
        Listener::Callback const& callback
    ){
        return impl_->register_listener(event, callback);
    }

    void EventManager::enqueue(Event const& event)
    {
        impl_->enqueue(event);
    }

    void EventManager::dispatch()
    {
        impl_->dispatch();
    }

    void EventManager::register_event(Event const& event)
    {
        impl_->register_event(event);
    }

    std::shared_ptr<Listener> EventManager::impl::register_listener(
        Event const& event,
        Listener::Callback const& callback
    ){
#ifdef CHECKEVENTTYPE
        if (registered_events_.count(event) == 0)
            throw EventException("EventManager::register_listener(): Tried to register a listener for an unregistered event.");
#endif
        auto listener = std::make_shared<Listener>(callback);
        listeners_[event].emplace_back(listener);
        return listener;
    }

    void EventManager::impl::enqueue(Event const& event)
    {
#ifdef CHECKEVENTTYPE
        if (registered_events_.count(event) == 0)
            throw EventException("EventManager::enqueue(): Tried to post an unregistered event.");
#endif
        event_queue_.push(std::move(event));
    }

    void EventManager::impl::dispatch()
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

        // Notify the listeners on the events
        while (!event_queue_.empty())
        {
            auto const& ev = event_queue_.front();

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

    void EventManager::impl::register_event(Event const& event)
    {
        registered_events_.insert(event);
    }

} // namespace sfe
