//
// Created by Zhou Jun on 16/11/16.
//

#include <vector>
#include <cmath>

#include <gsl/gsl_sf_gamma.h>
#include "ModelChecking.h"
#include "State.h"
#include "Bltl/BltlChecker.h"
using namespace std;
double bayes_factor(vector<int> satArray, double theta=0.9, double alpha=1.0, double beta=1.0){
    int safe=0;
    int total = satArray.size();
    for(int i =0; i < total; i++){
        safe+=satArray[i];
    }
    if(safe==0)
    	return 0;
    return 1.0/gsl_sf_beta_inc(safe + alpha, total - safe + beta, theta)-1;
}

double modelchecking(State *state){
    state->assignValues();
    double bayes = 1;
    vector<int> result;
    while(bayes>BAYES_MIN && bayes < BAYES_MAX && result.size()<MAX_SIM) {
        for (int i = 0; i < N_SIM; i++){
            result.push_back(state->bltlChecker->check((*(state->trajectories))[result.size()]));
        }
        bayes = bayes_factor(result);
    }


    return bayes;
}

double loss(State* state){
    double bayes = modelchecking(state);
    if(bayes < BAYES_MAX)
        return LOSS_MAX;
    double score = 1.0;
    double weight = 0.2;
    for(auto itr=state->paramset->unknown_time_set.begin(); itr!=state->paramset->unknown_time_set.end(); itr++){
        if(state->paramset->weights[itr->first])
            weight = state->paramset->weights[itr->first];
        else
            weight=0.2;
        score*=pow(itr->second->value, weight*itr->second->weight_sign);
    }
    for(auto itr=state->paramset->prds.begin(); itr!=state->paramset->prds.end(); itr++)
        if(!itr->second->left->isfix || !itr->second->right->isfix){
            double distance = itr->second->right->value - itr->second->left->value;
            double max_distance = itr->second->right->range.second - itr->second->left->range.first;
            if(state->paramset->weights[itr->first])
                weight = state->paramset->weights[itr->first];
            else
                weight=0.2;
            score*=pow(distance/max_distance, state->paramset->weights[itr->first]);

            double avg_distance = (itr->second->right->value + itr->second->left->value)/2-itr->second->left->range.first;
            score*=pow(avg_distance/max_distance, itr->second->weight_sign);
        }
    //score*=1/log(bayes);
    return score;
}
