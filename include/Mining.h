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

using namespace std;
void do_mine(State* state, pt::ptree config);
class Miner{
public:
    Miner(){}
/*
    Miner(pt::ptree in, string bltl_input, vector<string> prd_inputs, vector<string> constraint_inputs, vector<string> weight_inputs) : input(in){
        

        prds = parse_prd(prd_inputs);
        bltl = parse_bltl(bltl_input);
        link_prd(bltl, prds);
        dt = input.get<double>("input.dt", 1.0);
        end_time = input.get<double>("input.end_time", Model::end_time);
        for(int i = 0; i < MAX_SIM; i++)
            trajectories.push_back(Model::simulate(dt, end_time));
        params = new ParameterSet(bltl, prds, trajectories);
        params->init_prd_range();
        params->init_time_range();
        params->parse_constraint_tree(constraint_inputs);
        if(!weight_inputs.empty())
            params->parse_weight(weight_inputs);
        bltlChecker= new RecursiveBltlChecker(bltl, prds, trajectories[0]);
        //bltlChecker= new TreeBltlChecker(bltl);
    }
*/
    void mine(){
        state = new State(params);
        state->prd_values = generate_prd(params->tree_roots, initial_state);
        state->time_values = generate_time(params->unknown_time_set, initial_state);
        
        state->trajectories = &trajectories;
        state->bltlChecker=bltlChecker;
        if(bltl->getOperation()==op_PRD)
            minePrd();
        else
            mineGeneral();
        refine_time();
    }

    void refine_time(){
        for(auto itr=params->unknown_prd_set.begin();itr!=params->unknown_prd_set.end();itr++)
            itr->second->value = state->prd_values[itr->first];
        for(auto itr=params->unknown_time_set.begin();itr!=params->unknown_time_set.end();itr++)
            itr->second->value = state->time_values[itr->first];

        for(auto itr = params->unknown_time_set.begin(); itr != params->unknown_time_set.end(); itr++){
            cout<<itr->first<<" ";
            double loss_v = 0;
            int best = itr->second->value;
            double best_loss = loss(state);
            while(itr->second->value>itr->second->range.first && itr->second->value < itr->second->range.second){
                itr->second->value-=itr->second->weight_sign;
                cout<<itr->second->value<<", ";
                loss_v = loss(state);
                if(loss_v < best_loss){
                    best = itr->second->value;
                    best_loss = loss_v;
                }
                cout<<loss_v << "| ";
            }
            itr->second->value = best;
            loss(state);
            cout << endl<<endl;
            state->time_values[itr->first] = itr->second->value;
        }

    }

    string to_string(){
	    std::stringstream ss;
	    ss<<"{";
	    for(auto itr=params->known_prd_set.begin();itr!=params->known_prd_set.end();itr++)
	        ss<<itr->first<<": "<<itr->second->value<<", ";
        for(auto itr=params->known_time_set.begin();itr!=params->known_time_set.end();itr++)
	        ss<<itr->first<<": "<<itr->second->value<<", ";
        for(auto itr=params->unknown_prd_set.begin();itr!=params->unknown_prd_set.end();itr++)
            ss<<itr->first<<": "<<state->prd_values[itr->first]<<", ";
        for(auto itr=params->unknown_time_set.begin();itr!=params->unknown_time_set.end();itr++)
	        ss<<itr->first<<": "<<state->time_values[itr->first]<<", ";
	    ss<<"}";
	    return ss.str();
    }

    void minePrd(){
        double min,max;
        min=trajectories[0].m_states[0][bltl->getPrd()->varId];
        max=trajectories[0].m_states[0][bltl->getPrd()->varId];
        for(auto itr = trajectories.begin(); itr!=trajectories.end(); itr++){
            if (min>itr->m_states[0][bltl->getPrd()->varId])
                min=itr->m_states[0][bltl->getPrd()->varId];
            if (max<itr->m_states[0][bltl->getPrd()->varId])
                max=itr->m_states[0][bltl->getPrd()->varId];
        }
        for(auto itr = state->prd_values.begin(); itr!=state->prd_values.end(); itr++)
            if(itr->first.find("left")!=string::npos)
                itr->second=min;
            else
                itr->second=max;
    }
    virtual void mineGeneral()=0;


    pt::ptree input;
    State *state;
    map<string, Prd*> prds;
    Bltl* bltl;
    ParameterSet* params;
    vector<Trajectory> trajectories;
    BltlChecker* bltlChecker;
    double dt;
    double end_time;

    map<string, string> initial_state;

};

class SingleMiner : public Miner{
public:
    SingleMiner(){}
    void mineGeneral(){
        do_mine(state, input.get_child("input.config"));
    }
};

class FileMiner : public SingleMiner{
public:
    FileMiner(){}
    FileMiner(pt::ptree in, string bltl_input, vector<string> prd_inputs, vector<string> constraint_inputs, vector<string> weight_inputs){
        string filename = in.get<string>("input.filename");
        FileTrajectoryProvider trajProvider(filename);
        trajectories = trajProvider.getTrajectories(MAX_SIM);

        init_with_trajectories(in, bltl_input, prd_inputs, constraint_inputs, weight_inputs);
        
        
        bltlChecker= new RecursiveBltlChecker(bltl, prds, trajectories[0]);
    }
    

    void init_with_trajectories(pt::ptree in, string bltl_input, vector<string> prd_inputs, vector<string> constraint_inputs, vector<string> weight_inputs){
        input=in;
        prds = parse_prd(prd_inputs);
        bltl = parse_bltl(bltl_input);
        link_prd(bltl, prds);
        params = new ParameterSet(bltl, prds, trajectories);
        params->init_prd_range();
        params->init_time_range();
        params->parse_constraint_tree(constraint_inputs);
        if(!weight_inputs.empty())
            params->parse_weight(weight_inputs);
        try{
            string init_string = in.get<string>("input.init");
            initial_state = read_property_values(init_string);
        } catch(...){
            
        }
        
    }



};

class GPUMiner : public FileMiner{
  public:    
      GPUMiner(pt::ptree in, string bltl_input, vector<string> prd_inputs, vector<string> constraint_inputs, vector<string> weight_inputs){
    string filename = input.get<string>("input.filename");
      GPUFileTrajectoryProvider *trajProvider = new GPUFileTrajectoryProvider(filename);
      trajectories = trajProvider->getTrajectories(MAX_SIM);
      
      init_with_trajectories(in, bltl_input, prd_inputs, constraint_inputs, weight_inputs);


      bltlChecker= new GPUBltlChecker(bltl, prds, trajProvider);
      
    }



};


class MinerBuilder{
public:
	static vector<Miner*> buildMiner(string filename, bool isFileMiner){
        vector<Miner*> miners;
        pt::ptree input;
        pt::read_xml(filename, input, pt::xml_parser::trim_whitespace);

        vector<string> bltl_inputs;
        BOOST_FOREACH(pt::ptree::value_type &v, input.get_child("input.bltls")) {
            // The data function is used to access the data stored in a node.
            bltl_inputs.push_back(v.second.data());
        }
        vector<string> prd_inputs;
        BOOST_FOREACH(pt::ptree::value_type &v, input.get_child("input.prds")) {
            // The data function is used to access the data stored in a node.
            prd_inputs.push_back(v.second.data());
        }
        vector<string> constraint_inputs;
        BOOST_FOREACH(pt::ptree::value_type &v, input.get_child("input.constraints")) {
            // The data function is used to access the data stored in a node.
            constraint_inputs.push_back(v.second.data());
        }
        vector<string> weight_inputs;
        BOOST_FOREACH(pt::ptree::value_type &v, input.get_child("input.weights")) {
            // The data function is used to access the data stored in a node.
            weight_inputs.push_back(v.second.data());
        }

        if(isFileMiner){
            for(auto itr = bltl_inputs.begin(); itr != bltl_inputs.end(); itr++)
                miners.push_back(new FileMiner(input, *itr+";", prd_inputs, constraint_inputs, weight_inputs));
            return miners;
        }
  }
  static vector<Miner*> buildGPUMiner(string filename){
    vector<Miner*> miners;
    pt::ptree input;
    pt::read_xml(filename, input, pt::xml_parser::trim_whitespace);

    vector<string> bltl_inputs;
    BOOST_FOREACH(pt::ptree::value_type &v, input.get_child("input.bltls")) {
      // The data function is used to access the data stored in a node.
      bltl_inputs.push_back(v.second.data());
    }
    vector<string> prd_inputs;
    BOOST_FOREACH(pt::ptree::value_type &v, input.get_child("input.prds")) {
      // The data function is used to access the data stored in a node.
      prd_inputs.push_back(v.second.data());
    }
    vector<string> constraint_inputs;
    BOOST_FOREACH(pt::ptree::value_type &v, input.get_child("input.constraints")) {
      // The data function is used to access the data stored in a node.
      constraint_inputs.push_back(v.second.data());
    }
    vector<string> weight_inputs;
    BOOST_FOREACH(pt::ptree::value_type &v, input.get_child("input.weights")) {
      // The data function is used to access the data stored in a node.
      weight_inputs.push_back(v.second.data());
    }

    for(auto itr = bltl_inputs.begin(); itr != bltl_inputs.end(); itr++)
      miners.push_back(new GPUMiner(input, *itr+";", prd_inputs, constraint_inputs, weight_inputs));
    return miners;
  }

};



#endif //GPPM_MINING_H
