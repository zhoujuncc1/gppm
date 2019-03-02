//
// Created by Zhou Jun on 16/11/16.
//

#ifndef GPPM_MODELCHECKING_H
#define GPPM_MODELCHECKING_H
#include "Model.h"
#include <map>
#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include "Bltl/Bltl.h"
#include "Bltl/bltl_parser.h"
#include "Bltl/BltlChecker.h"

#define N_SIM 100
#define MAX_SIM 1000
#define BAYES_MIN 0.01
#define BAYES_MAX 100
#define LOSS_MAX 1000000

using namespace std;
namespace pt = boost::property_tree;

class State;
double modelchecking(State *state);
double loss(State *state);

class ModelChecker
{
  public:
    map<string, Prd *> prds;
    Bltl *bltl;
    vector<Trajectory> trajectories;
    BltlChecker *bltlChecker;
    ModelChecker(string bltl_input, vector<string> prd_inputs, string filename)
    {
        prds = parse_prd(prd_inputs);
        bltl = parse_bltl(bltl_input);
        link_prd(bltl, prds);
        FileTrajectoryProvider trajProvider(filename);
        trajectories = trajProvider.getTrajectories(MAX_SIM);
        bltlChecker = new RecursiveBltlChecker(bltl, prds, trajectories[0]);
    }

    vector<int> check()
    {
        vector<int> results;
        if (bltlChecker->isGPU)
        {
            results = bltlChecker->check();
        }
        else
        {
            for (int i = 0; i < trajectories.size(); i++)
                results.push_back(bltlChecker->check(trajectories[i]));
        }
        return results;
    }

    static vector<ModelChecker *> buildChecker(string filename)
    {
        vector<ModelChecker *> checkers;

        pt::ptree input;
        pt::read_xml(filename, input, pt::xml_parser::trim_whitespace);

        vector<string> bltl_inputs;
        BOOST_FOREACH (pt::ptree::value_type &v, input.get_child("input.bltls"))
        {
            // The data function is used to access the data stored in a node.
            bltl_inputs.push_back(v.second.data());
        }
        vector<string> prd_inputs;
        BOOST_FOREACH (pt::ptree::value_type &v, input.get_child("input.prds"))
        {
            // The data function is used to access the data stored in a node.
            prd_inputs.push_back(v.second.data());
        }
        string traj_filename = input.get<string>("input.filename");

        for (auto itr = bltl_inputs.begin(); itr != bltl_inputs.end(); itr++)
            checkers.push_back(new ModelChecker(*itr + ";", prd_inputs, traj_filename));
        return checkers;
    }
};

#endif //GPPM_MODELCHECKING_H
