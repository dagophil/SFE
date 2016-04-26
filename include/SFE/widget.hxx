#ifndef SFE_WIDGET_HXX
#define SFE_WIDGET_HXX

#include <memory>
#include <functional>

#include <SFML/Graphics.hpp>

#include "event_manager.hxx"

namespace sfe
{
    ////////////////////////////////////////////////////////////
    /// Horizontal alignment.
    ////////////////////////////////////////////////////////////
    enum class AlignX
    {
        Left,
        Center,
        Right
    };

    ////////////////////////////////////////////////////////////
    /// Vertical alignment.
    ////////////////////////////////////////////////////////////
    enum class AlignY
    {
        Top,
        Center,
        Bottom
    };

    ////////////////////////////////////////////////////////////
    /// The scale method.
    ////////////////////////////////////////////////////////////
    enum class Scale
    {
        None,
        X,
        Y
    };

    ////////////////////////////////////////////////////////////
    /// Base class for GUI widgets.
    ////////////////////////////////////////////////////////////
    class Widget
    {
    public:

        ////////////////////////////////////////////////////////////
        /// The callback function type.
        ////////////////////////////////////////////////////////////
        typedef std::function<void(Widget &)> CallbackFunction;

        ////////////////////////////////////////////////////////////
        /// The viewport ratio (width divided by height).
        ////////////////////////////////////////////////////////////
        static float viewport_ratio;

        ////////////////////////////////////////////////////////////
        /// The default constructor creates the widget at (0, 0)
        /// with size (1, 1).
        ////////////////////////////////////////////////////////////
        Widget();
        
        ////////////////////////////////////////////////////////////
        /// Virtual default destructor.
        ////////////////////////////////////////////////////////////
        virtual ~Widget() = default;

        ////////////////////////////////////////////////////////////
        /// Add the given widget as subwidget.
        /// For convenience, a non-owning widget pointer to the
        /// subwidget is returned.
        ////////////////////////////////////////////////////////////
        Widget* add_widget(std::unique_ptr<Widget> w);

        ////////////////////////////////////////////////////////////
        /// Remove the given widget.
        /// For convenience, the widget is returned so it can be
        /// reused.
        ////////////////////////////////////////////////////////////
        std::unique_ptr<Widget> remove_widget(Widget* w);

        ////////////////////////////////////////////////////////////
        /// Marks the widget fpr removal. The widget will be removed
        /// in the next update call on the parent widget.
        ////////////////////////////////////////////////////////////
        void remove_from_parent();

        ////////////////////////////////////////////////////////////
        /// Remove all subwidgets.
        ////////////////////////////////////////////////////////////
        void clear_widgets();

        ////////////////////////////////////////////////////////////
        /// Update the mouse and raise mouseover and click events.
        ////////////////////////////////////////////////////////////
        bool update_mouse(float x, float y);

        ////////////////////////////////////////////////////////////
        /// Update the widget and all subwidgets.
        ////////////////////////////////////////////////////////////
        void update(sf::Time elapsed_time);

        ////////////////////////////////////////////////////////////
        /// Render the widget and all subwidgets.
        ////////////////////////////////////////////////////////////
        void render(sf::RenderTarget & target, sf::FloatRect const & parent_render_rect) const;

        ////////////////////////////////////////////////////////////
        /// Return the x-coordinate.
        ////////////////////////////////////////////////////////////
        float get_x() const;
        
        ////////////////////////////////////////////////////////////
        /// Set the x-coordinate.
        ////////////////////////////////////////////////////////////
        void set_x(float x);

        ////////////////////////////////////////////////////////////
        /// Return the y-coordinate.
        ////////////////////////////////////////////////////////////
        float get_y() const;

        ////////////////////////////////////////////////////////////
        /// Set the y-coordinate.
        ////////////////////////////////////////////////////////////
        void set_y(float y);

        ////////////////////////////////////////////////////////////
        /// Return the width.
        ////////////////////////////////////////////////////////////
        float get_width() const;

        ////////////////////////////////////////////////////////////
        /// Set the width.
        ////////////////////////////////////////////////////////////
        void set_width(float w);

        ////////////////////////////////////////////////////////////
        /// Return the height.
        ////////////////////////////////////////////////////////////
        float get_height() const;

        ////////////////////////////////////////////////////////////
        /// Set the height.
        ////////////////////////////////////////////////////////////
        void set_height(float h);

        ////////////////////////////////////////////////////////////
        /// Return whether the widget is visible.
        ////////////////////////////////////////////////////////////
        bool get_visible() const;

        ////////////////////////////////////////////////////////////
        /// Set whether the widget is visible.
        ////////////////////////////////////////////////////////////
        void set_visible(bool b);

        ////////////////////////////////////////////////////////////
        /// Return the z-index.
        ////////////////////////////////////////////////////////////
        int get_z_index() const;

        ////////////////////////////////////////////////////////////
        /// Set the z-index.
        ////////////////////////////////////////////////////////////
        void set_z_index(int z_index);

        ////////////////////////////////////////////////////////////
        /// Return the horizontal alignment.
        ////////////////////////////////////////////////////////////
        AlignX get_align_x() const;

        ////////////////////////////////////////////////////////////
        /// Set the horizontal alignment.
        ////////////////////////////////////////////////////////////
        void set_align_x(AlignX a);

        ////////////////////////////////////////////////////////////
        /// Return the vertical alignment.
        ////////////////////////////////////////////////////////////
        AlignY get_align_y() const;

        ////////////////////////////////////////////////////////////
        /// Set the vertical alignment.
        ////////////////////////////////////////////////////////////
        void set_align_y(AlignY a);

        ////////////////////////////////////////////////////////////
        /// Return the scale method.
        ////////////////////////////////////////////////////////////
        Scale get_scale() const;

        ////////////////////////////////////////////////////////////
        /// Set the scale method.
        ////////////////////////////////////////////////////////////
        void set_scale(Scale s);

        ////////////////////////////////////////////////////////////
        /// Return the ratio.
        ////////////////////////////////////////////////////////////
        float get_ratio() const;

        ////////////////////////////////////////////////////////////
        /// Set the ratio.
        ////////////////////////////////////////////////////////////
        void set_ratio(float r);

        ////////////////////////////////////////////////////////////
        /// Return whether the widget rectangle contains the mouse.
        ////////////////////////////////////////////////////////////
        bool get_mouseover() const;

        ////////////////////////////////////////////////////////////
        /// Return whether clicks are absorbed by this widget.
        /// If this is true, the click event is not passed onto
        /// widgets beneath this one.
        ////////////////////////////////////////////////////////////
        bool get_absorb_click() const;

        ////////////////////////////////////////////////////////////
        /// Set whether clicks are absorbed by this widget.
        /// If this is true, the click event is not passed onto
        /// widgets beneath this one.
        ////////////////////////////////////////////////////////////
        void set_absorb_click(bool absorb_click);

        ////////////////////////////////////////////////////////////
        /// Return the rectangle that is used for rendering.
        ////////////////////////////////////////////////////////////
        sf::FloatRect const & get_render_rect() const;

        ////////////////////////////////////////////////////////////
        /// Add a callback for the mouse enter event.
        ////////////////////////////////////////////////////////////
        void add_mouse_enter_callback(CallbackFunction && f);

        ////////////////////////////////////////////////////////////
        /// Add a callback for the mouse leave event.
        ////////////////////////////////////////////////////////////
        void add_mouse_leave_callback(CallbackFunction && f);

        ////////////////////////////////////////////////////////////
        /// Add a callback for the click begin event.
        ////////////////////////////////////////////////////////////
        void add_click_begin_callback(CallbackFunction && f);

        ////////////////////////////////////////////////////////////
        /// Add a callback for the click end event.
        void add_click_end_callback(CallbackFunction && f);

        ////////////////////////////////////////////////////////////
        /// Clear the callbacks for the mouse enter event.
        ////////////////////////////////////////////////////////////
        void clear_mouse_enter_callbacks();

        ////////////////////////////////////////////////////////////
        /// Clear the callbacks for the mouse leave event.
        ////////////////////////////////////////////////////////////
        void clear_mouse_leave_callbacks();

        ////////////////////////////////////////////////////////////
        /// Clear the callbacks for the click begin event.
        ////////////////////////////////////////////////////////////
        void clear_click_begin_callbacks();

        ////////////////////////////////////////////////////////////
        /// Clear the callbacks for the click end event.
        ////////////////////////////////////////////////////////////
        void clear_click_end_callbacks();

        ////////////////////////////////////////////////////////////
        /// Add an event listener.
        ////////////////////////////////////////////////////////////
        void add_listener(std::unique_ptr<Listener> listener);

    protected:

        ////////////////////////////////////////////////////////////
        /// Concrete update method that can be overwritten on
        /// subclasses.
        ////////////////////////////////////////////////////////////
        virtual void update_impl(sf::Time elapsed_time);

        ////////////////////////////////////////////////////////////
        /// Concrete render method that can be overwritten on
        /// subclasses.
        ////////////////////////////////////////////////////////////
        virtual void render_impl(sf::RenderTarget & target) const;

    private:

        ////////////////////////////////////////////////////////////
        /// Compute the render rectangle with respect to the scale
        /// method and the alignment.
        ////////////////////////////////////////////////////////////
        sf::FloatRect compute_render_rect(sf::FloatRect const & parent_render_rect) const;

        ////////////////////////////////////////////////////////////
        /// Sort the subwidgets by ascending z-index.
        ////////////////////////////////////////////////////////////
        void sort_widgets();
        
        ////////////////////////////////////////////////////////////
        /// The rectangle.
        ////////////////////////////////////////////////////////////
        sf::FloatRect rect_;

        ////////////////////////////////////////////////////////////
        /// The actual rectangle that was used in the last render()
        /// method. Opposed to rect_, the render_rect_ is computed
        /// with respect to scale and alignment.
        ////////////////////////////////////////////////////////////
        mutable sf::FloatRect render_rect_;

        ////////////////////////////////////////////////////////////
        /// Whether the widget is visible.
        ////////////////////////////////////////////////////////////
        bool visible_;

        ////////////////////////////////////////////////////////////
        /// The z-index.
        ////////////////////////////////////////////////////////////
        int z_index_;

        ////////////////////////////////////////////////////////////
        /// The horizontal alignment.
        ////////////////////////////////////////////////////////////
        AlignX align_x_;

        ////////////////////////////////////////////////////////////
        /// The vertical alignment.
        ////////////////////////////////////////////////////////////
        AlignY align_y_;

        ////////////////////////////////////////////////////////////
        /// The scale method.
        ////////////////////////////////////////////////////////////
        Scale scale_;

        ////////////////////////////////////////////////////////////
        /// The scale ratio (width divided by height), used if
        /// scale_ is ScaleInX or ScaleInY.
        ////////////////////////////////////////////////////////////
        float ratio_;

        ////////////////////////////////////////////////////////////
        /// True if the widget rectangle contains the mouse.
        ////////////////////////////////////////////////////////////
        bool mouseover_;

        ////////////////////////////////////////////////////////////
        /// True if a mouse button was pressed while the widget was
        /// hovered and the mouse button was not released yet.
        ////////////////////////////////////////////////////////////
        bool mousedown_;

        ////////////////////////////////////////////////////////////
        /// Whether the click event is absorbed or passed onto
        /// widgets beneath this one.
        ////////////////////////////////////////////////////////////
        bool absorb_click_;

        ////////////////////////////////////////////////////////////
        /// The parent widget will remove this widget once the
        /// remove_this_ flag becomes true.
        ////////////////////////////////////////////////////////////
        bool remove_this_;

        ////////////////////////////////////////////////////////////
        /// The subwidgets.
        ////////////////////////////////////////////////////////////
        std::vector<std::unique_ptr<Widget> > widgets_;

        ////////////////////////////////////////////////////////////
        /// The callbacks for the mouse enter event.
        ////////////////////////////////////////////////////////////
        std::vector<CallbackFunction> mouse_enter_callbacks_;

        ////////////////////////////////////////////////////////////
        /// The callbacks for the mouse leave event.
        ////////////////////////////////////////////////////////////
        std::vector<CallbackFunction> mouse_leave_callbacks_;

        ////////////////////////////////////////////////////////////
        /// The callbacks for the click begin event.
        ////////////////////////////////////////////////////////////
        std::vector<CallbackFunction> click_begin_callbacks_;

        ////////////////////////////////////////////////////////////
        /// The callbacks for the click end event.
        ////////////////////////////////////////////////////////////
        std::vector<CallbackFunction> click_end_callbacks_;

        ////////////////////////////////////////////////////////////
        /// A container for event listeners.
        ////////////////////////////////////////////////////////////
        std::vector<std::unique_ptr<Listener> > listeners_;

    }; // class Widget

    ////////////////////////////////////////////////////////////
    /// A color widget consists of a single color.
    ////////////////////////////////////////////////////////////
    class ColorWidget : public Widget
    {
    public:

        ////////////////////////////////////////////////////////////
        /// Create color widget with the given color.
        ////////////////////////////////////////////////////////////
        ColorWidget(sf::Color color);

        ////////////////////////////////////////////////////////////
        /// Return the color.
        ////////////////////////////////////////////////////////////
        sf::Color const & get_color() const;

        ////////////////////////////////////////////////////////////
        /// Set the color.
        ////////////////////////////////////////////////////////////
        void set_color(sf::Color const & color);

    protected:

        ////////////////////////////////////////////////////////////
        /// Draw a rectangle of the stored color.
        ////////////////////////////////////////////////////////////
        virtual void render_impl(sf::RenderTarget & target) const override;

    private:

        ////////////////////////////////////////////////////////////
        /// The color.
        ////////////////////////////////////////////////////////////
        sf::Color color_;

    }; // class ColorWidget

    ////////////////////////////////////////////////////////////
    /// A widget that displays an image.
    ////////////////////////////////////////////////////////////
    class ImageWidget : public Widget
    {
    public:

        ////////////////////////////////////////////////////////////
        /// Create a widget from the given image.
        ////////////////////////////////////////////////////////////
        ImageWidget(std::string const & filename);

    protected:

        ////////////////////////////////////////////////////////////
        /// Draw the image.
        ////////////////////////////////////////////////////////////
        virtual void render_impl(sf::RenderTarget & target) const override;

    private:

        ////////////////////////////////////////////////////////////
        /// The stored image.
        ////////////////////////////////////////////////////////////
        sf::Texture const & texture_;

    }; // class ImageWidget

} // namespace sfe

#endif
