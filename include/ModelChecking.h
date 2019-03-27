//
// Created by Zhou Jun on 16/11/16.
//

#ifndef GPPM_MODELCHECKING_H
#define GPPM_MODELCHECKING_H
#include "Bltl/Bltl.h"
#include "Bltl/BltlChecker.h"
#include "Bltl/bltl_parser.h"
#include "Model.h"
#include "ParameterSet.h"
#include "miner_utils.h"
#include "State.h"
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <ctime>
#include <map>
#include <string>
#include <vector>

#define N_SIM 100
#define MAX_SIM 1000


using namespace std;
namespace pt = boost::property_tree;

//class State;

class ModelChecker {
public:
    map<string, Prd*> prds;
    Bltl* bltl;
    vector<Trajectory> trajectories;
    BltlChecker* bltlChecker;
    ParameterSet* params;
    map<string, string> initial_state;
    ModelChecker() {}
    ModelChecker(string bltl_input, vector<string> prd_inputs, string filename)
    {
        prds = parse_prd(prd_inputs);
        bltl = parse_bltl(bltl_input);
        link_prd(bltl, prds);
        FileTrajectoryProvider trajProvider(filename);
        trajectories = trajProvider.getTrajectories(MAX_SIM);
        params = new ParameterSet(bltl, prds, trajectories);
        params->init_prd_range();
        params->init_time_range();
        params->parse_constraint_tree(vector<string>());

        bltlChecker = new RecursiveBltlChecker(bltl, prds, trajectories[0]);
    }

    ModelChecker(string init_string, string bltl_input, vector<string> prd_inputs,
        string filename)
        : ModelChecker(bltl_input, prd_inputs, filename)
    {
        try {
            initial_state = read_property_values(init_string);
            generate_prd(params->tree_roots, initial_state);
            generate_time(params->unknown_time_set, initial_state);

        } catch (...) {
        }
    }

    vector<int> check()
    {
        clock_t begin = clock();

        vector<int> results;
        if (bltlChecker->isGPU) {
            results = bltlChecker->check();
        } else {
            results = bltlChecker->check(trajectories);
        }
        clock_t end = clock();
        long elapsed_secs = end - begin;
        printf("Elapsed Time: %ld seconds\n", elapsed_secs);
        return results;
    }

    static vector<ModelChecker*> buildChecker(string filename)
    {
        vector<ModelChecker*> checkers;

        pt::ptree input;
        pt::read_xml(filename, input, pt::xml_parser::trim_whitespace);

        vector<string> bltl_inputs;
        BOOST_FOREACH (pt::ptree::value_type& v, input.get_child("input.bltls")) {
            // The data function is used to access the data stored in a node.
            bltl_inputs.push_back(v.second.data());
        }
        vector<string> prd_inputs;
        BOOST_FOREACH (pt::ptree::value_type& v, input.get_child("input.prds")) {
            // The data function is used to access the data stored in a node.
            prd_inputs.push_back(v.second.data());
        }
        string traj_filename = input.get<string>("input.filename");
        boost::optional<string> v = input.get_optional<string>("input.init");
        if (v) {
            string init_string = v.value();
            for (auto itr = bltl_inputs.begin(); itr != bltl_inputs.end(); itr++)
                checkers.push_back(new ModelChecker(init_string, *itr + ";", prd_inputs,
                    traj_filename));
        } else {
            for (auto itr = bltl_inputs.begin(); itr != bltl_inputs.end(); itr++)
                checkers.push_back(
                    new ModelChecker(*itr + ";", prd_inputs, traj_filename));
        }
        return checkers;
    }
};

class GPUModelChecker : public ModelChecker {
public:
    GPUModelChecker(string init_string, string bltl_input,
        vector<string> prd_inputs, string filename)
        : GPUModelChecker(bltl_input, prd_inputs, filename)
    {
        try {
            initial_state = read_property_values(init_string);
            generate_prd(params->tree_roots, initial_state);
            generate_time(params->unknown_time_set, initial_state);
        } catch (...) {
        }
    }
    GPUModelChecker(string bltl_input, vector<string> prd_inputs,
        string filename)
    {
        prds = parse_prd(prd_inputs);
        bltl = parse_bltl(bltl_input);
        link_prd(bltl, prds);
        GPUFileTrajectoryProvider* trajProvider = new GPUFileTrajectoryProvider(filename);
        trajectories = trajProvider->getTrajectories(MAX_SIM);
        params = new ParameterSet(bltl, prds, trajectories);
        params->init_prd_range();
        params->init_time_range();
        params->parse_constraint_tree(vector<string>());

        bltlChecker = new GPUBltlChecker(bltl, prds, trajProvider);
    }

    /*
      vector<int> check()
      {
          return bltlChecker->check();
      }
  */
    static vector<GPUModelChecker*> buildChecker(string filename)
    {
        vector<GPUModelChecker*> checkers;

        pt::ptree input;
        pt::read_xml(filename, input, pt::xml_parser::trim_whitespace);

        vector<string> bltl_inputs;
        BOOST_FOREACH (pt::ptree::value_type& v, input.get_child("input.bltls")) {
            // The data function is used to access the data stored in a node.
            bltl_inputs.push_back(v.second.data());
        }
        vector<string> prd_inputs;
        BOOST_FOREACH (pt::ptree::value_type& v, input.get_child("input.prds")) {
            // The data function is used to access the data stored in a node.
            prd_inputs.push_back(v.second.data());
        }
        string traj_filename = input.get<string>("input.filename");
        boost::optional<string> v = input.get_optional<string>("input.init");
        if (v) {
            string init_string = v.value();
            for (auto itr = bltl_inputs.begin(); itr != bltl_inputs.end(); itr++)
                checkers.push_back(new GPUModelChecker(init_string, *itr + ";",
                    prd_inputs, traj_filename));
        } else {
            for (auto itr = bltl_inputs.begin(); itr != bltl_inputs.end(); itr++)
                checkers.push_back(
                    new GPUModelChecker(*itr + ";", prd_inputs, traj_filename));
        }
        return checkers;
    }
};

#endif // GPPM_MODELCHECKING_H
