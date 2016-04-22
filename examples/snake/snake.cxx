#include <iostream>

#include <SFML/Graphics.hpp>

#include <SFE/screen.hxx>
#include <SFE/input.hxx>

void create_gui(sfe::Widget & gui)
{
    using namespace sfe;

    //// Create the background.
    //auto bg = std::make_unique<ImageWidget>("camel_bg.jpg");
    //gui.add_widget(std::move(bg));
}

template <class T>
void create_game_objects(T & game_objects)
{
    using namespace sfe;

    auto obj = std::make_unique<GameObject>();
    game_objects.push_back(std::move(obj));
}

int main()
{
    using namespace sfe;

    unsigned int const WIDTH = 800;
    unsigned int const HEIGHT = 500;

    // Create the screen.
    Screen screen;
    create_gui(screen.get_gui());
    create_game_objects(screen.get_game_objects());

    // Create the clock.
    sf::Clock clock;

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

        // Draw the screen.
        window.clear();
        screen.render(window);
        window.display();
    }
}
