#ifndef SFE_EXAMPLE_SNAKE_HXX
#define SFE_EXAMPLE_SNAKE_HXX

#include <deque>

#include <SFE/game.hxx>
#include <SFE/ndarray.hxx>

// Game constants.
static float const game_field_width = 2.2f;
static float const game_field_height = 1.55f;
static float const field_width = 0.05f;
static float const field_height = 0.05f;
static unsigned int const num_fields_x = std::lroundf(game_field_width / field_width);
static unsigned int const num_fields_y = std::lroundf(game_field_height / field_height);

////////////////////////////////////////////////////////////
/// Convert game field coordinates to view coordinates.
////////////////////////////////////////////////////////////
sf::Vector2f field_to_view(unsigned int x, unsigned int y)
{
    float const fx = static_cast<float>(num_fields_x);
    float const fy = static_cast<float>(num_fields_y);
    float const vx = (x - 0.5f * (fx - 1)) * game_field_width / fx;
    float const vy = (y - 0.5f * (fy - 1)) * game_field_height / fy;
    return{ vx, vy };
}

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

    ////////////////////////////////////////////////////////////
    /// Create the background images and place the snake.
    ////////////////////////////////////////////////////////////
    virtual void init() override;

protected:

    ////////////////////////////////////////////////////////////
    /// Move the snake.
    ////////////////////////////////////////////////////////////
    virtual void update(sf::Time elapsed_time) override;

private:

    ////////////////////////////////////////////////////////////
    /// A SnakePart holds the GameObject and the position of a
    /// snake head or body part in field coordinates.
    ////////////////////////////////////////////////////////////
    struct SnakePart
    {
        unsigned int x;
        unsigned int y;
        sfe::GameObject* obj;
    };

    ////////////////////////////////////////////////////////////
    /// The different states that a game field can take.
    ////////////////////////////////////////////////////////////
    enum class FieldType
    {
        Empty,
        Snake,
        Food
    };

    ////////////////////////////////////////////////////////////
    /// Create the background images.
    ////////////////////////////////////////////////////////////
    void create_background();

    ////////////////////////////////////////////////////////////
    /// Place the first snake.
    ////////////////////////////////////////////////////////////
    void place_snake();

    ////////////////////////////////////////////////////////////
    /// Create a snake head part at the given field coordinates.
    ////////////////////////////////////////////////////////////
    void create_head_part(unsigned int x, unsigned int y);

    ////////////////////////////////////////////////////////////
    /// Create a snake body part at the given field coordinates.
    ////////////////////////////////////////////////////////////
    void create_body_part(unsigned int x, unsigned int y);

    ////////////////////////////////////////////////////////////
    /// Stores which fields are currently occupied by the snake
    /// body.
    ////////////////////////////////////////////////////////////
    sfe::Array2D<FieldType> field_occupied_;

    ////////////////////////////////////////////////////////////
    /// The snake head.
    ////////////////////////////////////////////////////////////
    SnakePart snake_head_;

    ////////////////////////////////////////////////////////////
    /// The snake body.
    ////////////////////////////////////////////////////////////
    std::deque<SnakePart> snake_body_;

}; // class SnakeGame

template <typename... Args>
SnakeGame::SnakeGame(Args && ... args)
    :
    Game(args...),
    field_occupied_(num_fields_x, num_fields_y, FieldType::Empty)
{}

void SnakeGame::init()
{
    create_background();
    place_snake();
}

void SnakeGame::update(sf::Time elapsed_time)
{
    //// Apply some rotation to make things interesting.
    //total_elapsed_time_ += elapsed_time;
    //auto r = 5 * std::sin(1 * total_elapsed_time_.asSeconds());
    //screen_.get_game_view().setRotation(r);
}

void SnakeGame::create_background()
{
    using namespace sfe;
    
    auto const ratio = screen_.get_game_view().getSize().x / screen_.get_game_view().getSize().y;

    // Create the background.
    auto bg = std::make_unique<ImageObject>("camel_bg.jpg");
    bg->set_z_index(-2);
    bg->set_size(2 * ratio, 2);
    screen_.add_game_object(std::move(bg));

    // Create the frame for the game field.
    auto frame = std::make_unique<ImageObject>("frame.png");
    frame->set_z_index(-1);
    frame->set_size(game_field_width * 1.11286407767f, game_field_height * 1.16006884682f);
    screen_.add_game_object(std::move(frame));
}

void SnakeGame::place_snake()
{
    create_head_part(num_fields_x / 3, num_fields_y / 2);
    for (int i = 0; i < 3; ++i)
        create_body_part(snake_head_.x - 1 - i, snake_head_.y);
}

void SnakeGame::create_head_part(unsigned int x, unsigned int y)
{
    // Create the snake part object and occupy the game field.
    snake_head_.x = x;
    snake_head_.y = y;
    field_occupied_(x, y) = FieldType::Snake;

    // Create the game object and add it to the screen.
    auto const pos = field_to_view(x, y);
    auto head = std::make_unique<sfe::ImageObject>("snake_head.png");
    head->set_size(field_width, field_height);
    head->set_position(pos);
    snake_head_.obj = screen_.add_game_object(std::move(head));
}

void SnakeGame::create_body_part(unsigned int x, unsigned int y)
{
    // Create the snake part object and occupy the game field.
    SnakePart part;
    part.x = x;
    part.y = y;
    field_occupied_(x, y) = FieldType::Snake;

    // Create the game object and add it to the screen.
    auto const pos = field_to_view(x, y);
    auto body = std::make_unique<sfe::ImageObject>("snake_body.png");
    body->set_size(field_width, field_height);
    body->set_position(pos);
    part.obj = screen_.add_game_object(std::move(body));

    // Append the snake part to the queue.
    snake_body_.push_back(part);
}

#endif
