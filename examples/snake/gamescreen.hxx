#ifndef SFE_EXAMPLE_SNAKE_GAMESCREEN_HXX
#define SFE_EXAMPLE_SNAKE_GAMESCREEN_HXX

#include <SFE/game_object.hxx>
#include <SFE/resource_manager.hxx>
#include <SFE/screen.hxx>

#include <deque>
#include <iostream>
#include <memory>
#include <random>

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
        GameScreen(
            std::shared_ptr<sfe::EventManager> const& event_manager,
            std::shared_ptr<sfe::ResourceManager> const& resource_manager
        );

    private:

        ////////////////////////////////////////////////////////////
        /// A FieldObject holds the GameObject and the position of a
        /// snake part.
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
            Food,
            Coin
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
            FlashLight,
            FlashDark,
            Coins,

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
        sfe::GameObject* food_;

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
        /// The runtime of the current effect.
        ////////////////////////////////////////////////////////////
        sf::Time effect_time_;

        ////////////////////////////////////////////////////////////
        /// A counter for managing when events are raised.
        ////////////////////////////////////////////////////////////
        int event_counter_;

        ////////////////////////////////////////////////////////////
        /// The flash widget.
        ////////////////////////////////////////////////////////////
        sfe::Widget* flash_;

        ////////////////////////////////////////////////////////////
        /// The coins. Key is the (x, y) coordinate of the coin
        /// position in the field.
        ////////////////////////////////////////////////////////////
        std::map<std::pair<int, int>, sfe::GameObject*> coins_;

    }; // class GameScreen

    inline GameScreen::GameScreen(
        std::shared_ptr<sfe::EventManager> const& event_manager,
        std::shared_ptr<sfe::ResourceManager> const& resource_manager
    )   :
        Screen(sf::View(), event_manager, resource_manager),
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
        clear_special_effects();

        // Initialize the screen variables with a default game field.
        std::fill(fields_.begin(), fields_.end(), FieldType::Empty);
        snake_head_ = FieldObject();
        snake_body_.clear();
        current_direction_ = Direction::Right;
        new_direction_ = Direction::Right;
        step_time_ = sf::seconds(0.4f);
        until_next_step_ = step_time_;
        running_ = false;
        food_counter_ = 0;
        easymode_ = true;
        current_effect_ = Effect::None;
        effect_time_ = sf::Time::Zero;
        event_counter_ = 0;
        coins_.clear();

        // Initialize the listeners.
        init_listeners();

        // Create the gui.
        create_gui();

        // Create the background image.
        auto const ratio = get_game_view().getSize().x / get_game_view().getSize().y;
        auto camel_texture = get_resource_manager()->get_texture("img/camel_bg.jpg");
        auto bg = std::make_unique<ImageObject>(camel_texture);
        bg->set_z_index(-2);
        bg->set_size(2 * ratio, 2);
        add_game_object(std::move(bg));

        // Create the borders of the game field.
        auto frame_texture = get_resource_manager()->get_texture("img/frame.png");
        auto field_border = std::make_unique<ImageObject>(frame_texture);
        field_border->set_z_index(-1);
        field_border->set_size(game_field_width * 1.11286407767f, game_field_height * 1.16006884682f);
        add_game_object(std::move(field_border));

        // Place the initial snake parts.
        create_head_part(num_fields_x / 3, num_fields_y / 2);
        for (int i = 0; i < 3; ++i)
            create_body_part(snake_head_.x - 1 - i, snake_head_.y);

        // Spawn the first food item.
        auto strawberry_texture = get_resource_manager()->get_texture("img/strawberry.png");
        auto food = std::make_unique<ImageObject>(strawberry_texture);
        food->set_size(field_width, field_height);
        food_ = add_game_object(std::move(food));
        spawn_food();
    }

    inline void GameScreen::init_listeners()
    {
        using namespace sfe;

        // Register the event types.
        auto event_manager_shared_ptr = get_event_manager();
        if (!event_manager_shared_ptr)
        {
            throw GameException("The GameScreen needs an event manager.");
        }
        auto & event_manager = *event_manager_shared_ptr;
        event_manager.register_event(Event("SelectEasyDifficulty"));
        event_manager.register_event(Event("SelectHardDifficulty"));
        event_manager.register_event(Event("StartGame"));
        event_manager.register_event(Event("MovedSnake"));
        event_manager.register_event(Event("CollectedFood"));
        event_manager.register_event(Event("CollectedCoin"));
        event_manager.register_event(Event("AddSpecialEffect"));
        event_manager.register_event(Event("ClearSpecialEffects"));
        event_manager.register_event(Event("GameOver"));
        event_manager.register_event(Event("SoundOn"));
        event_manager.register_event(Event("SoundOff"));

        auto create_and_register_listener = [&](Event const& event, Listener::Callback f)
        {
            auto listener = event_manager.register_listener(event, f);
            add_listener(std::move(listener));
        };

        // Select easy difficulty.
        create_and_register_listener(
            Event("SelectEasyDifficulty"),
            [this](Event const & event) {
                easymode_ = true;
            }
        );

        // Select hard difficulty.
        create_and_register_listener(
            Event("SelectHardDifficulty"),
            [this](Event const & event) {
                easymode_ = false;
            }
        );

        // Start the game.
        create_and_register_listener(
            Event("StartGame"),
            [this](Event const & event) {
                if (!easymode_)
                    step_time_ = sf::seconds(0.1f);
                running_ = true;
            }
        );

        // Collected food.
        create_and_register_listener(
            Event("CollectedFood"),
            [this, &event_manager](Event const & event) {
                // TODO: Add some points.
                ++food_counter_;
                ++event_counter_;
                spawn_food();
                step_time_ = 0.9f * (step_time_ - sf::seconds(0.04f)) + sf::seconds(0.04f);

                // In hard mode: Add or remove a special effect.
                if (!easymode_)
                {
                    if ((event_counter_ + 4) % 8 == 0)
                        event_manager.enqueue(Event("AddSpecialEffect"));
                    if (event_counter_ > 0 && (event_counter_ + 8) % 8 == 0)
                        event_manager.enqueue(Event("ClearSpecialEffects"));
                }
            }
        );
        
        // Collected coin.
        create_and_register_listener(
            Event("CollectedCoin"),
            [this](Event const & event) {
                // TODO: Add some points.
                // Remove the current selected coin.
                auto it = coins_.find({ snake_head_.x, snake_head_.y });
                if (it != coins_.end())
                {
                    remove_game_object(it->second);
                    coins_.erase(it);
                }

                // Spawn a new food item after all coins have been collected.
                if (coins_.size() == 0)
                {
                    event_counter_ += 4;
                    spawn_food();
                }
            }
        );

        // Add special effect.
        create_and_register_listener(
            Event("AddSpecialEffect"),
            [this](Event const & event) {
                add_special_effect();
            }
        );

        // Clear special effects.
        create_and_register_listener(
            Event("ClearSpecialEffects"),
            [this](Event const & event) {
                clear_special_effects();
            }
        );

        // Game over.
        create_and_register_listener(
            Event("GameOver"),
            [this](Event const & event) {
                std::cout << "Game over." << std::endl;
                std::cout << "You collected " << food_counter_ << " food." << std::endl;
                init_impl();
            }
        );

        create_and_register_listener(
            Event("SoundOn"),
            [](Event const & event) {
                std::cout << "Sound ON!" << std::endl;
            }
        );
        create_and_register_listener(
            Event("SoundOff"),
            [](Event const & event) {
                std::cout << "Sound OFF!" << std::endl;
            }
        );
    }

    inline void GameScreen::create_gui()
    {
        using namespace sfe;

        auto event_manager_shared_ptr = get_event_manager();
        if (!event_manager_shared_ptr)
        {
            throw GameException("The GameScreen needs an event manager.");
        }
        auto & event_manager = *event_manager_shared_ptr;

        // Create the container of the difficulty selector.
        auto difficulty_container = std::make_unique<Widget>();
        auto container_ptr = get_gui().add_widget(std::move(difficulty_container));
        container_ptr->set_align_y(AlignY::Center);
        container_ptr->set_height(0.4f);
        auto difficulty_remover = event_manager.register_listener(
            Event("StartGame"),
            [container_ptr](Event const& event)
        {
            container_ptr->remove_from_parent();
        });
        container_ptr->add_listener(std::move(difficulty_remover));

        // Create the box for the currently selected item.
        auto text_frame_texture = get_resource_manager()->get_texture("img/text_frame.png");
        auto text_frame = std::make_unique<ImageWidget>(text_frame_texture);
        auto frame_ptr = container_ptr->add_widget(std::move(text_frame));
        frame_ptr->set_align_x(AlignX::Center);
        frame_ptr->set_align_y(AlignY::Top);
        frame_ptr->set_height(0.5f);
        frame_ptr->set_scale(Scale::X);
        auto frame_easy_selector = event_manager.register_listener(
            Event("SelectEasyDifficulty"),
            [frame_ptr](Event const& event)
        {
            frame_ptr->set_align_y(AlignY::Top);
        });
        frame_ptr->add_listener(std::move(frame_easy_selector));
        auto frame_hard_selector = event_manager.register_listener(
            Event("SelectHardDifficulty"),
            [frame_ptr](Event const& event)
        {
            frame_ptr->set_align_y(AlignY::Bottom);
        });
        frame_ptr->add_listener(std::move(frame_hard_selector));

        // Create the "easy" text.
        auto easy_texture = get_resource_manager()->get_texture("img/easy.png");
        auto easy = std::make_unique<ImageWidget>(easy_texture);
        easy->set_align_x(AlignX::Center);
        easy->set_align_y(AlignY::Top);
        easy->set_height(0.5f);
        easy->set_scale(Scale::X);
        easy->add_mouse_enter_callback([frame_ptr, &event_manager](Widget & w) {
            event_manager.enqueue(Event("SelectEasyDifficulty"));
        });
        easy->add_click_end_callback([this, &event_manager](Widget & w) {
            event_manager.enqueue(Event("StartGame"));
        });
        container_ptr->add_widget(std::move(easy));

        // Create the "hard" text.
        auto hard_texture = get_resource_manager()->get_texture("img/hard.png");
        auto hard = std::make_unique<ImageWidget>(hard_texture);
        hard->set_align_x(AlignX::Center);
        hard->set_align_y(AlignY::Bottom);
        hard->set_height(0.5f);
        hard->set_scale(Scale::X);
        hard->add_mouse_enter_callback([&event_manager](Widget & w) {
            event_manager.enqueue(Event("SelectHardDifficulty"));
        });
        hard->add_click_end_callback([&event_manager](Widget & w) {
            event_manager.enqueue(Event("StartGame"));
        });
        container_ptr->add_widget(std::move(hard));

        // Create the container for the sound icons.
        auto sound_container = std::make_unique<Widget>();
        auto sound_container_ptr = get_gui().add_widget(std::move(sound_container));
        sound_container_ptr->set_align_x(AlignX::Right);
        sound_container_ptr->set_align_y(AlignY::Top);
        sound_container_ptr->set_x(0.02f);
        sound_container_ptr->set_y(0.02f);
        sound_container_ptr->set_height(0.05f);
        sound_container_ptr->set_z_index(100); // always on top
        
        // Create the different sound icons.
        std::string sound_file[] = {
            "img/sound_off_glow.png",
            "img/sound_off.png",
            "img/sound_on_glow.png",
            "img/sound_on.png" 
        };
        std::vector<Widget*> sound_ptr;
        for (int i = 0; i < 4; ++i)
        {
            // Create the sound widget.
            auto sound_texture = get_resource_manager()->get_texture(sound_file[i]);
            auto sound = std::make_unique<ImageWidget>(sound_texture);
            auto ptr = sound_container_ptr->add_widget(std::move(sound));
            sound_ptr.push_back(ptr);
            ptr->set_scale(Scale::X);
            ptr->set_align_x(AlignX::Right);
            ptr->set_z_index(i);

            // Change the z-index on a mouse enter or mouse leave event.
            ptr->add_mouse_enter_callback([i](Widget & w) {
                auto offset = (i == 0 || i == 2) ? 1 : -1;
                w.set_z_index(w.get_z_index() + offset);
            });
            ptr->add_mouse_leave_callback([i](Widget & w) {
                auto offset = (i == 1 || i == 3) ? 1 : -1;
                w.set_z_index(w.get_z_index() + offset);
            });

            // Toggle the visibility on a sound toggle event.
            auto sound_toggle = [ptr](Event const& event)
            {
                ptr->set_visible(!ptr->get_visible());
            };
            auto sound_on_listener = event_manager.register_listener(
                Event("SoundOn"),
                sound_toggle
            );
            ptr->add_listener(std::move(sound_on_listener));
            auto sound_off_listener = event_manager.register_listener(
                Event("SoundOff"),
                sound_toggle
            );
            ptr->add_listener(std::move(sound_off_listener));
        }

        // If this click callback would be added to all sound icon widgets,
        // it would be sent four times on a single click. To prevent this,
        // it is only added to one of the widgets.
        sound_ptr[0]->add_click_end_callback([sound_ptr, &event_manager](Widget & w) {
            if (sound_ptr[0]->get_visible())
                event_manager.enqueue(Event("SoundOn"));
            else
                event_manager.enqueue(Event("SoundOff"));
        });

        // By default, show the sound-on icons.
        // TODO: Read the actual state from an options object.
        sound_ptr[0]->set_visible(false);
        sound_ptr[1]->set_visible(false);
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
                auto event_manager_shared_ptr = get_event_manager();
                if (!event_manager_shared_ptr)
                {
                    throw sfe::GameException("The GameScreen needs an event manager.");
                }
                auto & event_manager = *event_manager_shared_ptr;

                // Check if the snake collides with itself or the boundary.
                auto const new_head = get_new_head();
                if (new_head.x < 0 || new_head.x >= num_fields_x ||
                    new_head.y < 0 || new_head.y >= num_fields_y ||
                    fields_(new_head.x, new_head.y) == FieldType::Snake)
                {
                    event_manager.enqueue(sfe::Event("GameOver"));
                }
                else
                {
                    // Move the snake and spawn an additional body part if food was collected.
                    bool const got_food = fields_(new_head.x, new_head.y) == FieldType::Food;
                    bool const got_coin = fields_(new_head.x, new_head.y) == FieldType::Coin;
                    move_snake(got_food || got_coin, new_head);
                    if (got_food)
                        event_manager.enqueue(sfe::Event("CollectedFood"));
                    else if (got_coin)
                        event_manager.enqueue(sfe::Event("CollectedCoin"));
                    else
                        event_manager.enqueue(sfe::Event("MovedSnake"));
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
        if (current_effect_ != Effect::None)
            effect_time_ += elapsed_time;
        if (current_effect_ == Effect::Wave)
        {
            auto r = 5 * std::sin(1 * effect_time_.asSeconds());
            get_game_view().setRotation(r);
        }
        else if (current_effect_ == Effect::FlashLight || current_effect_ == Effect::FlashDark)
        {
            sf::Color col = (current_effect_ == Effect::FlashLight) ? sf::Color::White : sf::Color::Black;
            auto const max_a = (current_effect_ == Effect::FlashLight) ? 112 : 165;
            if (effect_time_.asSeconds() <= 1.0f)
                col.a = 255 - static_cast<sf::Uint8>(effect_time_.asSeconds() * (255-max_a));
            else
                col.a = max_a;
            auto flash = dynamic_cast<sfe::ColorWidget*>(flash_);
            flash->set_color(col);
        }
    }

    inline void GameScreen::create_head_part(int x, int y)
    {
        // Create the snake part object and occupy the game field.
        snake_head_.x = x;
        snake_head_.y = y;
        fields_(x, y) = FieldType::Snake;

        // Create the game object and add it to the screen.
        auto const pos = field_to_view(x, y);
        auto snake_head_texture = get_resource_manager()->get_texture("img/snake_head.png");
        auto head = std::make_unique<sfe::ImageObject>(snake_head_texture);
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

        // Create the game object and add it to the screen.
        auto const pos = field_to_view(x, y);
        auto snake_body_texture = get_resource_manager()->get_texture("img/snake_body.png");
        auto body = std::make_unique<sfe::ImageObject>(snake_body_texture);
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
        // Find a random position on the field.
        auto const num_free_fields = std::count(fields_.begin(), fields_.end(), FieldType::Empty);
        std::uniform_int_distribution<> rand(0, num_free_fields - 1);
        auto pos = rand(rand_engine_);

        // Loop over all empty fields until the position is reached.
        int i = 0;
        for (size_t y = 0; y < fields_.height(); ++y)
        {
            for (size_t x = 0; x < fields_.width(); ++x)
            {
                if (fields_(x, y) != FieldType::Empty)
                    continue;

                // Update the food position.
                if (i == pos)
                {
                    fields_(x, y) = FieldType::Food;
                    food_->set_position(field_to_view(x, y));
                    food_->set_visible(true);
                    return;
                }
                ++i;
            }
        }
    }

    inline void GameScreen::add_special_effect()
    {
        using namespace sfe;

        // Create a random effect.
        auto num_effects = static_cast<int>(Effect::EffectCount);
        std::uniform_int_distribution<int> rand(1, num_effects - 1);
        auto eff = rand(rand_engine_);
        current_effect_ = static_cast<Effect>(eff);
        effect_time_ = sf::Time::Zero;

        // Initialize the effect.
        if (current_effect_ == Effect::Crooked)
        {
            get_game_view().setRotation(14);
        }
        else if (current_effect_ == Effect::FlashLight)
        {
            auto flash = std::make_unique<ColorWidget>(sf::Color(255, 255, 255, 255));
            flash_ = get_gui().add_widget(std::move(flash));
        }
        else if (current_effect_ == Effect::FlashDark)
        {
            auto flash = std::make_unique<ColorWidget>(sf::Color(0, 0, 0, 255));
            flash_ = get_gui().add_widget(std::move(flash));
        }
        else if (current_effect_ == Effect::Coins)
        {
            // Hide the food.
            for (auto & f : fields_)
                if (f == FieldType::Food)
                    f = FieldType::Empty;
            food_->set_visible(false);

            // Spawn the coins.
            while (snake_body_.size() > 1)
            {
                auto const part = snake_body_.back();
                snake_body_.pop_back();
                fields_(part.x, part.y) = FieldType::Coin;
                auto part_ptr = dynamic_cast<ImageObject*>(part.obj);
                if (part_ptr == nullptr)
                    throw ScreenException("GameScreen::add_special_effect(): Failed to cast snake body part to ImageObject*.");
                auto coin_texture = get_resource_manager()->get_texture("img/coin.png");
                part_ptr->set_texture(coin_texture);
                coins_[{part.x, part.y}] = part_ptr;
            }

        }
    }

    inline void GameScreen::clear_special_effects()
    {
        get_game_view().setRotation(0);
        if (current_effect_ == Effect::FlashLight || current_effect_ == Effect::FlashDark)
        {
            flash_->remove_from_parent();
            flash_ = nullptr;
        }
        current_effect_ = Effect::None;
    }

} // namespace snake

#endif
