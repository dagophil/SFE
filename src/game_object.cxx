#include "SFE/game_object.hxx"

namespace sfe
{
    GameObject::GameObject()
        :
        position_({ 0, 0 }),
        size_({ 0, 0 }),
        rotation_(0)
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

    float GameObject::get_rotation() const
    {
        return rotation_;
    }
    
    void GameObject::set_rotation(float rotation)
    {
        rotation_ = rotation;
    }
}
