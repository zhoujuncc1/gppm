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

    void mine(){
        state = new State(params);
        state->prd_values = generate_prd(params->tree_roots);
        state->time_values = generate_time(params->unknown_time_set);
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
            while(loss_v < LOSS_MAX && itr->second->value>itr->second->range.first && itr->second->value < itr->second->range.second){
                itr->second->value-=itr->second->weight_sign;
                cout<<itr->second->value<<", ";
                loss_v = loss(state);
                cout<<loss_v << "| ";
            }
            cout << endl<<endl;
            itr->second->value+=itr->second->weight_sign;
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

};

class SingleMiner : public Miner{
public:
    SingleMiner(){}
    SingleMiner(pt::ptree in, string bltl_input, vector<string> prd_inputs, vector<string> constraint_inputs, vector<string> weight_inputs) : Miner(in, bltl_input, prd_inputs, constraint_inputs, weight_inputs){}
    void mineGeneral(){
        do_mine(state, input.get_child("input.config"));
    }
};

class FileMiner : public SingleMiner{
public:    
    FileMiner(pt::ptree in, string bltl_input, vector<string> prd_inputs, vector<string> constraint_inputs, vector<string> weight_inputs){
        input = in;
        prds = parse_prd(prd_inputs);
        bltl = parse_bltl(bltl_input);
        link_prd(bltl, prds);
        string filename = input.get<string>("input.filename");
        FileTrajectoryProvider trajProvider(filename);
        trajectories = trajProvider.getTrajectories(MAX_SIM);
        params = new ParameterSet(bltl, prds, trajectories);
        params->init_prd_range();
        params->init_time_range();
        params->parse_constraint_tree(constraint_inputs);
        if(!weight_inputs.empty())
            params->parse_weight(weight_inputs);
        
        
        bltlChecker= new RecursiveBltlChecker(bltl, prds, trajectories[0]);
        //bltlChecker= new TreeBltlChecker(bltl);
    }



};

class ClusterMiner : public Miner{
public:
    ClusterMiner(pt::ptree in, string bltl_input, vector<string> prd_inputs, vector<string> constraint_inputs, vector<string> weight_inputs) : Miner(in, bltl_input, prd_inputs, constraint_inputs, weight_inputs){
        pt::ptree cluster_conf = input.get_child("input.cluster");
        input_t = cluster_conf.get<int>("input_t");
        cluster_prd = cluster_conf.get<int>("cluster_prd");
        cluster_time = cluster_conf.get<int>("cluster_time");

    }
    void mineGeneral(){
        vector<string> prd_keys, time_keys;
        vector<int> prd_index, time_index;

        int length = 0;
        for (auto it=state->prd_values.begin(); it!=state->prd_values.end(); ++it){
            prd_keys.push_back(it->first);
            prd_index.push_back(length++);
        }
        for (auto it=state->time_values.begin(); it!=state->time_values.end(); ++it) {
            time_keys.push_back(it->first);
            time_index.push_back(length++);
        }

        for(int i = 0; i < input_t; i++) {
            vector<double> values;
            _generate_property(state);
            BOOST_FOREACH(string k, prd_keys){
                values.push_back(state->prd_values[k]);
            }
            BOOST_FOREACH(string k, time_keys){
                values.push_back(state->time_values[k]);
            }
            points.push_back(Point(i, values));
        }
        KMeans kmeans(cluster_prd, input_t, length, prd_index, 1000);
	    kmeans.run(points);
        
        cout << "======================================" << "\n";
        cout << "======================================" << "\n";

        for(int i = 0; i < cluster_prd; i++){
            cout << "Cluster " << i+1 <<"\n";
            cout << "======================================" << "\n";
            KMeans sub_kmeans(cluster_time, kmeans.getClusters()[i].getTotalPoints(), length, time_index, 1000);
            sub_kmeans.run(kmeans.getClusters()[i].getPoints());
        }


    }
    void _generate_property(State* state){
        int trials = 0;
        double bayes = 0;
        do{
            state->prd_values = generate_prd(state->paramset->tree_roots);
            state->time_values = generate_time(state->paramset->unknown_time_set);
            bayes = modelchecking(state);
        } while(bayes < BAYES_MAX && trials<1000);
        if(bayes < BAYES_MAX)
            exit(1);
        
    }
    int input_t;
    int cluster_time, cluster_prd;
    vector<Point> points;

};

/*
#include <omp.h>
class ParallelClusterMiner : public ClusterMiner{
public:
    ParallelClusterMiner(pt::ptree input) {
        ClusterMiner(input);
    }
    void mine(){
        state = new State(params);
        state->trajectories = &trajectories;
        state->prd_values = generate_prd(params->tree_roots);
        state->time_values = generate_time(params->unknown_time_set);
        vector<string> prd_keys, time_keys;
        vector<int> prd_index, time_index, all_index;

        int length = 0;
        for (auto it=state->prd_values.begin(); it!=state->prd_values.end(); ++it){
            prd_keys.push_back(it->first);
            prd_index.push_back(length);
            all_index.push_back(length);
            length++;
        }
        for (auto it=state->time_values.begin(); it!=state->time_values.end(); ++it) {
            time_keys.push_back(it->first);
            time_index.push_back(length);
            all_index.push_back(length);
            length++;
        }
        omp_lock_t writelock;

        omp_init_lock(&writelock);
#pragma omp parallel for
        for(int i = 0; i < input_t; i++) {
            vector<double> values;
            _generate_property(state);
            BOOST_FOREACH(string k, prd_keys){
                values.push_back(state->prd_values[k]);
            }
            BOOST_FOREACH(string k, time_keys){
                values.push_back(state->time_values[k]);
            }
            omp_set_lock(&writelock);
            points.push_back(Point(i, values));
            omp_unset_lock(&writelock);

        }
        omp_destroy_lock(&writelock);

        KMeans kmeans(cluster_time, input_t, length, time_index, 1000);
	    kmeans.run(points);
        
        cout << "======================================" << "\n";
        cout << "======================================" << "\n";

        for(int i = 0; i < cluster_time; i++){
            cout << "Cluster " << i+1 <<"\n";
            cout << "======================================" << "\n";
            KMeans sub_kmeans(cluster_prd, kmeans.getClusters()[i].getTotalPoints(), length, prd_index, 1000);
            sub_kmeans.run(kmeans.getClusters()[i].getPoints());
        }
        


    }


};
*/


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
        else {
            string miner_type = input.get<string>("input.miner");
            if (miner_type.compare("single")==0){
                for(auto itr = bltl_inputs.begin(); itr != bltl_inputs.end(); itr++)
                    miners.push_back(new SingleMiner(input, *itr+";", prd_inputs, constraint_inputs, weight_inputs));
            }
            else if(miner_type.compare("cluster")==0){
                for(auto itr = bltl_inputs.begin(); itr != bltl_inputs.end(); itr++)
                    miners.push_back(new ClusterMiner(input, *itr+";", prd_inputs, constraint_inputs, weight_inputs));
            }
            return miners;
        }
	}

};



#endif //GPPM_MINING_H
