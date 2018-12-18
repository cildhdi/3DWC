#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <cctype>

namespace cl
{
namespace utility
{

std::string to_lower(const std::string& s);

std::string url_encode(const std::string& value);

}
}