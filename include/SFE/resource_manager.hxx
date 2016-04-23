#ifndef SFE_RESOURCE_MANAGER_HXX
#define SFE_RESOURCE_MANAGER_HXX

#include <string>
#include <map>
#include <iostream>

#include <SFML/Graphics.hpp>

namespace sfe
{
    ////////////////////////////////////////////////////////////
    /// The resource manager ensures that files such as images,
    /// sounds, etc. are loaded only once.
    ////////////////////////////////////////////////////////////
    class ResourceManager
    {
    public:

        ////////////////////////////////////////////////////////////
        /// The global resource manager.
        ////////////////////////////////////////////////////////////
        static ResourceManager & global();

        ////////////////////////////////////////////////////////////
        /// Default constructor.
        ////////////////////////////////////////////////////////////
        ResourceManager() = default;

        ////////////////////////////////////////////////////////////
        /// If a texture with the given name is already stored, it
        /// is returned. Otherwise, the texture will be loaded from
        /// the file with the given nam.
        ////////////////////////////////////////////////////////////
        sf::Texture & get_texture(std::string const & name);

    private:
        
        ////////////////////////////////////////////////////////////
        /// The texture storage.
        ////////////////////////////////////////////////////////////
        std::map<std::string, sf::Texture> textures_;

    }; // class ResourceManager

} // namespace sfe

#endif
