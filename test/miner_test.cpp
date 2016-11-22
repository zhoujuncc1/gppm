#include<gtest/gtest.h>
#include<ParameterSet.h>
#include <Bltl/bltl_parser.h>
#include<miner_utils.h>
class MinerTest : public testing::Test{
public:
    Bltl* bltl;
    std::map<std::string, Prd*> map;
    ParameterSet *params;
    virtual void SetUp(){
        std::string bltl_input = "p1 & F[k1]G[k2]p2;";
        std::string prd_input = "p1:26::\np2:26::";
        map = parse_prd(prd_input);

        bltl= parse_bltl(bltl_input);

        link_prd(bltl, map);
        params = new ParameterSet(bltl, map);


    }
};

TEST_F(MinerTest, BuildParameterSet){
    ASSERT_NE(params->all_set["p1.left"],nullptr);
    ASSERT_FALSE(params->all_set["p1.left"]->isfix);
    ASSERT_FALSE(params->all_set["p1.right"]->isfix);
    ASSERT_FALSE(params->all_set["k1"]->isfix);
    ASSERT_FALSE(params->all_set["k2"]->isfix);

}

//Note: Need to check with actual trajectories
TEST_F(MinerTest, InitPrdRange){
    params->init_prd_range();
    printf("%s: %lf, %lf\n", "p1.left", params->all_set["p1.left"]->range.first, params->all_set["p1.left"]->range.second);
    printf("%s: %lf, %lf\n", "p2.left", params->all_set["p2.left"]->range.first, params->all_set["p2.left"]->range.second);
}

TEST_F(MinerTest, InitTimeRange){
params->init_time_range();
ASSERT_NE(params->unknown_time_set.size(), 0);
ASSERT_EQ(((TimeVariable*)params->all_set["k1"])->range.second, 62);
ASSERT_EQ(((TimeVariable*)params->all_set["k1"])->range.first, 1);
ASSERT_EQ(((TimeVariable*)params->all_set["k2"])->range.first, 1);
ASSERT_EQ(((TimeVariable*)params->all_set["k2"])->range.second, 62);


}


TEST_F(MinerTest, BuildTree){
    params->parse_constraint_tree("p1.left<p2.right\np2.left>p1.right");
    ASSERT_EQ(params->tree_roots.size(),2);
    ASSERT_EQ(params->tree_roots[0], params->all_set["p1.left"]);
    ASSERT_EQ(params->tree_roots[1], params->all_set["p1.right"]);
    ASSERT_EQ(params->all_set["p1.left"]->children[0],params->all_set["p2.right"]);

}

TEST_F(MinerTest, ParseWeight){
params->parse_weight("p1:0.2\np2:0.2\nk1:0.3\nk2:0.2");
ASSERT_EQ(params->weights["p1"],0.2);
ASSERT_EQ(params->weights["k1"],0.3);

}

TEST(ParseConstraints, ParseConstraints){
    std::string input("p1.left<p2.right\np2.left>p1.right");
    vector<pair<std::string, std::string> > constraints = parse_constraint(input);
    ASSERT_STREQ(constraints[0].first.c_str(), "p1.left");
    ASSERT_STREQ(constraints[0].second.c_str(), "p2.right");
    ASSERT_STREQ(constraints[1].first.c_str(), "p1.right");
    ASSERT_STREQ(constraints[1].second.c_str(), "p2.left");
}