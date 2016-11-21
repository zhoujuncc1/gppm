//
// Created by Zhou Jun on 20/11/16.
//

#include <iostream>
#include "Mining.h"

int main(int argc, char** argv){
    std::string bltl_input = "p1 & F[k1]G[k2]p2;";
    std::string prd_input = "p1:26::\np2:26::";
    std::string constraint_input = "";
    std::string weight_input = "p1:0.2\np2:0.2\nk1:0.2\nk2:0.2";
    Miner miner(prd_input, bltl_input, constraint_input, weight_input);
    miner.mine();
    std::cout<<miner.state->to_string();

}