#include <iostream>

#include "snake.hxx"

int main()
{
    using namespace sfe;

    snake::SnakeGame snake_game(1280, 800, "Snake", sf::Style::Close);
    snake_game.get_window().setKeyRepeatEnabled(false);
    snake_game.get_window().setFramerateLimit(60);
    snake_game.run();
}
