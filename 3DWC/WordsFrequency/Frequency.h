#pragma once

#define CL_BEGIN namespace cl{
#define CL_END }

#include <vector>
#include <algorithm>

CL_BEGIN

class Frequency
{
public:
	using PSL = std::pair<std::string, long>;
public:
	Frequency() {}
	long& operator[](const std::string& word)
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

	void sort()
	{
		std::sort(frequency.begin(), frequency.end(), [](const PSL& lhs, const PSL& rhs)
		{
			return lhs.second > rhs.second;
		});
	}

	Frequency operator+(const Frequency& rhs)
	{
		Frequency new_frequency = *this;
		new_frequency += rhs;
		return new_frequency;
	}

	Frequency operator+=(const Frequency& rhs)
	{
		for (auto& p : rhs.frequency)
		{
			(*this)[p.first] += p.second;
		}
		return *this;
	}
public:
	std::vector<PSL> frequency;
};

CL_END