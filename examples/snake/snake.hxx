#ifndef SFE_EXAMPLE_SNAKE_HXX
#define SFE_EXAMPLE_SNAKE_HXX

#include <deque>
#include <random>

#include <SFE/game.hxx>
#include <SFE/ndarray.hxx>
#include <SFE/input.hxx>
#include <SFE/event_manager.hxx>

#include "gamescreen.hxx"

namespace snake
{
    ////////////////////////////////////////////////////////////
    /// The snake game.
    ////////////////////////////////////////////////////////////
    class SnakeGame : public sfe::Game
    {
    public:

        ////////////////////////////////////////////////////////////
        /// Create a snake game.
        ////////////////////////////////////////////////////////////
        template <typename... Args>
        SnakeGame(Args && ... args);

    }; // class SnakeGame

    template <typename... Args>
    SnakeGame::SnakeGame(Args && ... args)
        :
        Game(args...)
    {
        init_ = [this]()
        {
            load_screen(std::make_unique<GameScreen>());
        };
    }

} // namespace snake

#endif
