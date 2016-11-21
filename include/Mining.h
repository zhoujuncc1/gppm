//
// Created by Zhou Jun on 17/11/16.
//

#ifndef GPPM_MINING_H
#define GPPM_MINING_H

#include <string>
#include "Bltl/Bltl.h"
#include "Bltl/bltl_parser.h"
#include "miner_utils.h"
#include "ParameterSet.h"
#include "State.h"

void do_mine(State* state);

class Miner{
public:
    Miner(std::string prd_input, std::string bltl_input, std::string constraint_input="", std::string weight_input=""){
        prds = parse_prd(prd_input);
        bltl = parse_bltl(bltl_input);
        link_prd(bltl, prds);
        params = new ParameterSet(bltl, prds);
        params->init_prd_range();
        params->init_time_range();
        if(!constraint_input.empty())
            params->parse_constraint_tree(constraint_input);
        if(!weight_input.empty())
            params->parse_weight(weight_input);
    }


    void mine(){
        state = new State(params);
        state->prd_values = generate_prd(params->tree_roots);
        state->time_values = generate_time(params->unknown_time_set);
        do_mine(state);

    }
    State *state;
    std::map<std::string, Prd*> prds;
    Bltl* bltl;
    ParameterSet* params;


};

#endif //GPPM_MINING_H
