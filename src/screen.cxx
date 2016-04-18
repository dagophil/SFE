#include "SFE/screen.hxx"

namespace sfe
{

    Screen::Screen(unsigned int window_width, unsigned int window_height)
    {
        resize(window_width, window_height);
    }

    void Screen::resize(unsigned int window_width, unsigned int window_height)
    {
        Widget::viewport_ratio = window_width / static_cast<float>(window_height);
    }

    void Screen::update(sf::Time elapsed_time)
    {
        gui_.update(elapsed_time);
    }

    void Screen::render(sf::RenderTarget & target) const
    {
        target.setView({ { 0.5f, 0.5f },{ 1.0f, 1.0f } });
        gui_.render(target);
    }

    Widget & Screen::get_gui()
    {
        return gui_;
    }

    Widget const & Screen::get_gui() const
    {
        return gui_;
    }

} // namespace sfe
