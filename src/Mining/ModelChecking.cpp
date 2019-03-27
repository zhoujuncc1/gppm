//
// Created by Zhou Jun on 16/11/16.
//

#include <cmath>
#include <vector>

#include "../../include/Bltl/BltlChecker.h"
#include "../../include/ModelChecking.h"
#include "../../include/State.h"
#include <gsl/gsl_sf_gamma.h>
using namespace std;

int count(vector<int> satArray)
{
    int safe = 0;
    int total = satArray.size();
    for (int i = 0; i < total; i++) {
        safe += satArray[i];
    }
    return safe;
}

double bayes_factor(vector<int> satArray, double theta = 0.9,
    double alpha = 1.0, double beta = 1.0)
{
    int safe = count(satArray);
    if (safe == 0)
        return 0;
    return 1.0 / gsl_sf_beta_inc(safe + alpha, satArray.size() - safe + beta, theta) - 1;
}

vector<int> modelchecking(State* state)
{
    state->assignValues();
    double bayes = 1;
    vector<int> result;
    if (state->bltlChecker->isGPU) {
        result = state->bltlChecker->check();
    } else {
        for (int i = 0; i < state->trajectories->size(); i++) {
            result.push_back(state->bltlChecker->check((*(state->trajectories))[i]));
        }
    }

    return result;
}


double loss(State* state)
{
    vector<int> result = modelchecking(state);
    // double bayes = bayes_factor(result);
    double bayes = ((double)count(result)) / result.size();
    if (bayes < RATE_MAX)
        return LOSS_MAX;
    double score = 1.0;
    double weight = 0.2;
    for (auto itr = state->paramset->unknown_time_set.begin();
         itr != state->paramset->unknown_time_set.end(); itr++) {
        if (state->paramset->weights[itr->first])
            weight = state->paramset->weights[itr->first];
        else
            weight = 0.2;
        score *= pow(itr->second->value, weight * itr->second->weight_sign);
        // printf("%d, %f, %f, %.f\n", itr->second->value, weight,
        // itr->second->weight_sign, score);
    }
    for (auto itr = state->paramset->prds.begin();
         itr != state->paramset->prds.end(); itr++)
        if (!itr->second->left->isfix || !itr->second->right->isfix) {
            double distance = itr->second->right->value - itr->second->left->value;
            double max_distance = itr->second->right->range.second - itr->second->left->range.first;
            if (state->paramset->weights[itr->first])
                weight = state->paramset->weights[itr->first];
            else
                weight = 0.2;
            score *= pow(distance / max_distance, state->paramset->weights[itr->first]);

            double avg_distance = (itr->second->right->value + itr->second->left->value) / 2 - itr->second->left->range.first;
            score *= pow(avg_distance / max_distance, itr->second->weight_sign);
        }
    // score*=1/log(bayes);
    if (score <= 0.0)
        score = LOSS_MAX;
    return score;
}
