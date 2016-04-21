#include <iostream>

#include "SFE/input.hxx"

namespace sfe
{

    Input & Input::global()
    {
        static Input instance;
        return instance;
    }

    void Input::reset()
    {
        // Reset the pressed and released states.
        key_pressed_.fill(false);
        key_released_.fill(false);
        btn_pressed_.fill(false);
        btn_released_.fill(false);

        // Check which keys were released.
        for (int k = 0; k < sf::Keyboard::KeyCount; ++k)
        {
            auto const key = static_cast<sf::Keyboard::Key>(k);
            if (key_down_[k] && !sf::Keyboard::isKeyPressed(key))
            {
                key_down_[k] = false;
                key_released_[k] = true;
            }
        }

        // Check which mouse buttons were released.
        for (int b = 0; b < sf::Mouse::ButtonCount; ++b)
        {
            auto const btn = static_cast<sf::Mouse::Button>(b);
            if (btn_down_[b] && !sf::Mouse::isButtonPressed(btn))
            {
                btn_down_[b] = false;
                btn_released_[b] = true;
            }
        }
    }

    void Input::press(sf::Keyboard::Key k)
    {
        auto i = static_cast<int>(k);
        key_down_[i] = true;
        key_pressed_[i] = true;
    }

    void Input::press(sf::Mouse::Button b)
    {
        auto i = static_cast<int>(b);
        btn_down_[i] = true;
        btn_pressed_[i] = true;
    }

    bool Input::is_down(sf::Keyboard::Key k)
    {
        auto i = static_cast<int>(k);
        return key_down_[i];
    }

    bool Input::is_down(sf::Mouse::Button b)
    {
        auto i = static_cast<int>(b);
        return btn_down_[i];
    }

    bool Input::is_pressed(sf::Keyboard::Key k)
    {
        auto i = static_cast<int>(k);
        return key_pressed_[i];
    }

    bool Input::is_pressed(sf::Mouse::Button b)
    {
        auto i = static_cast<int>(b);
        return btn_pressed_[i];
    }

    bool Input::is_released(sf::Keyboard::Key k)
    {
        auto i = static_cast<int>(k);
        return key_released_[i];
    }

    bool Input::is_released(sf::Mouse::Button b)
    {
        auto i = static_cast<int>(b);
        return btn_released_[i];
    }

} // namespace sfe
