#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "Frequency.h"

#include <string>
#include <rapidjson/document.h>
#include <glog/logging.h>
#include <map>
#include <cstdio>
#include <iostream>
#include <ctime>
#include <openssl/md5.h>
#include <sstream>
#include <cpr/cpr.h>
#include <iostream>
#include <codecvt>
#include <windows.h>
#include <locale>
#include <cctype>
#include <iomanip>
#include <cstring>
#include <memory>

CL_BEGIN

// app_id: read form json file
// time_stamp: generate before GET
// nonce_str: set to WordsFrequency
// sign: generate before GET
// text: arg

class WordsFrequency
{
public:
    using Params = std::map<std::string, std::string>;
    using Frequency = cl::Frequency;
public:
    // json file: app_id and app_key, like:
    /*
{
    "app_id" : "7173982713",
    "app_key" : "JKJkjHhjhJHFf"
}
    */
    // see https://ai.qq.com/doc/nlpbase.shtml
    // and https://ai.qq.com/doc/auth.shtml
    WordsFrequency(const std::string &config_path);
    WordsFrequency() = delete;
    Frequency text(const std::string& text);
    Frequency sentence(const std::string& sentence);

private:
    Frequency frequency_from_json(rapidjson::Document& document);
private:
    std::string _app_id;
    std::string _app_key;
    std::string _nonce_str = "WordsFrequency";
    const int _retry_times = 10;
    std::vector<std::string> _ignore_words = { "，","。","？","“","”",",",".","：", "你","我", "他", "它","她","的", "了" };
};

CL_END