#pragma once

#include <App.h>

#include <vector>
#include <algorithm>
#include <string>
#undef max
#undef min
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

CL_BEGIN

class Frequency
{
public:
    using PSL = std::pair<std::string, long>;
public:
    Frequency();
    void sort();
    void remove(const std::vector<std::string>& filter);
    std::string to_json() const;

public:
    long& operator[](const std::string& word);
    Frequency operator+(const Frequency& rhs);
    Frequency operator+=(const Frequency& rhs);
public:
    std::vector<PSL> frequency;
};

CL_END