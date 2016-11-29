//
// Created by Zhou Jun on 14/11/16.
//

#include "../../include/miner_utils.h"
#include <sstream>
#include <random>

double _recursive_generate_prd(Parameter* param, map<string, double> &value_set);

vector<pair<string, string> > parse_constraint(vector<string> inputs){
    vector<pair<string, string> > constraints;
    for(auto input = inputs.begin(); input !=inputs.end(); input++) {
        string item = *input;
        size_t begin = 0;
        size_t found;
        string low, high;
        if((found=item.find("<"))!=string::npos) {
            low = item.substr(0, found);
            high = item.substr(found + 1, string::npos);
        }
        else if((found=item.find(">"))!=string::npos) {
            high = item.substr(0, found);
            low = item.substr(found + 1, string::npos);
        }

        constraints.push_back(pair<string, string>(low, high));
    }
    return constraints;
}

map<string, double> generate_prd(vector<Parameter*> constraint_tree){
    bool success = false;
    int trials = 0;
    while(!success && trials < 100){
        try{
            map<string, double> value_set;
            for(int i = 0; i < constraint_tree.size(); i++){
                _recursive_generate_prd(constraint_tree[i], value_set);
            }
            return value_set;
            success=true;
        }
        catch(int e){trials++;}
    }
}

double _recursive_generate_prd(Parameter* param, map<string, double> &value_set){
    if(value_set.find(param->name)!=value_set.end())
        return value_set[param->name];
    double min = param->range.second;
    for(int i = 0; i < param->children.size(); i++){
        double value = _recursive_generate_prd(param->children[i], value_set);
        if(min>value)
            min=value;
    }
    if (param->range.first>min)
        throw -1;
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> distribution(param->range.first,min);
    param->value=distribution(generator);
    value_set[param->name]=param->value;
    return param->value;
}


map<string, int> generate_time(map<string, TimeVariable*> params){
    map<string, int> value_set;
    for(auto itr=params.begin(); itr!=params.end(); itr++){
        std::random_device rd;
        std::default_random_engine generator(rd());
        std::uniform_int_distribution<int> distribution(itr->second->range.first,itr->second->range.second);
            itr->second->value = distribution(generator);
            value_set[itr->first]=itr->second->value;
    }
    return value_set;
        
}



