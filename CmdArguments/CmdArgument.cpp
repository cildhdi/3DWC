#include "CmdArgument.h"

namespace cl
{

CmdArgument::ArgumentName CmdArgument::name() const
{
    return _arg_name;
}

CmdArgument::ArgumentValue CmdArgument::value() const
{
    return _arg_value;
}

bool CmdArgument::has_value() const
{
    return _has_value;
}

CmdArgument::CmdArgument(const std::string& arg, char seperate): _has_value(true)
{
    const auto seperate_pos = arg.find_first_of(seperate);
    if (seperate_pos == arg.npos)
    {
        _arg_name = arg.substr(1);
        _has_value = false;
        _arg_value = "";
        return;
    }
    _arg_name = arg.substr(1, seperate_pos - 1);
    if (arg.size() > seperate_pos + 1 && arg.at(seperate_pos + 1) == '"' && arg.back() == '"')
        _arg_value = arg.substr(seperate_pos + 2, arg.size() - seperate_pos - 3);
    else
        _arg_value = arg.substr(seperate_pos + 1);
}


}

