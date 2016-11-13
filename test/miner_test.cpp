#include<gtest/gtest.h>
#include<ParameterSet.h>
#include <Bltl/bltl_parser.h>
#include<Constraint.h>

class MinerTest : public testing::Test{
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

TEST_F(MinerTest, BuildParameterSet){
    ParameterSet *params = new ParameterSet(bltl);
    ASSERT_NE(params->all_set["p1.left"],nullptr);
    ASSERT_FALSE(params->all_set["p1.left"]->isfix);
    ASSERT_FALSE(params->all_set["p1.right"]->isfix);
    ASSERT_FALSE(params->all_set["k1"]->isfix);
    ASSERT_FALSE(params->all_set["k2"]->isfix);

}

TEST(ParseConstraints, ParseConstraints){
    std::string input("p1.left<p2.right\np2.left>p1.right");
    vector<pair<std::string, std::string> > constraints = parse_constraint(input);
    ASSERT_STREQ(constraints[0].first.c_str(), "p1.left");
    ASSERT_STREQ(constraints[0].second.c_str(), "p2.right");
    ASSERT_STREQ(constraints[1].first.c_str(), "p1.right");
    ASSERT_STREQ(constraints[1].second.c_str(), "p2.left");
}