#include "WordsFrequency.h"



CL_BEGIN

namespace internal
{

	unsigned char ToHex(unsigned char x)
	{
		return  x > 9 ? x + 55 : x + 48;
	}

	unsigned char FromHex(unsigned char x)
	{
		unsigned char y;
		if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
		else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
		else if (x >= '0' && x <= '9') y = x - '0';
		else assert(0);
		return y;
	}

	std::string url_encode(const std::wstring& str)
	{
		std::string strTemp = "";
		size_t length = str.length();
		for (size_t i = 0; i < length; i++)
		{
			if (isalnum((unsigned char)str[i]) ||
				(str[i] == '-') ||
				(str[i] == '_') ||
				(str[i] == '.') ||
				(str[i] == '~'))
				strTemp += str[i];
			else if (str[i] == ' ')
				strTemp += "+";
			else
			{
				strTemp += '%';
				strTemp += ToHex((unsigned char)str[i] >> 4);
				strTemp += ToHex((unsigned char)str[i] % 16);
			}
		}
		return strTemp;
	}
}

WordsFrequency::WordsFrequency(const std::string &config_path)
{
	FILE *fp = std::fopen(config_path.c_str(), "r");
	if (fp == nullptr)
		LOG(FATAL) << "couldn't find config file:" << config_path;
	std::fseek(fp, 0, SEEK_END);
	size_t filesize = (size_t)std::ftell(fp);
	std::fseek(fp, 0, SEEK_SET);
	char *buffer = new char[filesize + 1];
	size_t len = std::fread(buffer, 1, filesize, fp);
	buffer[len] = '\0';
	LOG(INFO) << "reading config file:" << config_path << ":" << buffer;
	rapidjson::Document doc;
	doc.Parse(buffer);

	rapidjson::Value &ai = doc["app_id"];
	if (ai.GetStringLength())
	{
		_app_id = ai.GetString();
		LOG(INFO) << "app_id:" << _app_id;
	}
	else
	{
		LOG(ERROR) << "couldn't find app id";
	}
	rapidjson::Value &ak = doc["app_key"];
	if (ai.GetStringLength())
	{
		_app_key = ak.GetString();
		LOG(INFO) << "app_key:" << _app_key;
	}
	else
	{
		LOG(ERROR) << "couldn't find app key";
	}
	std::fclose(fp);
	delete[] buffer;
}

unsigned char md5[17] = { 0 };
char smd5[33] = { 0 };

std::string WordsFrequency::generate_sign(Params& params)
{
	std::memset(md5, 0, sizeof(md5));
	std::memset(smd5, 0, sizeof(smd5));

	std::string kv;
	for (auto& p : params)
	{
		if (!p.first.empty())
			kv += (p.first + '=' + p.second + '&');
	}
	kv += ("app_key=" + _app_key);
	MD5(reinterpret_cast<const unsigned char*>(kv.c_str()), kv.size(), md5);
	for (int i = 0; i < 16; i++)
		std::sprintf(smd5 + i * 2, "%02X", static_cast<int>(md5[i]));
	return smd5;
}

WordsFrequency::Frequency WordsFrequency::sentence(const std::string & sentence)
{
	Params params;
	params["app_id"] = _app_id;
	params["nonce_str"] = _nonce_str;
	params["time_stamp"] = std::to_string(std::time(0));
	params["text"] = sentence;
	std::string sign = generate_sign(params);
	params["sign"] = sign;
	LOG(INFO) << "time stamp:" << params["time_stamp"];
	LOG(INFO) << "sign:" << sign;
	cpr::Parameters parameters;
	for (auto& p : params)
	{
		parameters.AddParameter({ p.first, p.second });
	}
	auto resp = cpr::Get(cpr::Url{ "https://api.ai.qq.com/fcgi-bin/nlp/nlp_wordseg" }, parameters);
	LOG(INFO) << "url:\n" << resp.url << "\ntext:\n" << resp.text;
	return Frequency();
}



CL_END