#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include <SFE/screen.hxx>
#include <SFE/input.hxx>

#include "snake.hxx"

static unsigned int const WIDTH = 800;
static unsigned int const HEIGHT = 500;

void create_gui(sfe::Widget & gui)
{
    using namespace sfe;

    // TODO: Create the gui.
}

template <class T>
void create_game_objects(T & game_objects)
{
    using namespace sfe;

    auto bg = std::make_unique<ImageObject>("camel_bg.jpg");
    bg->set_z_index(-1);
    bg->set_size(WIDTH / static_cast<float>(HEIGHT), 1);
    game_objects.push_back(std::move(bg));
}

int main()
{
    using namespace sfe;

    // Create the screen.
    Screen screen(sf::View({ 0, 0, WIDTH / static_cast<float>(HEIGHT), 1 }));

    create_gui(screen.get_gui());
    create_game_objects(screen.get_game_objects());

    // Create the clock.
    sf::Clock clock;
    sf::Time total_elapsed_time;

    // Create the window.
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Snake", sf::Style::Close);

    // Run the main loop.
    while (window.isOpen())
    {
        // Process window events.
        sfe::Input::global().reset();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
                sfe::Input::global().press(event.key.code);
            else if (event.type == sf::Event::MouseButtonPressed)
                sfe::Input::global().press(event.mouseButton.button);
        }

        // Update the screen.
        auto elapsed_time = clock.restart();
        screen.update(window, elapsed_time);

        // Apply some rotation to make things interesting.
        total_elapsed_time += elapsed_time;
        auto r = 5 * std::sin(1 * total_elapsed_time.asSeconds());
        screen.get_game_view().setRotation(r);

        // Draw the screen.
        window.clear();
        screen.render(window);
        window.display();
    }
}
