#ifndef SFE_STD_HXX
#define SFE_STD_HXX

#if defined(_WIN32)
    #define SFE_SYSTEM_WINDOWS
#endif

#if !defined(SFE_STATIC)

    #if defined(SFE_SYSTEM_WINDOWS)
        #define SFE_API_EXPORT __declspec(dllexport)
        #define SFE_API_IMPORT __declspec(dllimport)
        #ifdef _MSC_VER
            #pragma warning(disable: 4251)
        #endif
    #else
        #if __GNUC__ >= 4
            #define SFE_API_EXPORT __attribute__ ((__visibility__ ("default")))
            #define SFE_API_IMPORT __attribute__ ((__visibility__ ("default")))
        #else
            #define SFE_API_EXPORT
            #define SFE_API_IMPORT
        #endif
    #endif

#else

    #define SFE_API_EXPORT
    #define SFE_API_IMPORT

#endif

#if defined(sfe_EXPORTS)
    #define SFE_API SFE_API_EXPORT
#else
    #define SFE_API SFE_API_IMPORT
#endif

#include <stdexcept>

namespace sfe
{
#pragma warning(push)
#pragma warning(disable: 4275)
    class SFE_API ExceptionBase : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };
#pragma warning(pop)
}

#define DECLARE_EXCEPTION(name) \
class SFE_API name : public sfe::ExceptionBase \
{ public: using sfe::ExceptionBase::ExceptionBase; };

#endif
