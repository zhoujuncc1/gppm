/*
 * bltl_parser.cpp
 *
 *  Created on: 1 Nov 2016
 *      Author: zhoujun
 */
#include <sstream>
#include "bltl_parser.h"

std::map<std::string, Prd*> parse_prd(std::string str) {
	std::map<std::string, Prd*> map;
	std::stringstream ss;
	ss.str(str);
	std::string item;
	while (std::getline(ss, item)) {
		size_t begin = 0;
		size_t found = item.find(":");
		std::string k = item.substr(0, found);

		begin = found + 1;
		found = item.find(":", begin);
		std::string v = item.substr(begin, found);

		begin = found + 1;
		found = item.find(":", begin);
		std::string l = item.substr(begin, found);
		std::string r = item.substr(found+1);
		map[k]=new Prd(k,v,l,r);
	}
}
