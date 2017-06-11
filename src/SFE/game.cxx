#include <SFE/game.hxx>
#include <SFE/event_manager.hxx>
#include <SFE/input.hxx>
#include <SFE/resource_manager.hxx>
#include <SFE/screen.hxx>

namespace sfe
{
    class Game::impl
    {
    public:

        impl(
            Game & game,
            unsigned int const width,
            unsigned int const height,
            std::string const& title,
            sf::Uint32 const style = sf::Style::Default
        );

        void run();

        sf::RenderWindow & get_window();

        sf::RenderWindow const& get_window() const;

        void load_screen(
            std::unique_ptr<Screen> new_screen,
            bool const change_to_default_view
        );

        std::shared_ptr<EventManager> get_event_manager() const;

        std::shared_ptr<ResourceManager> get_resource_manager() const;

    private:

        ////////////////////////////////////////////////////////////
        /// Reference to the actual game.
        ////////////////////////////////////////////////////////////
        Game & game_;

        ////////////////////////////////////////////////////////////
        /// The current screen.
        ////////////////////////////////////////////////////////////
        std::unique_ptr<Screen> screen_;

        ////////////////////////////////////////////////////////////
        /// The new requested screen. The pointer is only valid
        /// between a call to load_screen() and the next frame.
        ////////////////////////////////////////////////////////////
        std::unique_ptr<Screen> requested_screen_;

        ////////////////////////////////////////////////////////////
        /// The render window.
        ////////////////////////////////////////////////////////////
        sf::RenderWindow window_;

        ////////////////////////////////////////////////////////////
        /// The clock that measures the elapsed time per frame.
        ////////////////////////////////////////////////////////////
        sf::Clock clock_;

        ////////////////////////////////////////////////////////////
        /// The event manager.
        ////////////////////////////////////////////////////////////
        std::shared_ptr<EventManager> event_manager_;

        ////////////////////////////////////////////////////////////
        /// The resource manager.
        ////////////////////////////////////////////////////////////
        std::shared_ptr<ResourceManager> resource_manager_;

    };

    Game::Game(
        unsigned int const width,
        unsigned int const height,
        std::string const & title,
        sf::Uint32 const style
    )   :
        impl_{ std::make_unique<impl>(*this, width, height, title, style) }
    {}

    Game::~Game() = default;

    void Game::run()
    {
        impl_->run();
    }

    sf::RenderWindow & Game::get_window()
    {
        return impl_->get_window();
    }

    sf::RenderWindow const & Game::get_window() const
    {
        return impl_->get_window();
    }

    void Game::load_screen(
        std::unique_ptr<Screen> new_screen,
        bool const change_to_default_view
    ){
        impl_->load_screen(std::move(new_screen), change_to_default_view);
    }

    std::shared_ptr<EventManager> Game::get_event_manager() const
    {
        return impl_->get_event_manager();
    }

    std::shared_ptr<ResourceManager> Game::get_resource_manager() const
    {
        return impl_->get_resource_manager();
    }

    Game::impl::impl(
        Game & game,
        unsigned int const width,
        unsigned int const height,
        std::string const& title,
        sf::Uint32 const style
    )   :
        game_(game),
        window_(sf::VideoMode(width, height), title, style),
        event_manager_(std::make_shared<EventManager>()),
        resource_manager_(std::make_shared<ResourceManager>())
    {}

    void Game::impl::run()
    {
        // Initialize the components.
        game_.init_impl();

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
            game_.update_impl(elapsed_time);

            // Handle all events.
            event_manager_->dispatch();

            // Draw the screen.
            window_.clear();
            screen_->render(window_);
            window_.display();
        }
    }

    sf::RenderWindow & Game::impl::get_window()
    {
        return window_;
    }

    sf::RenderWindow const & Game::impl::get_window() const
    {
        return window_;
    }

    void Game::impl::load_screen(
        std::unique_ptr<Screen> new_screen,
        bool const change_to_default_view
    ){
        requested_screen_ = std::move(new_screen);
        if (change_to_default_view)
        {
            float const ratio = window_.getSize().x / static_cast<float>(window_.getSize().y);
            requested_screen_->set_game_view(sf::View({ -ratio, -1, 2 * ratio, 2 }));
        }
    }

    std::shared_ptr<EventManager> Game::impl::get_event_manager() const
    {
        return event_manager_;
    }

    std::shared_ptr<ResourceManager> Game::impl::get_resource_manager() const
    {
        return resource_manager_;
    }

} // namespace sfe
