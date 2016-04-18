#include <stdexcept>

#include "SFE/resource_manager.hxx"

namespace sfe
{

    ResourceManager & ResourceManager::global()
    {
        static ResourceManager instance;
        return instance;
    }
    
    sf::Texture & ResourceManager::get_texture(std::string const & name)
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
            auto p = textures_.insert({ name, {} });
            it = p.first;
            if (!it->second.loadFromFile(name))
            {
                textures_.erase(it);
                throw std::runtime_error("Could not load image " + name);
            }
            else
            {
                return it->second;
            }
        }
    }

} // namespace sfe
