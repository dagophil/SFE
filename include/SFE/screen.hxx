#ifndef SFE_SCREEN_HXX
#define SFE_SCREEN_HXX

#include <vector>

#include "game_object.hxx"
#include "widget.hxx"

namespace sfe
{
    ////////////////////////////////////////////////////////////
    /// A screen holds the gui widgets and the game objects.
    ////////////////////////////////////////////////////////////
    class Screen
    {
    public:

        ////////////////////////////////////////////////////////////
        /// Construct a screen with the given game view.
        ////////////////////////////////////////////////////////////
        Screen(sf::View game_view);

        ////////////////////////////////////////////////////////////
        /// Update the gui and the game objects.
        ////////////////////////////////////////////////////////////
        void update(sf::RenderWindow const & window, sf::Time elapsed_time);

        ////////////////////////////////////////////////////////////
        /// Render the gui and the game objects.
        ////////////////////////////////////////////////////////////
        void render(sf::RenderTarget & target) const;

        ////////////////////////////////////////////////////////////
        /// Process the user input.
        ////////////////////////////////////////////////////////////
        void process_input(int mouse_x, int mouse_y);

        ////////////////////////////////////////////////////////////
        /// Return the gui widget.
        ////////////////////////////////////////////////////////////
        Widget & get_gui();

        ////////////////////////////////////////////////////////////
        /// Return the gui widget.
        ////////////////////////////////////////////////////////////
        Widget const & get_gui() const;

        ////////////////////////////////////////////////////////////
        /// Add a game object to the screen.
        ////////////////////////////////////////////////////////////
        GameObject* add_game_object(std::unique_ptr<GameObject> obj);

        ////////////////////////////////////////////////////////////
        /// Remove a game object from the screen.
        /// For convenience, the object is returned so it can be
        /// reused.
        ////////////////////////////////////////////////////////////
        std::unique_ptr<GameObject> remove_game_object(GameObject* obj);

        ////////////////////////////////////////////////////////////
        /// Clear all game objects.
        ////////////////////////////////////////////////////////////
        void clear_game_objects();

        ////////////////////////////////////////////////////////////
        /// Return the game view.
        ////////////////////////////////////////////////////////////
        sf::View & get_game_view();

        ////////////////////////////////////////////////////////////
        /// Return the game view.
        ////////////////////////////////////////////////////////////
        sf::View const & get_game_view() const;

        ////////////////////////////////////////////////////////////
        /// Set the game view.
        ////////////////////////////////////////////////////////////
        void set_game_view(sf::View const & view);

    private:

        ////////////////////////////////////////////////////////////
        /// The game objects.
        ////////////////////////////////////////////////////////////
        std::vector<std::unique_ptr<GameObject> > game_objects_;

        ////////////////////////////////////////////////////////////
        /// The gui widget.
        ////////////////////////////////////////////////////////////
        Widget gui_;

        ////////////////////////////////////////////////////////////
        /// The game view.
        ////////////////////////////////////////////////////////////
        sf::View game_view_;

    }; // class Screen

} // namespace sfe

#endif
