#include <SFE/screen.hxx>
#include <SFE/resource_manager.hxx>
#include <SFE/utility.hxx>

#include <algorithm>

namespace sfe
{
    Screen::Screen(sf::View game_view, std::shared_ptr<ResourceManager> resource_manager)
        :
        game_view_(game_view),
        resource_manager_(resource_manager)
    {
        gui_.set_resource_manager(resource_manager_);
    }

    void Screen::update(sf::RenderWindow const & window, sf::Time elapsed_time)
    {
        // Update the viewport ratio for the widgets.
        Widget::viewport_ratio = window.getSize().x / static_cast<float>(window.getSize().y);

        // Process the user input.
        if (window.hasFocus())
        {
            // Get the mouse position on the window.
            auto mouse_pos = sf::Mouse::getPosition(window);

            // Update mouseover states of the gui widgets.
            auto handled = gui_.update_mouse(mouse_pos.x / static_cast<float>(window.getSize().x),
                                  mouse_pos.y / static_cast<float>(window.getSize().y));

            // TODO:
            // Update the hover states of the game objects.
            // If no gui widget was clicked, forward the click to the game objects.
        }

        // Update the logic of the gui widgets.
        gui_.update(elapsed_time);
        
        // Update the logic of the game objects.
        for (auto & obj : game_objects_)
            obj->update(elapsed_time);

        // Call the custom update method.
        if (update_)
            update_(elapsed_time);

        // Sort the game objects by their z-index so they are drawn in the right order.
        std::sort(game_objects_.begin(), game_objects_.end(),
            [](auto && a, auto && b) {
                return a->get_z_index() < b->get_z_index();
            }
        );
    }

    void Screen::render(sf::RenderTarget & target) const
    {
        target.setView(game_view_);
        for (auto const & obj : game_objects_)
            obj->render(target);
        target.setView({ { 0.5f, 0.5f },{ 1.0f, 1.0f } });
        gui_.render(target, { 0.0f, 0.0f, 1.0f, 1.0f });
    }

    Widget & Screen::get_gui()
    {
        return gui_;
    }

    Widget const & Screen::get_gui() const
    {
        return gui_;
    }

    GameObject* Screen::add_game_object(std::unique_ptr<GameObject> obj)
    {
        auto ptr = obj.get();
        ptr->set_resource_manager(resource_manager_);
        game_objects_.push_back(std::move(obj));
        return ptr;
    }

    std::unique_ptr<GameObject> Screen::remove_game_object(GameObject* obj)
    {
        auto comp = [obj](auto && objptr)
        {
            return objptr.get() == obj;
        };
        auto it = std::find_if(game_objects_.begin(), game_objects_.end(), comp);
        if (it != game_objects_.end())
        {
            auto objptr = std::move(*it);
            game_objects_.erase(it);
            return objptr;
        }
        else
        {
            return std::unique_ptr<GameObject>();
        }
    }

    void Screen::clear_game_objects()
    {
        game_objects_.clear();
    }

    sf::View & Screen::get_game_view()
    {
        return game_view_;
    }

    sf::View const & Screen::get_game_view() const
    {
        return game_view_;
    }

    void Screen::set_game_view(sf::View const & view)
    {
        game_view_ = view;
    }

    void Screen::add_listener(std::unique_ptr<Listener> listener)
    {
        listeners_.push_back(std::move(listener));
    }

    void Screen::clear_listeners()
    {
        listeners_.clear();
    }

} // namespace sfe
