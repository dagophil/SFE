#include <iostream>

#include <SFML/Graphics.hpp>

#include <SFE/screen.hxx>
#include <SFE/widget.hxx>

void create_widgets(sfe::Widget & container)
{
    using namespace sfe;

    // Create the red background.
    auto background = std::make_unique<ColorWidget>(sf::Color(255, 0, 0));
    background->set_width(0.25);
    background->set_height(0.25);
    background->set_x(0.25);
    background->set_y(0.25);

    // Add blue in the upper left corner.
    auto w0 = std::make_unique<ColorWidget>(sf::Color(0, 0, 255));
    w0->set_width(0.5);
    w0->set_height(0.5);

    // Add green in the lower right corner.
    auto w1 = std::make_unique<ColorWidget>(sf::Color(0, 255, 0));
    w1->set_width(0.5);
    w1->set_height(0.5);
    w1->set_x(0.5);
    w1->set_y(0.5);

    // Add the disk image on top of the green.
    auto w2 = std::make_unique<ImageWidget>("disk.png");
    w2->set_scale(Scale::X);
    w2->set_align_x(AlignX::Center);
    w1->add_widget(std::move(w2));

    // Nest the widgets.
    background->add_widget(std::move(w0));
    background->add_widget(std::move(w1));
    container.add_widget(std::move(background));
}

int main()
{
    using namespace sfe;

    unsigned int const WIDTH = 800;
    unsigned int const HEIGHT = 600;

    // Create the screen with some widgets.
    Screen screen(WIDTH, HEIGHT);
    create_widgets(screen.get_gui());

    // Create a clock to measure the elapsed time per frame.
    sf::Clock clock;

    // Create the window.
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Test");

    // Run the main loop.
    while (window.isOpen())
    {
        // Process window events.
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized)
                screen.resize(event.size.width, event.size.height);
        }

        // Update the widgets.
        auto elapsed_time = clock.restart();
        screen.update(elapsed_time);

        // Draw the widgets.
        window.clear();
        screen.render(window);
        window.display();
    }
}
