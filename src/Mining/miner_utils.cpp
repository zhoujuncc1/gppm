//
// Created by Zhou Jun on 14/11/16.
//

#include "../../include/miner_utils.h"
#include <sstream>
#include <random>

std::random_device rd;
std::default_random_engine generator(rd());
std::uniform_int_distribution<int> distribution(0, RESOLUTION);

double _recursive_generate_prd(Parameter* param, map<string, double> &value_set);
double _recursive_generate_prd(Parameter* param, map<string, double> &value_set,  map<string, double> &current, double step_size);
double _generate_real(double min, double max);

void generate_time(map<string, TimeVariable*> params, double step_size, map<string, int> &current){
    for(auto itr=params.begin(); itr!=params.end(); itr++){
        std::random_device rd;
        std::default_random_engine generator(rd());
        int left_bound = (1.0 + step_size)*current[itr->first] > itr->second->range.first?(1.0 + step_size)*current[itr->first]:itr->second->range.first;
        int right_bound = (1.0 - step_size)*current[itr->first] < itr->second->range.second?(1.0 - step_size)*current[itr->first] : itr->second->range.second;
        std::uniform_int_distribution<int> distribution(-left_bound,right_bound);
        current[itr->first]=distribution(generator);
    }        
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


void generate_prd(vector<Parameter*> constraint_tree, double step_size, map<string, double>& current){
    bool success = false;
    int trials = 0;
    while(!success && trials < 100){
        try{
            map<string, double> value_set;
            for(int i = 0; i < constraint_tree.size(); i++){
                _recursive_generate_prd(constraint_tree[i], value_set, current, step_size);
            }
            return;
            success=true;
        }
        catch(int e){trials++;}
    }
    if(!success){
        printf("Generate Error!\n");
        exit(1);
    }
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
    if(!success){
        printf("Generate Error!\n");
        exit(1);
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
    param->value=_generate_real(param->range.first,min);
    if(param->value<0)
        param->value = 0;
    value_set[param->name]=param->value;
    return param->value;
}

double _recursive_generate_prd(Parameter* param, map<string, double> &value_set,  map<string, double> &current, double step_size){
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
    double left_bound = current[param->name]-step_size>param->range.first?current[param->name]-step_size:param->range.first;
    double right_bound = current[param->name]+step_size<min?current[param->name]+step_size:min;
    param->value=_generate_real(left_bound,right_bound);

    if(param->value<0)
        param->value = 0;
    current[param->name]=param->value;
    value_set[param->name]=param->value;
    return param->value;
}




double _generate_real(double min, double max){
    return min+(max-min)/RESOLUTION*distribution(generator);
}
