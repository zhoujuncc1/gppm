/*
 * prd_test.cpp
 *
 *  Created on: 1 Nov 2016
 *      Author: zhoujun
 */

#include <iostream>
#include <map>
#include <string>

#include "../../include/Bltl/bltl_parser.h"
#include "../../include/Bltl/Parameter.h"
#include "../../include/Bltl/Prd.h"
#include "../gtest/include/gtest/gtest.h"

TEST(PrdParserTest, AllUnknown){
	std::string input = std::string("p1:x1::");
	std::map<std::string, Prd*> map = parse_prd(input);
	Prd* prd = map["p1"];
	ASSERT_STREQ(prd->variable.c_str(), "x1");
	ASSERT_FALSE(prd->left->isfix);
	ASSERT_FALSE(prd->right->isfix);
}

