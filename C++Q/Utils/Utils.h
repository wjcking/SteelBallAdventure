#pragma once
#include <algorithm>
template <typename T>
inline std::string convertToString(T val)
{
	std::stringstream stream;
	stream << val;
	return stream.str();
};
template <typename T>
inline std::string	getTypeName(T& t)
{
	std::string s(typeid(t).name());

	//remove the 'class ' part from the front of the string
	if (s.size() > 5)
	{
		s = s.erase(0, 6);
	}

	auto index = s.find_last_of(' ');
	auto data = s.substr(0, index);
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
};
inline unsigned int getOccurrences(const std::string& text, const std::string &dest)  
{
	unsigned int pos, pre = 0, count = 0;
	while ((pos = text.find(dest, pre)) != std::string::npos)
	{
		++count;
		pre = pos + 1;
	}
	return count;
};

inline std::string getSelectedFile(const std::string& file)
{
	CCASSERT(file != "", "[getSelectedFile]文件字符串不能为空");
	auto dot = file.find_last_of('.');
	if (dot < 0)
		return file;
	
	return StringUtils::format("%s_s.png", file.substr(0, dot).c_str());
}
inline std::string getDisabledFile(const std::string& file)
{
	CCASSERT(file != "", "[getDisabledFile]文件字符串不能为空");
	auto dot = file.find_last_of('.');
	if (dot < 0)
		return file;

	return StringUtils::format("%s_d.png", file.substr(0, dot).c_str());
}
 