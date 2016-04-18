#include <iostream>
#include <string>
#include <algorithm>

#include <boost/filesystem.hpp>

#include "SFE/widget.hxx"
#include "SFE/resource_manager.hxx"

void foo()
{
    auto p = boost::filesystem::current_path();
    std::cout << "current_path(): " << p.generic_string() << std::endl;
}

namespace sfe
{

    float Widget::viewport_ratio = 1.0f;

    Widget::Widget()
        :
        rect_({0.0f, 0.0f, 1.0f, 1.0f}),
        z_index_(0),
        align_x_(AlignX::Left),
        align_y_(AlignY::Top),
        scale_(Scale::None),
        ratio_(1.0f)
    {}

    Widget* Widget::add_widget(std::unique_ptr<Widget> w)
    {
        widgets_.push_back(std::move(w));
        return widgets_.back().get();
    }

    std::unique_ptr<Widget> Widget::remove_widget(Widget* w)
    {
        auto comp = [w](auto && wptr)
        {
            return wptr.get() == w;
        };
        auto it = std::find_if(widgets_.begin(), widgets_.end(), comp);
        if (it != widgets_.end())
        {
            auto wptr = std::move(*it);
            widgets_.erase(it);
            return wptr;
        }
        else
        {
            return std::unique_ptr<Widget>();
        }
    }

    void Widget::clear_widgets()
    {
        widgets_.clear();
    }

    void Widget::update(sf::Time elapsed_time)
    {
        update_impl(elapsed_time);
        sort_widgets();
        for (auto const & w : widgets_)
            w->update(elapsed_time);
    }

    void Widget::render(sf::RenderTarget & target) const
    {
        // Save the view.
        auto const old_view = target.getView();
        auto const & center = old_view.getCenter();
        auto const & size = old_view.getSize();

        // Compute the widget size with respect to the scale method.
        float width;
        float height;
        if (scale_ == Scale::X)
        {
            height = rect_.height;
            width = ratio_ * height / viewport_ratio;
        }
        else if (scale_ == Scale::Y)
        {
            width = rect_.width;
            height = width / ratio_ * viewport_ratio;
        }
        else // scale_ == Scale::None
        {
            width = rect_.width;
            height = rect_.height;
        }

        // Compute the widget position with respect to horizontal and vertical alignment.
        float left;
        if (align_x_ == AlignX::Left)
            left = rect_.left;
        else if (align_x_ == AlignX::Right)
            left = 1.0f - rect_.left - width;
        else // align_x_ == AlignX::Center
            left = 0.5f - 0.5f * width + rect_.left;
        float top;
        if (align_y_ == AlignY::Top)
            top = rect_.top;
        else if (align_y_ == AlignY::Bottom)
            top = 1.0f - rect_.top - height;
        else // align_y_ == AlignY::Center
            top = 0.5f - 0.5f * height+ rect_.top;

        // Assign the new view.
        sf::View view;
        view.setSize({ size.x / width, size.y / height });
        view.setCenter({ (center.x - left) / width, (center.y - top) / height });
        target.setView(view);

        // Render the widget and the subwidgets.
        render_impl(target);
        for (auto const & w : widgets_)
            w->render(target);

        // Restore the old view.
        target.setView(old_view);
    }

    float Widget::get_x() const
    {
        return rect_.left;
    }

    void Widget::set_x(float x)
    {
        rect_.left = x;
    }

    float Widget::get_y() const
    {
        return rect_.top;
    }

    void Widget::set_y(float y)
    {
        rect_.top = y;
    }

    float Widget::get_width() const
    {
        return rect_.width;
    }

    void Widget::set_width(float w)
    {
        rect_.width = w;
    }

    float Widget::get_height() const
    {
        return rect_.height;
    }

    void Widget::set_height(float h)
    {
        rect_.height = h;
    }

    int Widget::get_z_index() const
    {
        return z_index_;
    }

    void Widget::set_z_index(int z_index)
    {
        z_index_ = z_index;
    }

    AlignX Widget::get_align_x() const
    {
        return align_x_;
    }

    void Widget::set_align_x(AlignX a)
    {
        align_x_ = a;
    }

    AlignY Widget::get_align_y() const
    {
        return align_y_;
    }

    void Widget::set_align_y(AlignY a)
    {
        align_y_ = a;
    }

    Scale Widget::get_scale() const
    {
        return scale_;
    }

    void Widget::set_scale(Scale s)
    {
        scale_ = s;
    }

    float Widget::get_ratio() const
    {
        return ratio_;
    }

    void Widget::set_ratio(float r)
    {
        ratio_ = r;
    }

    void Widget::sort_widgets()
    {
        auto comp = [](auto && a, auto && b)
        {
            return a->get_z_index() < b->get_z_index();
        };
        std::sort(widgets_.begin(), widgets_.end(), comp);
    }

    ColorWidget::ColorWidget(sf::Color color)
        :
        color_(std::move(color))
    {}

    void ColorWidget::render_impl(sf::RenderTarget & target) const
    {
        auto r = sf::RectangleShape({ 1.0f, 1.0f });
        r.setFillColor(color_);
        target.draw(r);
    }

    ImageWidget::ImageWidget(std::string const & string)
        :
        texture_(ResourceManager::global().get_texture(string))
    {
        set_ratio(texture_.getSize().x / static_cast<float>(texture_.getSize().y));
    }

    void ImageWidget::render_impl(sf::RenderTarget & target) const
    {
        sf::Sprite spr(texture_);
        spr.setScale(1.0f / texture_.getSize().x, 1.0f / texture_.getSize().y);
        target.draw(spr);
    }

} // namespace sfe
