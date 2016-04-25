#ifndef SFE_EXAMPLE_SNAKE_GAMESCREEN_HXX
#define SFE_EXAMPLE_SNAKE_GAMESCREEN_HXX

#include <SFE/screen.hxx>

namespace snake
{
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
    /// The game screen.
    ////////////////////////////////////////////////////////////
    class GameScreen : public sfe::Screen
    {
    public:

        ////////////////////////////////////////////////////////////
        /// The default constructor.
        ////////////////////////////////////////////////////////////
        GameScreen();

    private:

        ////////////////////////////////////////////////////////////
        /// A FieldObject holds the GameObject and the position of a
        /// snake part or a food object.
        ////////////////////////////////////////////////////////////
        struct FieldObject
        {
            FieldObject()
                :
                x(0),
                y(0),
                obj(nullptr)
            {}

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
        /// The special effects.
        ////////////////////////////////////////////////////////////
        enum class Effect
        {
            None = 0,
            Crooked,
            Wave,

            EffectCount // this must always be the last element in the enum
        };

        ////////////////////////////////////////////////////////////
        /// Create the background images and place the snake.
        ////////////////////////////////////////////////////////////
        void init_impl();

        ////////////////////////////////////////////////////////////
        /// Initialize the event listeners.
        ////////////////////////////////////////////////////////////
        void init_listeners();

        ////////////////////////////////////////////////////////////
        /// Create the gui widgets.
        ////////////////////////////////////////////////////////////
        void create_gui();

        ////////////////////////////////////////////////////////////
        /// Move the snake.
        ////////////////////////////////////////////////////////////
        void update_impl(sf::Time elapsed_time);

        ////////////////////////////////////////////////////////////
        /// Update the direction according to the user input.
        ////////////////////////////////////////////////////////////
        void update_direction();

        ////////////////////////////////////////////////////////////
        /// Update the special effects.
        ////////////////////////////////////////////////////////////
        void update_special_effects(sf::Time elapsed_time);

        ////////////////////////////////////////////////////////////
        /// Update the step time according to the number of
        /// collected food items.
        ////////////////////////////////////////////////////////////
        void update_step_time();

        ////////////////////////////////////////////////////////////
        /// Create a snake head part at the given field coordinates.
        ////////////////////////////////////////////////////////////
        void create_head_part(int x, int y);

        ////////////////////////////////////////////////////////////
        /// Create a snake body part at the given field coordinates.
        ////////////////////////////////////////////////////////////
        void create_body_part(int x, int y, bool back = true);

        ////////////////////////////////////////////////////////////
        /// Return the position of the snake head after user input
        /// was considered.
        ////////////////////////////////////////////////////////////
        sf::Vector2i get_new_head() const;

        ////////////////////////////////////////////////////////////
        /// Move the snake.
        ////////////////////////////////////////////////////////////
        void move_snake(bool got_food, sf::Vector2i const & new_head_pos);

        ////////////////////////////////////////////////////////////
        /// Create a food item that is placed on a random empty
        /// field.
        ////////////////////////////////////////////////////////////
        void spawn_food();

        ////////////////////////////////////////////////////////////
        /// Stop the game loop and show some message.
        ////////////////////////////////////////////////////////////
        void game_over();

        ////////////////////////////////////////////////////////////
        /// Add a random special effect.
        ////////////////////////////////////////////////////////////
        void add_special_effect();

        ////////////////////////////////////////////////////////////
        /// Clear the special effects.
        ////////////////////////////////////////////////////////////
        void clear_special_effects();

        ////////////////////////////////////////////////////////////
        /// Stores which fields are currently occupied by the snake
        /// body.
        ////////////////////////////////////////////////////////////
        sfe::Array2D<FieldType> fields_;

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

        ////////////////////////////////////////////////////////////
        /// Counter for all foods that were collected.
        ////////////////////////////////////////////////////////////
        int food_counter_;

        ////////////////////////////////////////////////////////////
        /// The difficulty.
        ////////////////////////////////////////////////////////////
        bool easymode_;

        ////////////////////////////////////////////////////////////
        /// The current special effect.
        ////////////////////////////////////////////////////////////
        Effect current_effect_;

        ////////////////////////////////////////////////////////////
        /// The runtime of the wave effect.
        ////////////////////////////////////////////////////////////
        sf::Time wave_time_;

    }; // class GameScreen

    inline GameScreen::GameScreen()
        :
        Screen(sf::View()),
        fields_(num_fields_x, num_fields_y, FieldType::Empty),
        rand_engine_(std::random_device()())
    {
        init_ = [this]()
        {
            init_impl();
        };
        update_ = [this](sf::Time elapsed_time)
        {
            update_impl(elapsed_time);
        };
    }

    inline void GameScreen::init_impl()
    {
        using namespace sfe;

        // Clear the containers.
        get_gui().clear_widgets();
        clear_game_objects();
        clear_listeners();

        // Initialize the screen variables with a default game field.
        std::fill(fields_.begin(), fields_.end(), FieldType::Empty);
        num_free_fields_ = num_fields_x*num_fields_y;
        snake_head_ = FieldObject();
        snake_body_.clear();
        food_ = FieldObject();
        current_direction_ = Direction::Right;
        new_direction_ = Direction::Right;
        step_time_ = sf::seconds(0.4f);
        until_next_step_ = step_time_;
        running_ = false;
        food_counter_ = 0;
        easymode_ = true;
        current_effect_ = Effect::None;
        wave_time_ = sf::Time::Zero;

        // Initialize the listeners.
        init_listeners();

        // Create the gui.
        create_gui();

        // Create the background image.
        auto const ratio = get_game_view().getSize().x / get_game_view().getSize().y;
        auto bg = std::make_unique<ImageObject>("camel_bg.jpg");
        bg->set_z_index(-2);
        bg->set_size(2 * ratio, 2);
        add_game_object(std::move(bg));

        // Create the borders of the game field.
        auto field_border = std::make_unique<ImageObject>("frame.png");
        field_border->set_z_index(-1);
        field_border->set_size(game_field_width * 1.11286407767f, game_field_height * 1.16006884682f);
        add_game_object(std::move(field_border));

        // Place the initial snake parts.
        create_head_part(num_fields_x / 3, num_fields_y / 2);
        for (int i = 0; i < 3; ++i)
            create_body_part(snake_head_.x - 1 - i, snake_head_.y);

        // Spawn the first food item.
        spawn_food();
    }

    inline void GameScreen::init_listeners()
    {
        using namespace sfe;

        // Register the event types.
        EventManager::global().register_event("SelectEasyDifficulty");
        EventManager::global().register_event("SelectHardDifficulty");
        EventManager::global().register_event("StartGame");

        // Select easy difficulty.
        create_and_register_listener(
            [this](Event const & event) {
                easymode_ = true;
            },
            "SelectEasyDifficulty"
        );

        // Select hard difficulty.
        create_and_register_listener(
            [this](Event const & event) {
                easymode_ = false;
            },
            "SelectHardDifficulty"
        );

        // Start the game.
        create_and_register_listener(
            [this](Event const & event) {
                if (!easymode_)
                    step_time_ = sf::seconds(0.1f);
                running_ = true;
            },
            "StartGame"
        );
    }

    inline void GameScreen::create_gui()
    {
        using namespace sfe;

        // Create the container of the difficulty selector.
        auto difficulty_container = std::make_unique<Widget>();
        auto container_ptr = get_gui().add_widget(std::move(difficulty_container));
        auto difficulty_remover = std::make_unique<Listener>(
            [container_ptr](Event const & event) {
            container_ptr->remove_from_parent();
        }
        );
        EventManager::global().register_listener(*difficulty_remover, "StartGame");
        container_ptr->add_listener(std::move(difficulty_remover));

        // Create the box for the currently selected item.
        auto text_frame = std::make_unique<ImageWidget>("text_frame.png");
        auto frame_ptr = container_ptr->add_widget(std::move(text_frame));
        frame_ptr->set_align_x(AlignX::Center);
        frame_ptr->set_align_y(AlignY::Center);
        frame_ptr->set_height(0.2f);
        frame_ptr->set_scale(Scale::X);
        frame_ptr->set_y(-0.1f);
        auto frame_easy_selector = std::make_unique<Listener>(
            [frame_ptr](Event const & event) {
            frame_ptr->set_y(-0.1f);
        }
        );
        EventManager::global().register_listener(*frame_easy_selector, "SelectEasyDifficulty");
        frame_ptr->add_listener(std::move(frame_easy_selector));
        auto frame_hard_selector = std::make_unique<Listener>(
            [frame_ptr](Event const & event) {
            frame_ptr->set_y(0.1f);
        }
        );
        EventManager::global().register_listener(*frame_hard_selector, "SelectHardDifficulty");
        frame_ptr->add_listener(std::move(frame_hard_selector));

        // Create the "easy" text.
        auto easy = std::make_unique<ImageWidget>("easy.png");
        easy->set_align_x(AlignX::Center);
        easy->set_align_y(AlignY::Center);
        easy->set_height(0.2f);
        easy->set_scale(Scale::X);
        easy->set_y(-0.1f);
        easy->add_mouse_enter_callback([frame_ptr](Widget & w) {
            EventManager::global().enqueue("SelectEasyDifficulty");
        });
        easy->add_click_end_callback([this](Widget & w) {
            EventManager::global().enqueue("StartGame");
        });
        container_ptr->add_widget(std::move(easy));

        // Create the "hard" text.
        auto hard = std::make_unique<ImageWidget>("hard.png");
        hard->set_align_x(AlignX::Center);
        hard->set_align_y(AlignY::Center);
        hard->set_height(0.2f);
        hard->set_scale(Scale::X);
        hard->set_y(0.1f);
        hard->add_mouse_enter_callback([frame_ptr](Widget & w) {
            EventManager::global().enqueue("SelectHardDifficulty");
        });
        hard->add_click_end_callback([this](Widget & w) {
            EventManager::global().enqueue("StartGame");
        });
        container_ptr->add_widget(std::move(hard));
    }

    inline void GameScreen::update_impl(sf::Time elapsed_time)
    {
        if (running_)
        {
            // Update the direction according to the user input.
            update_direction();

            // Make a step.
            until_next_step_ -= elapsed_time;
            if (until_next_step_ < sf::Time::Zero)
            {
                // Check if the snake collides with itself or the boundary.
                auto const new_head = get_new_head();
                if (new_head.x < 0 || new_head.x >= num_fields_x ||
                    new_head.y < 0 || new_head.y >= num_fields_y ||
                    fields_(new_head.x, new_head.y) == FieldType::Snake)
                {
                    game_over();
                }
                else
                {
                    // Move the snake and spawn an additional body part if food was collected.
                    bool const got_food = fields_(new_head.x, new_head.y) == FieldType::Food;
                    move_snake(got_food, new_head);

                    // Spawn a new food.
                    if (got_food)
                    {
                        ++food_counter_;
                        spawn_food();
                        step_time_ = 0.9f * (step_time_ - sf::seconds(0.04f)) + sf::seconds(0.04f);
                    }
                }

                // Update the step time.
                until_next_step_ += step_time_;
            }

            // Update the special effects.
            update_special_effects(elapsed_time);
        }
    }

    inline void GameScreen::update_direction()
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

    inline void GameScreen::update_special_effects(sf::Time elapsed_time)
    {
        if (current_effect_ == Effect::Wave)
        {
            wave_time_ += elapsed_time;
            auto r = 5 * std::sin(1 * wave_time_.asSeconds());
            get_game_view().setRotation(r);
        }
    }

    inline void GameScreen::create_head_part(int x, int y)
    {
        // Create the snake part object and occupy the game field.
        snake_head_.x = x;
        snake_head_.y = y;
        fields_(x, y) = FieldType::Snake;
        --num_free_fields_;

        // Create the game object and add it to the screen.
        auto const pos = field_to_view(x, y);
        auto head = std::make_unique<sfe::ImageObject>("snake_head.png");
        head->set_size(field_width, field_height);
        head->set_position(pos);
        snake_head_.obj = add_game_object(std::move(head));
    }

    inline void GameScreen::create_body_part(int x, int y, bool back)
    {
        // Create the snake part object and occupy the game field.
        FieldObject part;
        part.x = x;
        part.y = y;
        fields_(x, y) = FieldType::Snake;
        --num_free_fields_;

        // Create the game object and add it to the screen.
        auto const pos = field_to_view(x, y);
        auto body = std::make_unique<sfe::ImageObject>("snake_body.png");
        body->set_size(field_width, field_height);
        body->set_position(pos);
        part.obj = add_game_object(std::move(body));

        // Append the snake part to the queue.
        if (back)
            snake_body_.push_back(part);
        else
            snake_body_.push_front(part);
    }

    inline sf::Vector2i GameScreen::get_new_head() const
    {
        int x = snake_head_.x;
        int y = snake_head_.y;
        if (new_direction_ == Direction::Up)
            --y;
        else if (new_direction_ == Direction::Down)
            ++y;
        else if (new_direction_ == Direction::Left)
            --x;
        else
            ++x;
        return{ x, y };
    }

    inline void GameScreen::move_snake(bool got_food, sf::Vector2i const & new_head_pos)
    {
        // Update the direction variable.
        current_direction_ = new_direction_;

        // If food was collected, just spawn a new body part were the head was.
        // Otherwise, move a body part from the back of the snake to the front.
        if (got_food)
        {
            create_body_part(snake_head_.x, snake_head_.y, false);
        }
        else
        {
            auto back = snake_body_.back();
            snake_body_.pop_back();
            fields_(back.x, back.y) = FieldType::Empty;
            back.x = snake_head_.x;
            back.y = snake_head_.y;
            back.obj->set_position(field_to_view(back.x, back.y));
            snake_body_.push_front(back);
        }

        // Move the snake head.
        fields_(new_head_pos.x, new_head_pos.y) = FieldType::Snake;
        snake_head_.x = new_head_pos.x;
        snake_head_.y = new_head_pos.y;
        snake_head_.obj->set_position(field_to_view(new_head_pos.x, new_head_pos.y));
    }

    inline void GameScreen::spawn_food()
    {
        // In hard mode: Add or remove a special effect.
        if (!easymode_)
        {
            if ((food_counter_ + 4) % 8 == 0)
                add_special_effect();
            if (food_counter_ > 0 && (food_counter_ + 8) % 8 == 0)
                clear_special_effects();
        }

        // Find a random position on the field.
        std::uniform_int_distribution<> rand(0, num_free_fields_ - 1);
        auto pos = rand(rand_engine_);

        // Loop over all empty fields until the position is reached.
        int i = 0;
        for (size_t y = 0; y < fields_.height(); ++y)
        {
            for (size_t x = 0; x < fields_.width(); ++x)
            {
                if (fields_(x, y) != FieldType::Empty)
                    continue;

                if (i == pos)
                {
                    food_.x = x;
                    food_.y = y;
                    fields_(x, y) = FieldType::Food;
                    auto food_pos = field_to_view(x, y);

                    if (food_.obj != nullptr)
                    {
                        // There already is a food object, so just move it.
                        food_.obj->set_position(food_pos);
                    }
                    else
                    {
                        // Create a new food object.
                        auto food = std::make_unique<sfe::ImageObject>("strawberry.png");
                        food->set_size(field_width, field_height);
                        food->set_position(food_pos);
                        food_.obj = add_game_object(std::move(food));
                    }

                    return;
                }
                ++i;
            }
        }
    }

    inline void GameScreen::game_over()
    {
        std::cout << "Game over." << std::endl;
        std::cout << "You collected " << food_counter_ << " food." << std::endl;

        clear_special_effects();
        clear_game_objects();
        init_impl();
    }

    inline void GameScreen::add_special_effect()
    {
        // Create a random effect.
        auto num_effects = static_cast<int>(Effect::EffectCount);
        std::uniform_int_distribution<int> rand(1, num_effects - 1);
        auto eff = rand(rand_engine_);
        current_effect_ = static_cast<Effect>(eff);

        // Apply the effect.
        if (current_effect_ == Effect::Crooked)
        {
            get_game_view().setRotation(14);
        }
        else if (current_effect_ == Effect::Wave)
        {
            wave_time_ = sf::Time::Zero;
        }
    }

    inline void GameScreen::clear_special_effects()
    {
        get_game_view().setRotation(0);
        current_effect_ = Effect::None;
    }

} // namespace snake

#endif
