#include <iostream>

#include <SFML/Graphics.hpp>

#include <SFE/widget.hxx>

int main()
{
    using namespace sfe;

    // Create some widgets.
    ColorWidget container(sf::Color(255, 0, 0));
    container.set_width(0.25);
    container.set_height(0.25);
    container.set_x(0.25);
    container.set_y(0.25);

    auto w0 = std::make_unique<ColorWidget>(sf::Color(0, 0, 255));
    w0->set_width(0.5);
    w0->set_height(0.5);
    container.add_widget(std::move(w0));

    auto w1 = std::make_unique<ColorWidget>(sf::Color(0, 255, 0));
    w1->set_width(0.5);
    w1->set_height(0.5);
    w1->set_x(0.5);
    w1->set_y(0.5);
    container.add_widget(std::move(w1));

    // Create a clock to measure the elapsed time per frame.
    sf::Clock clock;

    // Create the window.
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test");
    Widget::viewport_ratio = 1.333333; // 800 / 600
    window.setView({ { 0.5f, 0.5f }, { 1.0f, 1.0f } });

    // Run the main loop.
    while (window.isOpen())
    {
        // Process window events.
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Update the widgets.
        auto elapsed_time = clock.restart();
        container.update(elapsed_time);

        // Draw the widgets.
        window.clear();
        container.render(window);
        window.display();
    }
}
