#include <SFE/game.hxx>
#include <SFE/event_manager.hxx>
#include <SFE/input.hxx>
#include <SFE/resource_manager.hxx>

namespace sfe
{
    Game::Game(unsigned int width, unsigned int height, std::string const & title, sf::Uint32 style)
        :
        window_(sf::VideoMode(width, height), title, style),
        event_manager_(std::make_shared<EventManager>()),
        resource_manager_(std::make_shared<ResourceManager>())
    {}

    void Game::run()
    {
        // Initialize the components.
        init_impl();

        // Load the first screen.
        if (requested_screen_)
            screen_ = std::move(requested_screen_);
        if (!screen_)
            throw GameException("Game::run(): You must load a screen before running the game.");
        if (screen_->init_)
            screen_->init_();

        // Run the main loop.
        clock_.restart();
        while (window_.isOpen())
        {
            // Load the next screen.
            if (requested_screen_)
                screen_ = std::move(requested_screen_);

            // Process window events.
            sfe::Input::global().reset();
            sf::Event event;
            while (window_.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window_.close();
                else if (event.type == sf::Event::KeyPressed)
                    sfe::Input::global().press(event.key.code);
                else if (event.type == sf::Event::MouseButtonPressed)
                    sfe::Input::global().press(event.mouseButton.button);
            }
            if (!window_.isOpen())
                break;

            // Update the screen.
            auto elapsed_time = clock_.restart();
            screen_->update(window_, elapsed_time);

            // Call the concrete update method.
            update_impl(elapsed_time);

            // Handle all events.
            event_manager_->dispatch();

            // Draw the screen.
            window_.clear();
            screen_->render(window_);
            window_.display();
        }
    }

    sf::RenderWindow & Game::get_window()
    {
        return window_;
    }

    sf::RenderWindow const & Game::get_window() const
    {
        return window_;
    }

    void Game::load_screen(std::unique_ptr<Screen> new_screen, bool change_to_default_view)
    {
        requested_screen_ = std::move(new_screen);
        if (change_to_default_view)
        {
            float const ratio = window_.getSize().x / static_cast<float>(window_.getSize().y);
            requested_screen_->set_game_view(sf::View({ -ratio, -1, 2 * ratio, 2 }));
        }
    }

    std::shared_ptr<EventManager> Game::get_event_manager() const
    {
        return event_manager_;
    }

    std::shared_ptr<ResourceManager> Game::get_resource_manager() const
    {
        return resource_manager_;
    }

} // namespace sfe
