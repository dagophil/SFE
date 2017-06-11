#ifndef SFE_EXAMPLE_SNAKE_HXX
#define SFE_EXAMPLE_SNAKE_HXX

#include <SFE/game.hxx>
#include <SFE/ndarray.hxx>
#include <SFE/input.hxx>
#include <SFE/event_manager.hxx>

namespace snake
{
    ////////////////////////////////////////////////////////////
    /// The snake game.
    ////////////////////////////////////////////////////////////
    class SnakeGame : public sfe::Game
    {
    public:

        using Game::Game;

    private:

        ////////////////////////////////////////////////////////////
        /// Load the game screen.
        ////////////////////////////////////////////////////////////
        virtual void init_impl() override;

        ////////////////////////////////////////////////////////////
        /// The update function is empty, because the logic is
        /// implemented with events.
        ////////////////////////////////////////////////////////////
        virtual void update_impl(sf::Time const& elapsed_time) override;

    }; // class SnakeGame

} // namespace snake

#endif
