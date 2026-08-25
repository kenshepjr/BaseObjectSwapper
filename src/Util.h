#pragma once

namespace regex
{
	inline boost::regex generic{ R"(\((.*?)\))" };                // pos(0,0,100) -> "0,0,100"
	inline boost::regex transform{ R"(\((.*?),(.*?),(.*?)\))" };  // pos(0,0,100) -> 0, 0, 100
	inline boost::regex string{ R"(,\s*(?![^()]*\)))" };          // pos(0, 0, 100), rot(0, 0, 100) -> "pos(0, 0, 100)","rot(0, 0, 100)"
}

namespace util
{
	std::vector<std::string> split_with_regex(const std::string& a_str, const boost::regex& a_regex);

	RE::FormID  GetFormID(const std::string& a_str);
	FormIDOrSet GetSwapFormID(const std::string& a_str);
	FormIDOrderedSet GetFormIDOrderedSet(const std::string& a_str);
}
