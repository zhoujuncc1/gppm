/*
 * ParameterSet.cpp
 *
 *  Created on: 2 Nov 2016
 *      Author: zhoujun
 */

#include "../include/ParameterSet.h"
#include "../include/Bltl/TimeVariable.h"
#include "../include/parse_constraint.h"
#include <algorithm>
#define N_TRAJ 200

ParameterSet::ParameterSet(Bltl *bltl) {
    this->bltl = bltl;
    findParameters(this->bltl);
}

ParameterSet::~ParameterSet() {
    // TODO Auto-generated destructor stub
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

void ParameterSet::init_prd_range(){
    std::vector<Trajectory> trajectories;
    for(int i = 0; i < N_TRAJ; i++)
        trajectories.push_back(Model::simulate(1.0));
    for(int i = 0; i < Model::N_SPECIES; i++){
        auto result = std::minmax_element(trajectories[0].m_states[i].begin(), trajectories[0].m_states[i].end());
        ranges.push_back(std::pair<double, double>(*(result.first),*(result.second)));
        for(int j = 1; j < N_TRAJ; j++){
            result = std::minmax_element(trajectories[j].m_states[i].begin(), trajectories[j].m_states[i].end());
            ranges[i].first=std::min(ranges[i].first, *(result.first));
            ranges[i].second=std::max(ranges[i].second, *(result.second));
        }
    }

    for(auto itr = unknown_prd_set.begin(); itr!=unknown_prd_set.end(); itr++)
        itr->second->range=ranges[itr->second->varId];

}

void ParameterSet::parse_constraint_tree(string input){
    vector<pair<string, string> > constraints = parse_constraint(input);
    for(auto itr = constraints.begin(); itr!=constraints.end(); itr++)
        if(all_set[itr->first]->isfix)
            all_set[itr->second]->range.first=all_set[itr->first]->value;
        else if(all_set[itr->second]->isfix)
            all_set[itr->first]->range.second=all_set[itr->second]->value;
        else{
            all_set[itr->first]->children.push_back(all_set[itr->second]);
            all_set[itr->second]->parents.push_back(all_set[itr->first]);
        }
    for(auto itr = unknown_prd_set.begin(); itr!=unknown_prd_set.end(); itr++)
        if(itr->second->parents.size()==0)
            tree_roots.push_back(itr->second);

}

