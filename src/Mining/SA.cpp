#include "../../include/miner_utils.h"
#include "../../include/ModelChecking.h"
#include "../../include/Mining.h"
#include "../../include/State.h"
#include <gsl/gsl_siman.h>

/* how many points do we try before stepping */
#define N_TRIES 200

/* how many iterations for each T? */
#define ITERS_FIXED_T 1000

/* max step size in random walk */
#define STEP_SIZE 1.0

/* Boltzmann constant */
#define K 1.0

/* initial temperature */
#define T_INITIAL 0.008

/* damping factor for temperature */
#define MU_T 1.003
#define T_MIN 2.0e-6

gsl_siman_params_t params
        = {N_TRIES, ITERS_FIXED_T, STEP_SIZE,
           K, T_INITIAL, MU_T, T_MIN};

double sa_loss (void *xp){
    State* state = (State*)xp;
    loss(state);
}


void sa_step(const gsl_rng *r, void *xp, double step_size){
    State* state = (State*)xp;
    state->prd_values=generate_prd(state->paramset->tree_roots);
    state->time_values=generate_time(state->paramset->unknown_time_set);
}

double sa_metric (void *xp, void *yp){
    State* state1 = (State*)xp;
    State* state2 = (State*)yp;
    double distance = 0;
    for(auto itr = state1->prd_values.begin(); itr!=state1->prd_values.end(); itr++){
        distance+=itr->second-state2->prd_values[itr->first];
    }
    for(auto itr = state1->time_values.begin(); itr!=state1->time_values.end(); itr++){
        distance+=itr->second-state2->time_values[itr->first];
    }
    return distance;
}

void sa_print (void *xp){
    State* state = (State*)xp;
    printf("{");
    for(auto itr = state->prd_values.begin(); itr!=state->prd_values.end(); itr++){
        printf("%s: %f,", itr->first.c_str(), itr->second);
    }
    for(auto itr = state->time_values.begin(); itr!=state->time_values.end(); itr++){
        printf("%s: %d,", itr->first.c_str(), itr->second);
    }
    printf("{\n");
}

void sa_copy (void *source, void *dest){
    State* src = (State*) source;
    State* dst = (State*) dest;
    dst->paramset=src->paramset;
    dst->prd_values=src->prd_values;
    dst->time_values=src->time_values;
}

void * sa_copy_construct (void *xp){
    return new State((State*)xp);
}

void sa_destroy (void *xp){
    State* state = (State*)xp;
    delete state;
}

void do_mine(State* state) {
    const gsl_rng_type * T;
    gsl_rng * r;
    gsl_rng_env_setup();

    T = gsl_rng_default;
    r = gsl_rng_alloc(T);
    gsl_siman_solve(r, state, sa_loss, sa_step, sa_metric, sa_print,
                    sa_copy, sa_copy_construct, sa_destroy, 0, params);
}
