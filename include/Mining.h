//
// Created by Zhou Jun on 17/11/16.
//

#ifndef GPPM_MINING_H
#define GPPM_MINING_H

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "Bltl/Bltl.h"
#include "Bltl/bltl_parser.h"
#include "miner_utils.h"
#include "ParameterSet.h"
#include "State.h"
#include "ModelChecking.h"
#include "Model.h"
#include "kmeans.h"
namespace pt = boost::property_tree;


void do_mine(State* state, pt::ptree config);
class Miner{
public:

    Miner(pt::ptree in) : input(in){
        std::string bltl_input = input.get<std::string>("input.bltl");
        std::vector<std::string> prd_inputs;
        BOOST_FOREACH(pt::ptree::value_type &v, input.get_child("input.prds")) {
            // The data function is used to access the data stored in a node.
            prd_inputs.push_back(v.second.data());
        }
        std::vector<std::string> constraint_inputs;
        BOOST_FOREACH(pt::ptree::value_type &v, input.get_child("input.constraints")) {
            // The data function is used to access the data stored in a node.
            constraint_inputs.push_back(v.second.data());
        }
        std::vector<std::string> weight_inputs;
        BOOST_FOREACH(pt::ptree::value_type &v, input.get_child("input.weights")) {
            // The data function is used to access the data stored in a node.
            weight_inputs.push_back(v.second.data());
        }


        prds = parse_prd(prd_inputs);
        bltl = parse_bltl(bltl_input);
        link_prd(bltl, prds);
        params = new ParameterSet(bltl, prds);
        params->init_prd_range();
        params->init_time_range();
        params->parse_constraint_tree(constraint_inputs);
        if(!weight_inputs.empty())
            params->parse_weight(weight_inputs);
        for(int i = 0; i < MAX_SIM; i++)
            trajectories.push_back(Model::simulate(1.0));
    }


    virtual void mine();
    pt::ptree input;
    State *state;
    std::map<std::string, Prd*> prds;
    Bltl* bltl;
    ParameterSet* params;
    std::vector<Trajectory> trajectories;


};

class SingleMiner : Miner{
public:
    SingleMiner(pt::ptree input) : Miner(input){}
    void mine(){
        state = new State(params);
        state->prd_values = generate_prd(params->tree_roots);
        state->time_values = generate_time(params->unknown_time_set);
        state->trajectories = &trajectories;
        do_mine(state, input.get_child("input.config"));
    }
};

class ClusterMiner : Miner{
public:
    ClusterMiner(pt::ptree input) : Miner(input){
        pt::ptree cluster_conf = input.get_child("input.cluster");
        input_t = cluster_conf.get<int>("input_t");
        cluster_t = cluster_conf.get<int>("cluster_t");
    }
    void mine(){
        state = new State(params);
        state->prd_values = generate_prd(params->tree_roots);
        state->time_values = generate_time(params->unknown_time_set);
        state->trajectories = &trajectories;
        for(i = 0; i < input_t; i++) {
            state->prd_values = generate_prd(state->paramset->tree_roots);
            state->time_values = generate_time(state->paramset->unknown_time_set);

        }
    }
    int input_t;
    int cluster_t;
    vector<Point> points;

};

Miner buildMiner(std::string filename){
    pt::ptree input;
    pt::read_xml(filename, input, pt::xml_parser::trim_whitespace);\
    std::string miner_type = input.get<std::string>("input.miner");
    if (miner_type.compare("single"))
        return SingleMiner(input);
    else if(miner_type.compare("cluster"))
        return ClusterMiner(input);

}

#endif //GPPM_MINING_H
