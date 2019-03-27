//
// Created by Zhou Jun on 14/11/16.
//

#ifndef MINER_UTILS_H
#define MINER_UTILS_H

#include "../include/Bltl/Parameter.h"
#include "../include/Bltl/TimeVariable.h"
#include <map>
#include <string>
#include <vector>

#define RESOLUTION 100

using namespace std;

map<string, double> generate_prd(vector<Parameter*> constraint_tree);
map<string, int> generate_time(map<string, TimeVariable*> params);

map<string, double> generate_prd(vector<Parameter*> constraint_tree,
    map<string, string> initial_state);
map<string, int> generate_time(map<string, TimeVariable*> params,
    map<string, string> initial_state);

void generate_prd(vector<Parameter*> constraint_tree, double step_size,
    map<string, double>& current);
void generate_time(map<string, TimeVariable*> params, double step_size,
    map<string, int>& current);

#endif // MINER_UTILS_H
