#ifndef OPENMW_COMPONENTS_FALLBACK_VALIDATE_H
#define OPENMW_COMPONENTS_FALLBACK_VALIDATE_H

#include <map>
#include <string>
#include <vector>

namespace boost
{
    class any;
}

namespace Fallback
{

    bool isAllowedIntFallbackKey(std::string_view key);
    bool isAllowedFloatFallbackKey(std::string_view key);
    bool isAllowedNonNumericFallbackKey(std::string_view key);
    bool isAllowedUnusedFallbackKey(std::string_view key); // imported from Morrowind.ini but unused

    struct FallbackMap
    {
        std::map<std::string, std::string> mMap;
    };

    // Parses and validates a fallback map from boost program_options.
    // Note: for boost to pick up the validate function, you need to pull in the namespace e.g.
    // by using namespace Fallback;
    void validate(boost::any& v, std::vector<std::string> const& tokens, FallbackMap*, int);
}

#endif
