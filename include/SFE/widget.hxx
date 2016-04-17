#ifndef SFE_WIDGET_HXX
#define SFE_WIDGET_HXX

#include <memory>

#include <SFML/Graphics.hpp>

#include "resource_manager.hxx"

namespace sfe
{
    /// Horizontal alignment.
    enum class AlignX
    {
        Left,
        Center,
        Right
    };

    /// Vertical alignment.
    enum class AlignY
    {
        Top,
        Center,
        Bottom
    };

    /// The scale method.
    enum class Scale
    {
        None,
        X,
        Y
    };

    /// Base class for GUI widgets.
    class Widget
    {
    public:

        /// The viewport ratio (width divided by height).
        static float viewport_ratio;

        /// The default constructor creates the widget at (0, 0) with size (1, 1).
        Widget();
        
        /// Virtual default destructor.
        virtual ~Widget() = default;

        /// Add the given widget as subwidget.
        /// For convenience, a non-owning widget pointer to the subwidget is returned.
        Widget* add_widget(std::unique_ptr<Widget> w);

        /// Remove the given widget.
        /// For convenience, the widget is returned so it can be reused.
        std::unique_ptr<Widget> remove_widget(Widget* w);

        /// Remove all subwidgets.
        void clear_widgets();

        ///// Return the subwidgets.
        //std::vector<std::unique_ptr<Widget> > const & get_widgets() const;

        /// Update the widget and all subwidgets.
        void update(sf::Time elapsed_time);

        /// Render the widget and all subwidgets.
        void render(sf::RenderTarget & target) const;

        /// Return the x-coordinate.
        float get_x() const;

        /// Set the x-coordinate.
        void set_x(float x);

        /// Return the y-coordinate.
        float get_y() const;

        /// Set the y-coordinate.
        void set_y(float y);

        /// Return the width.
        float get_width() const;

        /// Set the width.
        void set_width(float w);

        /// Return the height.
        float get_height() const;

        /// Set the height.
        void set_height(float h);

        /// Return the z-index.
        int get_z_index() const;

        /// Set the z-index.
        void set_z_index(int z_index);

        /// Return the horizontal alignment.
        AlignX get_align_x() const;

        /// Set the horizontal alignment.
        void set_align_x(AlignX a);

        /// Return the vertical alignment.
        AlignY get_align_y() const;

        /// Set the vertical alignment.
        void set_align_y(AlignY a);

        /// Return the scale method.
        Scale get_scale() const;

        /// Set the scale method.
        void set_scale(Scale s);

    protected:

        /// Concrete update method that can be overwritten on subclasses.
        virtual void update_impl(sf::Time elapsed_time)
        {}

        /// Concrete render method that can be overwritten on subclasses.
        virtual void render_impl(sf::RenderTarget & target) const
        {}

    private:

        /// Sort the subwidgets by ascending z-index.
        void sort_widgets();
        
        /// The rectangle.
        sf::FloatRect rect_;

        /// The z-index.
        int z_index_;

        /// The horizontal alignment.
        AlignX align_x_;

        /// The vertical alignment.
        AlignY align_y_;

        /// The scale method.
        Scale scale_;

        /// The scale ratio (width divided by height), used if scale_ is ScaleInX or ScaleInY.
        float ratio_;

        /// The subwidgets.
        std::vector<std::unique_ptr<Widget> > widgets_;

    }; // class Widget

    /// A color widget consists of a single color.
    class ColorWidget : public Widget
    {
    public:

        /// Create color widget with the given color.
        ColorWidget(sf::Color color);

    protected:

        /// Draw the color.
        virtual void render_impl(sf::RenderTarget & target) const override;

    private:

        /// The color.
        sf::Color color_;

    };

} // namespace sfe

#endif
