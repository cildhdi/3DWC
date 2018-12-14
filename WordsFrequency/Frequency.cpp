#include "Frequency.h"

CL_BEGIN

Frequency::Frequency() {}

void Frequency::sort()
{
    std::sort(frequency.begin(), frequency.end(), [](const PSL& lhs, const PSL& rhs)
    {
        return lhs.second > rhs.second;
    });
}

void Frequency::remove(const std::vector<std::string>& filter)
{
    frequency.erase(std::remove_if(frequency.begin(), frequency.end(), [&](const Frequency::PSL& p)
    {
        return std::find(filter.begin(), filter.end(), p.first) != filter.end();
    }), frequency.end());
}

std::string Frequency::to_json() const
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value array;
    array.SetArray();
    for (auto& p : frequency)
    {
        rapidjson::Value element;
        element.SetObject();
        element.AddMember("word", rapidjson::StringRef(p.first.c_str()), document.GetAllocator());
        element.AddMember("times", p.second, document.GetAllocator());
        array.PushBack(element, document.GetAllocator());
    }
    document.AddMember("data", array, document.GetAllocator());
    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    document.Accept(writer);
    return sb.GetString();
}

long& Frequency::operator[](const std::string& word)
{
    auto it = std::find_if(frequency.begin(), frequency.end(), [&](const PSL& p)
    {
        return p.first == word;
    });
    if (it == frequency.end())
    {
        frequency.emplace_back(word, 0);
        return frequency.back().second;
    }
    else
    {
        return it->second;
    }
}

Frequency Frequency::operator+(const Frequency& rhs)
{
    Frequency new_frequency = *this;
    new_frequency += rhs;
    return new_frequency;
}

Frequency Frequency::operator+=(const Frequency& rhs)
{
    for (auto& p : rhs.frequency)
    {
        (*this)[p.first] += p.second;
    }
    return *this;
}

CL_END