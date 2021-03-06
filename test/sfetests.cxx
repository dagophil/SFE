#include <SFE/input.hxx>
#include <SFE/event_manager.hxx>
#include <SFE/resource_manager.hxx>
#include <SFE/screen.hxx>
#include <SFE/widget.hxx>

#include <SFML/Graphics.hpp>

#include <iostream>

void create_widgets(sfe::ResourceManager & resource_manager, sfe::Widget & container)
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
    w1->add_click_begin_callback([](Widget & w) {
        std::cout << "click begin green" << std::endl;
    });
    w1->add_click_end_callback([](Widget & w) {
        std::cout << "click end green" << std::endl;
    });

    // Add the disk image on top of the green.
    auto disk_texture = resource_manager.get_texture("disk.png");
    auto w2 = std::make_unique<ImageWidget>(disk_texture);
    w2->set_scale(Scale::X);
    w2->set_align_x(AlignX::Center);
    w2->add_click_begin_callback([](Widget & w) {
        std::cout << "click begin disk" << std::endl;
    });
    w2->add_click_end_callback([](Widget & w) {
        std::cout << "click end disk" << std::endl;
    });
    w2->set_absorb_click(true);
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

    auto event_manager = std::make_shared<EventManager>();
    auto resource_manager = std::make_shared<ResourceManager>();

    // Create the screen with some widgets.
    Screen screen(sf::View({ 0, 0, 1, 1 }), event_manager, resource_manager);
    create_widgets(*resource_manager, screen.get_gui());

    // Create a clock to measure the elapsed time per frame.
    sf::Clock clock;

    // Create the window.
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Test");

    // Run the main loop.
    while (window.isOpen())
    {
        // Process window events.
        sfe::Input::global().reset();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                sfe::Input::global().press(event.key.code);
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                sfe::Input::global().press(event.mouseButton.button);
            }
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
