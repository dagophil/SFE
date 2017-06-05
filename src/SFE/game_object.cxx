#include <SFE/game_object.hxx>

namespace sfe
{
    GameObject::GameObject()
        :
        position_({ 0, 0 }),
        size_({ 1, 1 }),
        rotation_(0),
        z_index_(0),
        visible_(true)
    {}

    void GameObject::update(sf::Time elapsed_time)
    {}

    void GameObject::render(sf::RenderTarget & target) const
    {
        if (visible_)
            render_impl(target);
    }

    sf::Vector2f const & GameObject::get_position() const
    {
        return position_;
    }

    void GameObject::set_position(sf::Vector2f const & position)
    {
        position_ = position;
    }

    void GameObject::set_position(float x, float y)
    {
        position_.x = x;
        position_.y = y;
    }

    sf::Vector2f const & GameObject::get_size() const
    {
        return size_;
    }

    void GameObject::set_size(sf::Vector2f const & size)
    {
        size_ = size;
    }

    void GameObject::set_size(float width, float height)
    {
        size_.x = width;
        size_.y = height;
    }

    float GameObject::get_rotation() const
    {
        return rotation_;
    }
    
    void GameObject::set_rotation(float angle)
    {
        rotation_ = angle;
    }

    void GameObject::rotate(float angle)
    {
        rotation_ += angle;
    }

    int GameObject::get_z_index() const
    {
        return z_index_;
    }

    void GameObject::set_z_index(int z_index)
    {
        z_index_ = z_index;
    }

    bool GameObject::get_visible() const
    {
        return visible_;
    }

    void GameObject::set_visible(bool b)
    {
        visible_ = b;
    }

    ImageObject::ImageObject(std::shared_ptr<sf::Texture> const& texture)
        :
        texture_(texture),
        mirror_x_(false),
        mirror_y_(false)
    {}

    void ImageObject::render_impl(sf::RenderTarget & target) const
    {
        auto const & texture = *texture_;
        sf::Sprite spr(texture);
        float rotation_offset = 0;
        if (mirror_x_ && mirror_y_)
            rotation_offset = 180;
        else if (mirror_x_)
            spr.setTextureRect(sf::IntRect(texture.getSize().x, 0, -static_cast<int>(texture.getSize().x), texture.getSize().y));
        else if (mirror_y_)
            spr.setTextureRect(sf::IntRect(0, texture.getSize().y, texture.getSize().x, -static_cast<int>(texture.getSize().y)));
        spr.setOrigin(0.5f * texture.getSize().x, 0.5f * texture.getSize().y);
        spr.setPosition(get_position().x, get_position().y);
        spr.setRotation(get_rotation()+rotation_offset);
        spr.setScale(get_size().x / static_cast<float>(texture.getSize().x),
                     get_size().y / static_cast<float>(texture.getSize().y));
        target.draw(spr);
    }

    bool ImageObject::get_mirror_x() const
    {
        return mirror_x_;
    }

    void ImageObject::set_mirror_x(bool b)
    {
        mirror_x_ = b;
    }

    bool ImageObject::get_mirror_y() const
    {
        return mirror_y_;
    }

    void ImageObject::set_mirror_y(bool b)
    {
        mirror_y_ = b;
    }
    void ImageObject::set_texture(std::shared_ptr<sf::Texture> const & texture)
    {
        texture_ = texture;
    }
}
