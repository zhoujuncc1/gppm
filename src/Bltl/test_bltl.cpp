/*
 * test_bltl.cpp
 *
 *  Created on: 28 Oct 2016
 *      Author: zhoujun
 */
#include <iostream>

#include "bltl_parser.h"

int main(int argc, char** argv){
	std::string input = "F(p1 U p2)";
	Bltl* bltl = bltl_parse(input);
	std::cout << bltl->to_string();
	return 0;
}


