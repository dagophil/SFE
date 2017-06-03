#include <SFE/utility.hxx>

#include <boost/filesystem.hpp>

namespace sfe
{

    std::string current_path()
    {
        auto p = boost::filesystem::current_path();
        return p.generic_string();
    }

} // namespace sfe
