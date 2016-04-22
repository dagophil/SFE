#include "SFE/game_object.hxx"
#include "SFE/resource_manager.hxx"

namespace sfe
{
    GameObject::GameObject()
        :
        position_({ 0, 0 }),
        size_({ 1, 1 }),
        rotation_(0),
        z_index_(0)
    {}

    void GameObject::update(sf::Time elapsed_time)
    {}

    void GameObject::render(sf::RenderTarget & target) const
    {}

    sf::Vector2f const & GameObject::get_position() const
    {
        return position_;
    }

    void GameObject::set_position(sf::Vector2f const & position)
    {
        position_ = position;
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

    ImageObject::ImageObject(std::string const & filename)
        :
        texture_(ResourceManager::global().get_texture(filename))
    {}

    void ImageObject::render(sf::RenderTarget & target) const
    {
        sf::Sprite spr(texture_);
        spr.setPosition(get_position());
        spr.setRotation(get_rotation());
        spr.setScale(get_size().x / static_cast<float>(texture_.getSize().x),
                     get_size().y / static_cast<float>(texture_.getSize().y));
        target.draw(spr);
    }
}
