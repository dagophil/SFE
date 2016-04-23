#ifndef SFE_EXAMPLE_SNAKE_HXX
#define SFE_EXAMPLE_SNAKE_HXX

#include <deque>
#include <random>

#include <SFE/game.hxx>
#include <SFE/ndarray.hxx>
#include <SFE/input.hxx>

// Game constants.
static float const game_field_width = 2.2f;
static float const game_field_height = 1.55f;
static float const field_width = 0.05f;
static float const field_height = 0.05f;
static int const num_fields_x = std::lroundf(game_field_width / field_width);
static int const num_fields_y = std::lroundf(game_field_height / field_height);

////////////////////////////////////////////////////////////
/// Convert game field coordinates to view coordinates.
////////////////////////////////////////////////////////////
sf::Vector2f field_to_view(int x, int y)
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
    /// A FieldObject holds the GameObject and the position of a
    /// snake part or a food object.
    ////////////////////////////////////////////////////////////
    struct FieldObject
    {
        int x;
        int y;
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
    /// The direction.
    ////////////////////////////////////////////////////////////
    enum class Direction
    {
        Right = 0,
        Down,
        Left,
        Up
    };

    ////////////////////////////////////////////////////////////
    /// Update the direction according to the user input.
    ////////////////////////////////////////////////////////////
    void update_direction();

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
    void create_head_part(int x, int y);
    
    ////////////////////////////////////////////////////////////
    /// Create a snake body part at the given field coordinates.
    ////////////////////////////////////////////////////////////
    void create_body_part(int x, int y);

    ////////////////////////////////////////////////////////////
    /// Create a food item that is placed on a random empty
    /// field.
    ////////////////////////////////////////////////////////////
    void create_random_food();

    ////////////////////////////////////////////////////////////
    /// Move the snake.
    ////////////////////////////////////////////////////////////
    void make_step();

    ////////////////////////////////////////////////////////////
    /// Stores which fields are currently occupied by the snake
    /// body.
    ////////////////////////////////////////////////////////////
    sfe::Array2D<FieldType> field_occupied_;

    ////////////////////////////////////////////////////////////
    /// The number of free game fields.
    ////////////////////////////////////////////////////////////
    size_t num_free_fields_;

    ////////////////////////////////////////////////////////////
    /// The snake head.
    ////////////////////////////////////////////////////////////
    FieldObject snake_head_;

    ////////////////////////////////////////////////////////////
    /// The snake body.
    ////////////////////////////////////////////////////////////
    std::deque<FieldObject> snake_body_;

    ////////////////////////////////////////////////////////////
    /// The food.
    ////////////////////////////////////////////////////////////
    FieldObject food_;

    ////////////////////////////////////////////////////////////
    /// The random engine.
    ////////////////////////////////////////////////////////////
    std::mt19937 rand_engine_;

    ////////////////////////////////////////////////////////////
    /// The current direction of the snake.
    ////////////////////////////////////////////////////////////
    Direction current_direction_;

    ////////////////////////////////////////////////////////////
    /// The new direction that considers the user input between
    /// two steps.
    ////////////////////////////////////////////////////////////
    Direction new_direction_;

    ////////////////////////////////////////////////////////////
    /// The step time.
    ////////////////////////////////////////////////////////////
    sf::Time step_time_;

    ////////////////////////////////////////////////////////////
    /// The time until the next snake step.
    ////////////////////////////////////////////////////////////
    sf::Time until_next_step_;

    ////////////////////////////////////////////////////////////
    /// Whether the snake is currently running.
    ////////////////////////////////////////////////////////////
    bool running_;

}; // class SnakeGame

template <typename... Args>
SnakeGame::SnakeGame(Args && ... args)
    :
    Game(args...),
    field_occupied_(num_fields_x, num_fields_y, FieldType::Empty),
    num_free_fields_(num_fields_x*num_fields_y),
    rand_engine_(std::random_device()()),
    current_direction_(Direction::Right),
    new_direction_(Direction::Right),
    step_time_(sf::seconds(0.5f)),
    until_next_step_(step_time_),
    running_(true)
{}

void SnakeGame::init()
{
    create_background();
    place_snake();
    create_random_food();
}

void SnakeGame::update(sf::Time elapsed_time)
{
    if (running_)
    {
        // Update the direction according to the user input.
        update_direction();

        // Make a step.
        until_next_step_ -= elapsed_time;
        if (until_next_step_ < sf::Time::Zero)
        {
            make_step();
            until_next_step_ += step_time_;
        }
    }

    //// Apply some rotation to make things interesting.
    //total_elapsed_time_ += elapsed_time;
    //auto r = 5 * std::sin(1 * total_elapsed_time_.asSeconds());
    //screen_.get_game_view().setRotation(r);
}

void SnakeGame::update_direction()
{   
    // Read the user input and change the direction variable.
    // Make sure that the snake does not go backwards.
    // TODO: Maybe use input.is_down().
    auto const & input = sfe::Input::global();
    if (input.is_pressed(sf::Keyboard::Up) && current_direction_ != Direction::Down)
        new_direction_ = Direction::Up;
    if (input.is_pressed(sf::Keyboard::Right) && current_direction_ != Direction::Left)
        new_direction_ = Direction::Right;
    if (input.is_pressed(sf::Keyboard::Down) && current_direction_ != Direction::Up)
        new_direction_ = Direction::Down;
    if (input.is_pressed(sf::Keyboard::Left) && current_direction_ != Direction::Right)
        new_direction_ = Direction::Left;

    // Change the direction of the snake head image.
    auto head_ptr = dynamic_cast<sfe::ImageObject*>(snake_head_.obj);
    if (new_direction_ == Direction::Left)
    {
        head_ptr->set_rotation(0);
        head_ptr->set_mirror_x(true);
    }
    else
    {
        head_ptr->set_rotation(90.f * static_cast<int>(new_direction_));
        head_ptr->set_mirror_x(false);
    }
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

void SnakeGame::create_head_part(int x, int y)
{
    // Create the snake part object and occupy the game field.
    snake_head_.x = x;
    snake_head_.y = y;
    field_occupied_(x, y) = FieldType::Snake;
    --num_free_fields_;

    // Create the game object and add it to the screen.
    auto const pos = field_to_view(x, y);
    auto head = std::make_unique<sfe::ImageObject>("snake_head.png");
    head->set_size(field_width, field_height);
    head->set_position(pos);
    snake_head_.obj = screen_.add_game_object(std::move(head));
}

void SnakeGame::create_body_part(int x, int y)
{
    // Create the snake part object and occupy the game field.
    FieldObject part;
    part.x = x;
    part.y = y;
    field_occupied_(x, y) = FieldType::Snake;
    --num_free_fields_;

    // Create the game object and add it to the screen.
    auto const pos = field_to_view(x, y);
    auto body = std::make_unique<sfe::ImageObject>("snake_body.png");
    body->set_size(field_width, field_height);
    body->set_position(pos);
    part.obj = screen_.add_game_object(std::move(body));

    // Append the snake part to the queue.
    snake_body_.push_back(part);
}

void SnakeGame::create_random_food()
{
    // Find a random position on the field.
    std::uniform_int_distribution<> rand(0, num_free_fields_-1);
    auto pos = rand(rand_engine_);

    // Loop over all empty fields until the position is reached.
    int i = 0;
    for (size_t y = 0; y < field_occupied_.height(); ++y)
    {
        for (size_t x = 0; x < field_occupied_.width(); ++x)
        {
            if (field_occupied_(x, y) != FieldType::Empty)
                continue;

            if (i == pos)
            {
                food_.x = x;
                food_.y = y;
                auto food_pos = field_to_view(x, y);
                auto food = std::make_unique<sfe::ImageObject>("strawberry.png");
                food->set_size(field_width, field_height);
                food->set_position(food_pos);
                food_.obj = screen_.add_game_object(std::move(food));
                return;
            }
            ++i;
        }
    }
}

void SnakeGame::make_step()
{
    // Find the new head position.
    current_direction_ = new_direction_;
    int new_x = snake_head_.x;
    int new_y = snake_head_.y;
    if (current_direction_ == Direction::Up)
        --new_y;
    else if (current_direction_ == Direction::Down)
        ++new_y;
    else if (current_direction_ == Direction::Left)
        --new_x;
    else
        ++new_x;

    // Check if the new head field is blocked or the snake went out of bounds.
    if (new_x < 0 || new_x >= num_fields_x ||
        new_y < 0 || new_y >= num_fields_y ||
        field_occupied_(new_x, new_y) == FieldType::Snake)
    {
        running_ = false;
        std::cout << "You lose." << std::endl;
    }
    else // The new head field is free.
    {
        // If the new head field does not contain food, the tail moves.
        bool const got_food = field_occupied_(new_x, new_y) == FieldType::Food;
        if (!got_food)
        {
            // Move the old tail to the place were the head was.
            auto back = snake_body_.back();
            snake_body_.pop_back();
            field_occupied_(back.x, back.y) = FieldType::Empty;
            back.x = snake_head_.x;
            back.y = snake_head_.y;
            back.obj->set_position(field_to_view(back.x, back.y));
            snake_body_.push_front(back);
        }

        // Move the head to the new place.
        field_occupied_(new_x, new_y) = FieldType::Snake;
        snake_head_.x = new_x;
        snake_head_.y = new_y;
        snake_head_.obj->set_position(field_to_view(new_x, new_y));
    }
}

#endif
