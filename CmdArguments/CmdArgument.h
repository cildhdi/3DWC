#pragma once

#include <utility/utility.h>

#include <string>
#include <cctype>
#include <cstdlib>

#define TO_NUMERIC(type, func) \
template<> \
inline type CmdArgument::to<type>() const \
{\
    if (_arg_value.empty())\
        return 0;\
    else\
        return std:: func (_arg_value.c_str());\
}

namespace cl
{

class CmdArgument
{
public:
    using ArgumentName = std::string;
    using ArgumentValue = std::string;
private:
    ArgumentName _arg_name;
    ArgumentValue _arg_value;
    bool _has_value;
public:
    ArgumentName name() const;
    ArgumentValue value() const;
    bool has_value() const;
    template<typename Value>
    Value to() const;
public:
    CmdArgument(const std::string& arg, char seperate = '=');
};


template<typename Value>
Value CmdArgument::to() const
{
    static_assert(false, "error type");
    return Value();
}

template<>
inline bool CmdArgument::to<bool>() const
{
    return utility::to_lower(_arg_value) == "true";
}

template<>
inline std::string CmdArgument::to<std::string>() const
{
    return _arg_value;
}

TO_NUMERIC(int, atoi);
TO_NUMERIC(long, atol);
TO_NUMERIC(double, atof);

#undef TO_NUMERIC

}

