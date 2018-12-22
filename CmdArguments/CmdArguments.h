#pragma once

#include "CmdArgument.h"
#include <string>
#include <vector>
#include <algorithm>
namespace cl
{

class CmdArguments
{
public:
    using Arguments = std::vector<cl::CmdArgument>;
private:
    std::string _app_name;
    Arguments _args;
public:
    CmdArguments(int argc, char ** argv);
    bool exist(const std::string& arg_name) const;
    std::string value(const std::string& arg_name) const;
    Arguments::iterator begin();
    Arguments::iterator end();
};

inline cl::CmdArguments::Arguments::iterator begin(CmdArguments& args)
{
    return args.begin();
}

inline cl::CmdArguments::Arguments::iterator end(CmdArguments& args)
{
    return args.end();
}

}