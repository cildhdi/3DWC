#include "WordsFrequency.h"


CL_BEGIN

namespace utility
{
	std::string url_encode(const std::string& value)
	{
		std::ostringstream escaped;
		escaped.fill('0');
		escaped << std::setiosflags(std::ios::uppercase) << std::hex;
		for (auto i = value.cbegin(), n = value.cend(); i != n; ++i)
		{
			char c = (*i);
			if (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~')
			{
				escaped << c;
				continue;
			}
			else if (c == ' ')
			{
				escaped << '+';
				continue;
			}
			escaped << '%' << std::setw(2) << std::int32_t((unsigned char)c);
		}
		return escaped.str();
	}


}

WordsFrequency::WordsFrequency(const std::string &config_path)
{
	FILE *fp = std::fopen(config_path.c_str(), "r");
	if (fp == nullptr)
		LOG(FATAL) << "couldn't find config file:" << config_path;
	std::fseek(fp, 0, SEEK_END);
	size_t filesize = static_cast<size_t>(std::ftell(fp));
	std::fseek(fp, 0, SEEK_SET);
	char *buffer = new char[filesize + 1];
	size_t len = std::fread(buffer, 1, filesize, fp);
	buffer[len] = '\0';
	rapidjson::Document doc;
	doc.Parse(buffer);

	rapidjson::Value &ai = doc["app_id"];
	if (ai.GetStringLength())
	{
		_app_id = ai.GetString();
	}
	else
	{
		LOG(ERROR) << "couldn't find app id";
	}
	rapidjson::Value &ak = doc["app_key"];
	if (ai.GetStringLength())
	{
		_app_key = ak.GetString();
	}
	else
	{
		LOG(ERROR) << "couldn't find app key";
	}
	LOG(INFO) << "config:\napp_id:" << _app_id << "\napp_key:" << _app_key;
	std::fclose(fp);
	delete[] buffer;
}

unsigned char md5[17] = { 0 };
char smd5[33] = { 0 };

WordsFrequency::Frequency WordsFrequency::sentence(const std::string & sentence)
{
	LOG(INFO) << "sentence:" << sentence;
	int retry = _retry_times;
	while (retry--)
	{
		if (retry != _retry_times - 1)
			LOG(INFO) << "retrying...";
		Params params;
		// prepare to genearte sign
		params["app_id"] = _app_id;
		params["nonce_str"] = _nonce_str;
		params["time_stamp"] = std::to_string(std::time(0));
		params["text"] = utility::url_encode(sentence);

		// genearte sign according to https://ai.qq.com/doc/auth.shtml
		std::memset(md5, 0, sizeof(md5));
		std::memset(smd5, 0, sizeof(smd5));
		std::string content;
		for (auto& p : params)
		{
			if (!p.first.empty())
				content += (p.first + '=' + p.second + '&');
		}
		content += ("app_key=" + _app_key);
		MD5(reinterpret_cast<const unsigned char*>(content.c_str()), content.size(), md5);
		for (int i = 0; i < 16; i++)
			std::sprintf(smd5 + i * 2, "%02X", static_cast<int>(md5[i]));
		content += ('&' + std::string("sign=") + smd5);
		cpr::Parameters parameters;
		parameters.content = content;
		cpr::Url url = { "https://api.ai.qq.com/fcgi-bin/nlp/nlp_wordseg" };
		LOG(INFO) << "Getting from " << url;
		auto response = cpr::Get(url, parameters);

		if (response.error.code != cpr::ErrorCode::OK)
		{
			LOG(ERROR) << "cpr error, message:" << response.error.message;
			continue;
		}
		else
		{
			LOG(INFO) << "responsed...";
		}
		rapidjson::Document document;
		document.Parse(response.text.c_str());

		int return_code = document["ret"].GetInt();
		LOG(INFO) << "return code:" << return_code
			<< ", meesage:" << document["msg"].GetString();
		if (return_code != 0)
			continue;

		return frequency_from_json(document);
	}
	LOG(ERROR) << "failed to split text:" << sentence;
	return Frequency();
}

WordsFrequency::Frequency WordsFrequency::frequency_from_json(rapidjson::Document& document)
{
	const rapidjson::Value& mix_tokens = document["data"]["mix_tokens"];
	Frequency frequency;
	for (rapidjson::SizeType i = 0; i < mix_tokens.Size(); i++)
		frequency[mix_tokens[i]["word"].GetString()]++;
	std::ostringstream oss;
	frequency.remove(_ignore_words);
	frequency.sort();
	for (auto& p : frequency.frequency)
		oss << "\nword:  " << std::setw(10) << std::left << p.first << "times:  " << p.second;
	LOG(INFO) << "result in descending order:" << oss.str();
	std::cout << frequency.to_json();
	return frequency;
}

Frequency WordsFrequency::text(const std::string & text)
{
	char* buffer = new char[text.size() + 1];
	std::memset(buffer, 0, text.size() + 1);
	text.copy(buffer, text.size());
	char *token = std::strtok(buffer, "¡£");
	std::vector<std::string> sentences;
	while (token != nullptr)
	{
		sentences.emplace_back(token);
		token = std::strtok(nullptr, "¡£");
	}
	LOG(INFO) << "split :\n" << buffer << "\ninto " << sentences.size() << " sentences.";

	Frequency frequency;
	for (auto& s : sentences)
	{
		frequency += sentence(s);
	}
	frequency.sort();
	std::ostringstream oss;
	for (auto& p : frequency.frequency)
		oss << "\nword:" << p.first << ", times:" << p.second;
	LOG(INFO) << "finish:\n" << oss.str();
	delete[] buffer;
	return frequency;
}

CL_END