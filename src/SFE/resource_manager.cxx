#include <SFE/resource_manager.hxx>

#include <stdexcept>

namespace sfe
{
    
    std::shared_ptr<sf::Texture> ResourceManager::get_texture(std::string const & name)
    {
        auto it = textures_.find(name);
        if (it != textures_.end())
        {
            // The texture is already loaded, so just return it.
            return it->second;
        }
        else
        {
            // Load the texture.
            auto texture = std::make_shared<sf::Texture>();
            if (!texture->loadFromFile(name))
            {
                throw ResourceException("Could not load image " + name);
            }
            texture->setSmooth(true);
            textures_.emplace(name, texture);
            return texture;
        }
    }

} // namespace sfe
