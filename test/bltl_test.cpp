/*
 * prd_test.cpp
 *
 *  Created on: 1 Nov 2016
 *      Author: zhoujun
 */

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <Bltl/bltl_parser.h>
#include <Bltl/Parameter.h>
#include <Bltl/Prd.h>
#include <Bltl/Bltl.h>
#include <Bltl/BltlChecker.h>

#include <gtest/gtest.h>

class BltlTest : public testing::Test{
public:
	Bltl* bltl;
	std::map<std::string, Prd*> map;
    virtual void SetUp(){
        std::string bltl_input = "p1 & F[k1]G[k2]p2;";
        std::string prd_input = "p1:1::\np2:1::";
        map = parse_prd(prd_input);

        bltl= parse_bltl(bltl_input);

        link_prd(bltl, map);
    }
};

TEST_F(BltlTest, BltlParser){
    ASSERT_EQ(bltl->to_string(), "(p1)&(F(G(p2)))");
    ASSERT_NE(bltl->getChild1()->getPrd(),nullptr);
    ASSERT_EQ(bltl->getChild1()->getPrd(), map["p1"]);
    ASSERT_STREQ(bltl->getChild1()->getTime()->name.c_str(), "k1");

    bltl= parse_bltl("F[k1=10]G[10]p1;");
    ASSERT_STREQ(bltl->getTime()->name.c_str(), "k1");
    ASSERT_EQ(bltl->getTime()->value, 10);
    ASSERT_EQ(bltl->getChild1()->getTime()->value, 10);



}


TEST_F(BltlTest, PrdAllUnknown){

	Prd* prd = map["p1"];
	ASSERT_EQ(prd->varId, 1);
	ASSERT_FALSE(prd->left->isfix);
    ASSERT_STREQ(prd->left->name.c_str(), "p1.left");
	ASSERT_FALSE(prd->right->isfix);
    ASSERT_STREQ(prd->right->name.c_str(), "p1.right");
}

TEST_F(BltlTest, LinkPrd){

Prd* prd = map["p1"];
ASSERT_EQ(prd->varId, 1);
ASSERT_FALSE(prd->left->isfix);
ASSERT_FALSE(prd->right->isfix);
}

TEST_F(BltlTest, WeightSign){
    ASSERT_EQ(bltl->getTime()->weight_sign, 1.0);
    ASSERT_EQ(bltl->getChild1()->getTime()->weight_sign, -1.0);

}


TEST(BltlCheckerTest, updateValue){
    std::string bltl_input = "p1 & F[10]G[10]p2;";
    std::string prd_input = "p1:1:0:2\np2:1:0:2";
    std::map<std::string, Prd*>map = parse_prd(prd_input);

    Bltl* bltl= parse_bltl(bltl_input);

    link_prd(bltl, map);
    BltlChecker checker(bltl);
    vector<double> values;
    values.push_back(1);
    values.push_back(1);
    checker.update(values, 0);

}



