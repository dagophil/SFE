#ifndef SFE_GAME_HXX
#define SFE_GAME_HXX

#include <SFE/sfestd.hxx>
#include <SFE/propagate_const.hxx>

#include <SFML/Config.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include <memory>

namespace sf
{
    class RenderWindow;
    class Time;
}

namespace sfe
{
    class EventManager;
    class ResourceManager;
    class Screen;

    ////////////////////////////////////////////////////////////
    /// Base class for all game applications.
    ////////////////////////////////////////////////////////////
    class SFE_API Game
    {
    public:

        ////////////////////////////////////////////////////////////
        /// Initialize the game.
        ////////////////////////////////////////////////////////////
        Game(
            unsigned int const width,
            unsigned int const height,
            std::string const& title,
            sf::Uint32 const style = sf::Style::Default
        );

        ////////////////////////////////////////////////////////////
        /// Default destructor.
        ////////////////////////////////////////////////////////////
        ~Game();

        ////////////////////////////////////////////////////////////
        /// Run the game loop.
        ////////////////////////////////////////////////////////////
        void run();

        ////////////////////////////////////////////////////////////
        /// Return the render window.
        ////////////////////////////////////////////////////////////
        sf::RenderWindow & get_window();

        ////////////////////////////////////////////////////////////
        /// Return the render window.
        ////////////////////////////////////////////////////////////
        sf::RenderWindow const & get_window() const;

        ////////////////////////////////////////////////////////////
        /// Load the given screen.
        ////////////////////////////////////////////////////////////
        void load_screen(
            std::unique_ptr<Screen> new_screen,
            bool const change_to_default_view = true
        );

        ////////////////////////////////////////////////////////////
        /// Return the event manager.
        ////////////////////////////////////////////////////////////
        std::shared_ptr<EventManager> get_event_manager() const;

        ////////////////////////////////////////////////////////////
        /// Returns the resource manager.
        ////////////////////////////////////////////////////////////
        std::shared_ptr<ResourceManager> get_resource_manager() const;

    private:

        class impl;
        friend class impl; // necessary to access private virtual methods
        sfe::propagate_const<std::unique_ptr<impl>> impl_;

        ////////////////////////////////////////////////////////////
        /// The concrete initialization of the derived class, which
        /// is called at the beginning of run().
        ////////////////////////////////////////////////////////////
        virtual void init_impl() = 0;

        ////////////////////////////////////////////////////////////
        /// The concrete update of the derived class, which is
        /// called once per frame.
        ////////////////////////////////////////////////////////////
        virtual void update_impl(sf::Time const& elapsed_time) = 0;

    }; // class Game

    ////////////////////////////////////////////////////////////
    /// Exception class for all game exceptions.
    ////////////////////////////////////////////////////////////
    DECLARE_EXCEPTION(GameException);

} // namespace sfe

#endif
