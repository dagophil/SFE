#include <iostream>

#include "SFE/screen.hxx"
#include "SFE/utility.hxx"

namespace sfe
{

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

        // TODO:
        // Update the logic of the game objects.
    }

    void Screen::render(sf::RenderTarget & target) const
    {
        target.setView({ { 0.5f, 0.5f },{ 1.0f, 1.0f } });
        gui_.render(target);
    }

    Widget & Screen::get_gui()
    {
        return gui_;
    }

    Widget const & Screen::get_gui() const
    {
        return gui_;
    }

    std::vector<std::unique_ptr<GameObject> > & Screen::get_game_objects()
    {
        return game_objects_;
    }

    std::vector<std::unique_ptr<GameObject> > const & Screen::get_game_objects() const
    {
        return game_objects_;
    }

} // namespace sfe
