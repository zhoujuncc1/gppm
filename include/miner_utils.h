//
// Created by Zhou Jun on 14/11/16.
//

#ifndef MINER_UTILS_H
#define MINER_UTILS_H

#include <string>
#include <vector>
#include <map>
#include "../include/Bltl/TimeVariable.h"
#include "../include/Bltl/Parameter.h"

#define RESOLUTION 10000

using namespace std;

vector<pair<string, string> > parse_constraint(vector<string> inputs);
map<string, double> generate_prd(vector<Parameter*> constraint_tree);
map<string, int> generate_time(map<string, TimeVariable*> params);

#endif //MINER_UTILS_H
