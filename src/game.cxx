#include "SFE/game.hxx"
#include "SFE/input.hxx"

namespace sfe
{
    Game::Game(unsigned int width, unsigned int height, std::string const & title, sf::Uint32 style)
        :
        screen_(sf::View()),
        window_(sf::VideoMode(width, height), title, style)
    {
        sf::Style::Close;
        float const ratio = width / static_cast<float>(height);
        screen_.set_game_view(sf::View({ -ratio, -1, 2 * ratio, 2 }));
    }

    void Game::run()
    {
        // Run the main loop.
        clock_.restart();
        while (window_.isOpen())
        {
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
            screen_.update(window_, elapsed_time);

            // Call the concrete update method.
            update(elapsed_time);

            // Draw the screen.
            window_.clear();
            screen_.render(window_);
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
}