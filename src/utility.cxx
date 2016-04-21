#include <boost/filesystem.hpp>

#include "SFE/utility.hxx"

namespace sfe
{

    std::string current_path()
    {
        auto p = boost::filesystem::current_path();
        return p.generic_string();
    }

} // namespace sfe
