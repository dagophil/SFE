#include <iostream>

#include "snake.hxx"

int main()
{
    using namespace sfe;

    SnakeGame snake_game(1280, 800, "Snake", sf::Style::Close);
    snake_game.init();
    snake_game.get_window().setKeyRepeatEnabled(false);
    snake_game.run();
}
