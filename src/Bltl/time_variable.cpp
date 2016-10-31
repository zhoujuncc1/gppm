#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>


#include "time_variable.h"

std::unordered_set<std::string> TimeVariable::pool=std::unordered_set<std::string>();
TimeVariable::TimeVariable() :
		name(get_name()), value(10000000) {
}

TimeVariable::TimeVariable(char* str) {
	std::string newStr(str);
	size_t equal = newStr.find('=');
	if (equal != std::string::npos) {
		name = newStr.substr(1, equal);
		value = std::stoi(newStr.substr(equal + 1, newStr.length() - 1));
		isfix=true;
	} else if ('0' < newStr[1] < '9') {
		name = get_name();
		value = std::stoi(newStr.substr(1, newStr.length()-1));
		isfix=true;
	} else {
		name = newStr.substr(1, newStr.length()-1);
		value=-1;
		isfix=false;
	}

}

auto TimeVariable::operator+=(const TimeVariable &rhs) -> TimeVariable & {
	value += rhs.value;
	return *this;
}

auto TimeVariable::get_name() -> std::string {
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

auto operator+(TimeVariable lhs, const TimeVariable &rhs) -> TimeVariable {
	lhs += rhs;
	return lhs;
}

auto to_string(const TimeVariable &v) -> std::string {
	std::stringstream ss;
	ss << v.name << "=" << v.value;
	return ss.str();
}

std::ostream & operator<<(std::ostream &os, const TimeVariable &v) {
	os << to_string(v);
	return os;
}

// int main()
// {
//     TimeVariable v("k", 5), w(20), u(2);
//     std::cout << v << std::endl << w << std::endl << u << std::endl;
//     w += v;
//     std::cout << w << std::endl << v + w << std::endl;
//     return 0;
// }
