#ifndef SFE_RESOURCE_MANAGER_HXX
#define SFE_RESOURCE_MANAGER_HXX

#include <SFE/sfestd.hxx>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <map>
#include <string>

namespace sfe
{
    ////////////////////////////////////////////////////////////
    /// The resource manager ensures that files such as images,
    /// sounds, etc. are loaded only once.
    ////////////////////////////////////////////////////////////
    class SFE_API ResourceManager
    {
    public:

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

    ////////////////////////////////////////////////////////////
    /// Exception class for all resource exceptions.
    ////////////////////////////////////////////////////////////
    DECLARE_EXCEPTION(ResourceException);

} // namespace sfe

#endif
