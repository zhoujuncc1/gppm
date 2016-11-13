/*
 * prd_test.cpp
 *
 *  Created on: 1 Nov 2016
 *      Author: zhoujun
 */

#include <iostream>
#include <map>
#include <string>

#include <Bltl/bltl_parser.h>
#include <Bltl/Parameter.h>
#include <Bltl/Prd.h>
#include <Bltl/Bltl.h>

#include <gtest/gtest.h>

class BltlTest : public testing::Test{
public:
	Bltl* bltl;
	std::map<std::string, Prd*> map;
    virtual void SetUp(){
        std::string input = std::string("p1:1::");
        map = parse_prd(input);

        bltl= parse_bltl("F[k1]G[k2]p1;");

        link_prd(bltl, map);
    }
};

TEST_F(BltlTest, BltlParser){
    ASSERT_EQ(bltl->to_string(), "F(G(p1))");

}

TEST_F(BltlTest, PrdAllUnknown){

	Prd* prd = map["p1"];
	ASSERT_EQ(prd->varId, 1);
	ASSERT_FALSE(prd->left->isfix);
	ASSERT_FALSE(prd->right->isfix);
}




