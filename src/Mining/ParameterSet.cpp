/*
 * ParameterSet.cpp
 *
 *  Created on: 2 Nov 2016
 *      Author: zhoujun
 */

#include "../../include/ParameterSet.h"
#include "../../include/Bltl/TimeVariable.h"
#include "../../include/miner_utils.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>

#define N_TRAJ 200

ParameterSet::ParameterSet(Bltl *bltl, map<std::string, Prd*> prds) {
    this->bltl = bltl;
    this->prds = prds;
    init_traj_ranges();
    resolveFlags(this->prds);
    findParameters(this->bltl);
}

void ParameterSet::init_traj_ranges(){
    std::vector <Trajectory> trajectories;
    for (int i = 0; i < N_TRAJ; i++)
        trajectories.push_back(Model::simulate(1.0));
    for (auto itr = trajectories[0].m_states[0].begin(); itr != trajectories[0].m_states[0].end();itr++){
        ranges.push_back(std::pair<double, double>(*itr, *itr));
        min.push_back(std::pair<double, double>(*itr, *itr));
        max.push_back(std::pair<double, double>(*itr, *itr));
    }
    double t_min[Model::N_SPECIES];        
    double t_max[Model::N_SPECIES];
    for (int j = 0; j < N_TRAJ; j++){
        for (int i = 0; i < Model::N_SPECIES; i++){
            t_min[i] = trajectories[j].m_states[0][i];
            t_max[i] = trajectories[j].m_states[0][i];
        }
        for (auto itr = trajectories[j].m_states.begin(); itr != trajectories[j].m_states.end(); itr++)
            for (int i = 0; i < Model::N_SPECIES; i++) {
                ranges[i].first = std::min(ranges[i].first, (*itr)[i]);
                ranges[i].second = std::max(ranges[i].second, (*itr)[i]);
                t_min[i] = std::min(t_min[i], (*itr)[i]);
                t_max[i] = std::max(t_max[i], (*itr)[i]);

            }
        for (int i = 0; i < Model::N_SPECIES; i++){
            min[i].first = std::min(min[i].first, t_min[i]);
            min[i].second = std::max(min[i].second, t_min[i]);
            max[i].first = std::min(max[i].first, t_max[i]);
            max[i].second = std::max(max[i].second, t_max[i]);
        }
    }
    for (int i = 0; i < Model::N_SPECIES; i++) {
        double dis = ranges[i].second - ranges[i].first;
        ranges[i].first -= 0.1 * dis;
        ranges[i].second += 0.1 * dis;

        dis = min[i].second - min[i].first;
        min[i].first -= 0.1 * dis;
        min[i].second += 0.1 * dis;

        dis = max[i].second - max[i].first;
        max[i].first -= 0.1 * dis;
        max[i].second += 0.1 * dis;
    }
}
void ParameterSet::resolveFlags(std::map<std::string, Prd*> prds){
    for(auto itr = prds.begin(); itr != prds.end(); itr++){
        if(itr->second->isFlag){
            if(itr->second->flag==MAX)
                itr->second->resolveFlag(max[itr->second->varId].first, max[itr->second->varId].second);
            if(itr->second->flag==MIN)
                itr->second->resolveFlag(min[itr->second->varId].first, min[itr->second->varId].second);    
        }
    }
}


void ParameterSet::findParameters(Bltl *bltl) {
    if (bltl->getOperation() == op_PRD) {
        if (bltl->getPrd()->left->isfix)
            known_prd_set[bltl->getPrd()->left->name] = bltl->getPrd()->left;
        else
            unknown_prd_set[bltl->getPrd()->left->name] = bltl->getPrd()->left;

        all_set[bltl->getPrd()->left->name] = bltl->getPrd()->left;

        if (bltl->getPrd()->right->isfix)
            known_prd_set[bltl->getPrd()->right->name] = bltl->getPrd()->right;
        else
            unknown_prd_set[bltl->getPrd()->right->name] = bltl->getPrd()->right;

        all_set[bltl->getPrd()->right->name] = bltl->getPrd()->right;
    } else {
        if (bltl->getTime()) {
            if (bltl->getTime()->isfix)
                known_time_set[bltl->getTime()->name] = bltl->getTime();
            else
                unknown_time_set[bltl->getTime()->name] = bltl->getTime();
            all_set[bltl->getTime()->name] = bltl->getTime();
        }
        if (bltl->getChild1())
            findParameters(bltl->getChild1());
        if (bltl->getChild2())
            findParameters(bltl->getChild2());
    }
}

void ParameterSet::init_prd_range() {
    for (auto itr = unknown_prd_set.begin(); itr != unknown_prd_set.end(); itr++)
        itr->second->range = ranges[itr->second->varId];

}

void ParameterSet::init_time_range() {
    for (auto itr = unknown_time_set.begin(); itr != unknown_time_set.end(); itr++) {
        itr->second->range.first = 1;
        itr->second->range.second = (int) Model::end_time;

    }
}

void ParameterSet::parse_constraint_tree(std::vector<std::string> inputs) {
    vector <pair<string, string> > constraints = parse_constraint(inputs);
    for (auto itr = constraints.begin(); itr != constraints.end(); itr++)
        if (all_set[itr->first]->isfix)
            all_set[itr->second]->range.first = all_set[itr->first]->value;
        else if (all_set[itr->second]->isfix)
            all_set[itr->first]->range.second = all_set[itr->second]->value;
        else {
            all_set[itr->first]->children.push_back(all_set[itr->second]);
            all_set[itr->second]->parents.push_back(all_set[itr->first]);
        }
    for (auto itr = unknown_prd_set.begin(); itr != unknown_prd_set.end(); itr++)
        if (itr->second->parents.size() == 0)
            tree_roots.push_back(itr->second);

}

void ParameterSet::parse_weight(std::vector<std::string> inputs) {
    for(auto input = inputs.begin(); input !=inputs.end(); input++) {
        string item = *input;
        size_t begin = 0;
        size_t found = item.find(":");
        std::string k = item.substr(0, found);
        boost::trim_right(k);
        begin = found + 1;
        std::string v = item.substr(begin);
        boost::trim_left(v);
        weights[k] = std::stod(v);
    }
}
