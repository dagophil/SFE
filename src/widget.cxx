#include <string>
#include <algorithm>

#include "SFE/widget.hxx"
#include "SFE/resource_manager.hxx"
#include "SFE/input.hxx"

namespace sfe
{

    float Widget::viewport_ratio = 1.0f;

    Widget::Widget()
        :
        rect_({0.0f, 0.0f, 1.0f, 1.0f}),
        visible_(true),
        z_index_(0),
        align_x_(AlignX::Left),
        align_y_(AlignY::Top),
        scale_(Scale::None),
        ratio_(1.0f),
        mouseover_(false),
        mousedown_(false),
        absorb_click_(false),
        remove_this_(false)
    {}

    Widget* Widget::add_widget(std::unique_ptr<Widget> w)
    {
        auto ret = w.get();

        // Use insertion sort with respect to the z-index.
        auto comp = [](auto && a, auto && b)
        {
            return a->get_z_index() < b->get_z_index();
        };
        auto it = std::lower_bound(widgets_.begin(), widgets_.end(), w, comp);
        widgets_.insert(it, std::move(w));
        return ret;
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

    void Widget::remove_from_parent()
    {
        remove_this_ = true;
    }

    void Widget::clear_widgets()
    {
        widgets_.clear();
    }

    bool Widget::update_mouse(float x, float y)
    {
        // Update the mouseoverstate.
        auto old_mouseover = mouseover_;
        mouseover_ = render_rect_.contains(x, y);

        // Fire the mouseover events.
        if (!old_mouseover && mouseover_)
            for (auto const & f : mouse_enter_callbacks_)
                f(*this);
        else if (old_mouseover && !mouseover_)
            for (auto const & f : mouse_leave_callbacks_)
                f(*this);

        // Update the subwidgets.
        bool handled = false;
        for (auto const & w : widgets_)
            if (w->update_mouse(x, y))
                handled = true;

        // Fire the click callbacks.
        if (!handled)
        {
            if (mouseover_ && sfe::Input::global().is_pressed(sf::Mouse::Left))
            {
                for (auto const & f : click_begin_callbacks_)
                    f(*this);
                mousedown_ = true;
            }
            if (mousedown_ && mouseover_ && sfe::Input::global().is_released(sf::Mouse::Left))
            {
                for (auto const & f : click_end_callbacks_)
                    f(*this);
            }
        }
        if (sfe::Input::global().is_released(sf::Mouse::Left))
            mousedown_ = false;

        // Check if the click should be absorbed.
        if (mouseover_ && absorb_click_)
            handled = true;
        return handled;

    }

    void Widget::update(sf::Time elapsed_time)
    {
        // Update this widget and the subwidgets.
        update_impl(elapsed_time);
        sort_widgets();
        for (auto const & w : widgets_)
            w->update(elapsed_time);

        //// Remove subwidgets that are marked for removal.
        widgets_.erase(
            std::remove_if(widgets_.begin(), widgets_.end(), [](auto && w) {
                return w->remove_this_;
            }),
            widgets_.end()
        );
    }

    void Widget::render(sf::RenderTarget & target, sf::FloatRect const & parent_render_rect) const
    {
        if (visible_)
        {
            // Render the widget and the subwidgets.
            render_rect_ = compute_render_rect(parent_render_rect);
            render_impl(target);
            for (auto const & w : widgets_)
                w->render(target, render_rect_);
        }
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

    bool Widget::get_visible() const
    {
        return visible_;
    }

    void Widget::set_visible(bool b)
    {
        visible_ = b;
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

    bool Widget::get_mouseover() const
    {
        return mouseover_;
    }

    bool Widget::get_absorb_click() const
    {
        return absorb_click_;
    }

    void Widget::set_absorb_click(bool absorb_click)
    {
        absorb_click_ = absorb_click;
    }

    sf::FloatRect const & Widget::get_render_rect() const
    {
        return render_rect_;
    }

    void Widget::add_mouse_enter_callback(CallbackFunction && f)
    {
        mouse_enter_callbacks_.emplace_back(f);
    }

    void Widget::add_mouse_leave_callback(CallbackFunction && f)
    {
        mouse_leave_callbacks_.emplace_back(f);
    }

    void Widget::add_click_begin_callback(CallbackFunction && f)
    {
        click_begin_callbacks_.emplace_back(f);
    }

    void Widget::add_click_end_callback(CallbackFunction && f)
    {
        click_end_callbacks_.emplace_back(f);
    }

    void Widget::clear_mouse_enter_callbacks()
    {
        mouse_enter_callbacks_.clear();
    }

    void Widget::clear_mouse_leave_callbacks()
    {
        mouse_leave_callbacks_.clear();
    }

    void Widget::clear_click_begin_callbacks()
    {
        click_begin_callbacks_.clear();
    }

    void Widget::clear_click_end_callbacks()
    {
        click_end_callbacks_.clear();
    }

    void Widget::add_listener(std::unique_ptr<Listener> listener)
    {
        listeners_.push_back(std::move(listener));
    }

    void Widget::update_impl(sf::Time elapsed_time)
    {}

    void Widget::render_impl(sf::RenderTarget & target) const
    {}

    sf::FloatRect Widget::compute_render_rect(sf::FloatRect const & parent_render_rect) const
    {
        // Compute the size with respect to the scale method.
        sf::FloatRect r = parent_render_rect;
        if (scale_ == Scale::X)
        {
            r.height *= rect_.height;
            r.width = r.height * ratio_ / viewport_ratio;
        }
        else if (scale_ == Scale::Y)
        {
            r.width *= rect_.width;
            r.height = r.width / ratio_ * viewport_ratio;
        }
        else // scale_ == Scale::None
        {
            r.width *= rect_.width;
            r.height *= rect_.height;
        }

        // Compute the widget position with respect to horizontal and vertical alignment.
        if (align_x_ == AlignX::Left)
            r.left += rect_.left * parent_render_rect.width;
        else if (align_x_ == AlignX::Right)
            r.left += (1.0f - rect_.left) * parent_render_rect.width - r.width;
        else // align_x_ == AlignX::Center
            r.left += (0.5f + rect_.left) * parent_render_rect.width - 0.5f * r.width;
        if (align_y_ == AlignY::Top)
            r.top += rect_.top * parent_render_rect.height;
        else if (align_y_ == AlignY::Bottom)
            r.top += (1.0f - rect_.top) * parent_render_rect.height - r.height;
        else // align_y_ == AlignY::Center
            r.top += (0.5f + rect_.top) * parent_render_rect.height - 0.5f * r.height;

        return r;
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
        auto const & rect = get_render_rect();
        auto r = sf::RectangleShape({ rect.width, rect.height });
        r.setPosition(rect.left, rect.top);
        r.setFillColor(color_);
        target.draw(r);
    }

    ImageWidget::ImageWidget(std::string const & filename)
        :
        texture_(ResourceManager::global().get_texture(filename))
    {
        set_ratio(texture_.getSize().x / static_cast<float>(texture_.getSize().y));
    }

    void ImageWidget::render_impl(sf::RenderTarget & target) const
    {
        auto const & r = get_render_rect();
        sf::Sprite spr(texture_);
        spr.setPosition(r.left, r.top);
        spr.setScale(r.width / texture_.getSize().x, r.height / texture_.getSize().y);
        target.draw(spr);
    }

} // namespace sfe
