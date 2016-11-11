#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>


#include <Bltl/TimeVariable.h>

std::unordered_set<std::string> TimeVariable::pool=std::unordered_set<std::string>();
TimeVariable::TimeVariable() :
		Parameter(get_name()), value(10000000) {
}

TimeVariable::TimeVariable(char* str) {
	std::string newStr(str);
	size_t equal = newStr.find('=');
	if (equal != std::string::npos) {
		name = newStr.substr(1, equal);
		value = std::stoi(newStr.substr(equal + 1, newStr.length() - 1));
		isfix=true;
	} else if ('0' < newStr[1] && newStr[1] < '9') {
		name = get_name();
		value = std::stoi(newStr.substr(1, newStr.length()-1));
		isfix=true;
	} else {
		name = newStr.substr(1, newStr.length()-1);
		isfix=false;
	}

}

std::string TimeVariable::get_name(){
	std::string s;
	while (true) {
		int r = std::rand() % 1000 + 100;
		s = "t" + std::to_string(r);
		if (pool.find(s) == pool.end()) {
			pool.insert(s);
			break;
		}
	}
	return s;
}

