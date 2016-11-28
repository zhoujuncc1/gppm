//
// Created by Zhou Jun on 17/11/16.
//

#ifndef GPPM_MINING_H
#define GPPM_MINING_H

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include "Bltl/Bltl.h"
#include "Bltl/bltl_parser.h"
#include "miner_utils.h"
#include "ParameterSet.h"
#include "State.h"
#include "ModelChecking.h"
#include "Model.h"

namespace pt = boost::property_tree;


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
        params->parse_constraint_tree(constraint_input);
        if(!weight_input.empty())
            params->parse_weight(weight_input);
        for(int i = 0; i < MAX_SIM; i++)
            trajectories.push_back(Model::simulate(1.0));
    }

    Miner(std::string filename){
        pt::ptree tree;
        pt::read_xml(filename, tree);
        std::string bltl_input = tree.get<std::string>("input.bltl");
        std::string prd_input = tree.get<std::string>("input.prd");
        std::string constraint_input = tree.get("input.constraint", "");
        std::string weight_input = tree.get<std::string>("input.weight");
        boost::erase_all(bltl_input, " ");
        boost::erase_all(prd_input, " ");
        boost::erase_all(constraint_input, " ");
        boost::erase_all(weight_input, " ");

        prds = parse_prd(prd_input);
        bltl = parse_bltl(bltl_input);
        link_prd(bltl, prds);
        params = new ParameterSet(bltl, prds);
        params->init_prd_range();
        params->init_time_range();
        params->parse_constraint_tree(constraint_input);
        if(!weight_input.empty())
            params->parse_weight(weight_input);
        for(int i = 0; i < MAX_SIM; i++)
            trajectories.push_back(Model::simulate(1.0));
    }


    void mine(){
        state = new State(params);
        state->prd_values = generate_prd(params->tree_roots);
        state->time_values = generate_time(params->unknown_time_set);
        state->trajectories = &trajectories;
        do_mine(state);

    }
    State *state;
    std::map<std::string, Prd*> prds;
    Bltl* bltl;
    ParameterSet* params;
    std::vector<Trajectory> trajectories;


};

#endif //GPPM_MINING_H
