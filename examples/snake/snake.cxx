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

void snake::SnakeGame::init_impl()
{
    load_screen(std::make_unique<GameScreen>(get_event_manager(), get_resource_manager()));
}

void snake::SnakeGame::update_impl(sf::Time const & elapsed_time)
{}
