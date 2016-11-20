//
// Created by Zhou Jun on 16/11/16.
//

#ifndef GPPM_MODELCHECKING_H
#define GPPM_MODELCHECKING_H
#include "Model.h"
#define N_SIM=100;
#define MAX_SIM=1000;
#define BAYES_MIN=0.01;
#define BAYES_MAX=100;
#define LOSS_MAX=1000000;



double modelchecking(State *state);
double loss(State* state);
#endif //GPPM_MODELCHECKING_H
