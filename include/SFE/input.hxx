#ifndef SFE_INPUT_HXX
#define SFE_INPUT_HXX

#include <array>

#include <SFML/Window.hpp>

namespace sfe
{
    ////////////////////////////////////////////////////////////
    /// The keyboard class keeps track of the keys that were
    /// pressed since the last frame.
    ////////////////////////////////////////////////////////////
    class Input
    {
    public:

        ////////////////////////////////////////////////////////////
        /// The global keyboard object that can be used to retreive
        /// the status of each key.
        ////////////////////////////////////////////////////////////
        static Input & global();

        ////////////////////////////////////////////////////////////
        /// Refreshes the keyboard state. Should be called once per
        /// frame before the window input events are handled.
        ////////////////////////////////////////////////////////////
        void reset();

        ////////////////////////////////////////////////////////////
        /// Marks the given key as pressed. Should be called when
        /// the according sf::Keyboard::KeyPressed event is fired.
        ////////////////////////////////////////////////////////////
        void press(sf::Keyboard::Key k);

        ////////////////////////////////////////////////////////////
        /// Marks the given mouse button as pressed. Should be
        /// called when the according sf::Mouse::ButtonPressed event
        /// is fired.
        ////////////////////////////////////////////////////////////
        void press(sf::Mouse::Button b);

        ////////////////////////////////////////////////////////////
        /// Return whether the given key is currently held down.
        ////////////////////////////////////////////////////////////
        bool is_down(sf::Keyboard::Key k) const;

        ////////////////////////////////////////////////////////////
        /// Return whether the given mouse button is currently held
        /// down.
        ////////////////////////////////////////////////////////////
        bool is_down(sf::Mouse::Button b) const;

        ////////////////////////////////////////////////////////////
        /// Return whether the key was pressed in the last frame.
        ////////////////////////////////////////////////////////////
        bool is_pressed(sf::Keyboard::Key k) const;

        ////////////////////////////////////////////////////////////
        /// Return whether the mouse button was pressed in the last
        /// frame.
        ////////////////////////////////////////////////////////////
        bool is_pressed(sf::Mouse::Button b) const;

        ////////////////////////////////////////////////////////////
        /// Return whether the key was released in the last frame.
        ////////////////////////////////////////////////////////////
        bool is_released(sf::Keyboard::Key k) const;

        ////////////////////////////////////////////////////////////
        /// Return whether the mouse button was released in the last
        /// frame.
        ////////////////////////////////////////////////////////////
        bool is_released(sf::Mouse::Button b) const;

    private:

        ////////////////////////////////////////////////////////////
        /// Stores which keys are currently held down.
        ////////////////////////////////////////////////////////////
        std::array<bool, sf::Keyboard::KeyCount> key_down_;

        ////////////////////////////////////////////////////////////
        /// Stores which mouse buttons are currently held down.
        ////////////////////////////////////////////////////////////
        std::array<bool, sf::Mouse::ButtonCount> btn_down_;

        ////////////////////////////////////////////////////////////
        /// Stores which keys were pressed in the last frame.
        ////////////////////////////////////////////////////////////
        std::array<bool, sf::Keyboard::KeyCount> key_pressed_;

        ////////////////////////////////////////////////////////////
        /// Stores which mouse buttons were pressed in the last
        /// frame.
        ////////////////////////////////////////////////////////////
        std::array<bool, sf::Mouse::ButtonCount> btn_pressed_;

        ////////////////////////////////////////////////////////////
        /// Stores which keys were released in the last frame.
        ////////////////////////////////////////////////////////////
        std::array<bool, sf::Keyboard::KeyCount> key_released_;

        ////////////////////////////////////////////////////////////
        /// Stores which mouse buttons were released in the last
        /// frame.
        ////////////////////////////////////////////////////////////
        std::array<bool, sf::Mouse::ButtonCount> btn_released_;

    }; // class Input

} // namespace sfe

#endif
