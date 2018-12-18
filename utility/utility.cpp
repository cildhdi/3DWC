#include "utility.h"

namespace cl
{
namespace utility
{

std::string to_lower(const std::string & s)
{
    std::string lower;
    lower.reserve(s.size());
    for (const auto& c : s)
        lower.push_back(std::tolower(c));
    return lower;
}

std::string url_encode(const std::string& value)
{
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::setiosflags(std::ios::uppercase) << std::hex;
    for (auto i = value.cbegin(), n = value.cend(); i != n; ++i)
    {
        char c = (*i);
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            escaped << c;
            continue;
        }
        else if (c == ' ')
        {
            escaped << '+';
            continue;
        }
        escaped << '%' << std::setw(2) << std::int32_t((unsigned char)c);
    }
    return escaped.str();
}

}
}