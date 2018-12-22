#include "CmdArguments.h"

namespace cl
{


CmdArguments::CmdArguments(int argc, char ** argv)
{
    _app_name = argv[0];
    _args.reserve(argc - 1);
    for (std::size_t i = 1; i < argc; i++)
        _args.emplace_back(argv[i]);
}

bool CmdArguments::exist(const std::string & arg_name) const
{
    return std::find_if(_args.begin(), _args.end(), [&](const cl::CmdArgument& arg) 
    {
        return arg.name() == arg_name;
    }) == _args.end();
}

std::string CmdArguments::value(const std::string & arg_name) const
{
    auto it = std::find_if(_args.begin(), _args.end(), [&](const cl::CmdArgument& arg)
    {
        return arg.name() == arg_name;
    });
    if (it == _args.end())
        throw std::exception("no such argument");
    else
        return it->value();
}

CmdArguments::Arguments::iterator CmdArguments::begin()
{
    return _args.begin();
}

CmdArguments::Arguments::iterator CmdArguments::end()
{
    return _args.end();
}

}