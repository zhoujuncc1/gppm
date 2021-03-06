/*
 * ParameterSet.h
 *
 *  Created on: 2 Nov 2016
 *      Author: zhoujun
 */

#ifndef SRC_PARAMETERSET_H_
#define SRC_PARAMETERSET_H_
#include "Bltl/Bltl.h"
#include "Model.h"
#include <map>

using namespace std;
class ParameterSet {
public:
    ParameterSet(Bltl* bltl, map<string, Prd*> prds,
        vector<Trajectory> trajectories);

    map<string, Parameter*> unknown_prd_set;
    map<string, Parameter*> known_prd_set;
    map<string, TimeVariable*> unknown_time_set;
    map<string, TimeVariable*> known_time_set;
    map<string, Parameter*> all_set;

    // Smallest are roots
    vector<Parameter*> tree_roots;
    map<string, double> weights;
    map<string, Prd*> prds;

    void init_prd_range();
    void init_time_range();

    // prase to trees where smallest are roots
    void parse_constraint_tree(vector<string> inputs);

    void parse_weight(vector<string> inputs);
    Bltl* bltl;
    vector<Trajectory> trajectories;

private:
    vector<pair<float, float>> ranges;
    vector<pair<float, float>> min;
    vector<pair<float, float>> max;
    vector<pair<float, float>> t0;
    vector<pair<string, string>> constraints;

    void init_traj_ranges();
    void findParameters(Bltl* bltl);
    void resolveFlags(map<string, Prd*> prds);
    void parse_constraint(vector<string> inputs);
};

#endif /* SRC_PARAMETERSET_H_ */
