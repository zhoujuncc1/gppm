//
// Created by Zhou Jun on 11/1/17.
//

#include "../../include/Mining.h"
#include "../../include/ModelChecking.h"
#include "../../include/State.h"
#include "../../include/miner_utils.h"
#include <boost/property_tree/ptree.hpp>
#include <gsl/gsl_siman.h>

namespace pt = boost::property_tree;

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

double sa_loss(void* xp)
{
    State* state = (State*)xp;
    return (*(state->loss))(state);
}

void sa_print(void* xp)
{
    State* state = (State*)xp;
    printf("{");
    for (auto itr = state->prd_values.begin(); itr != state->prd_values.end();
         itr++) {
        printf("%s: %f,", itr->first.c_str(), itr->second);
    }
    for (auto itr = state->time_values.begin(); itr != state->time_values.end();
         itr++) {
        printf("%s: %d,", itr->first.c_str(), itr->second);
    }
    printf("}");
}

void sa_step(const gsl_rng* r, void* xp, double step_size)
{
    State* state = (State*)xp;
    generate_prd(state->paramset->tree_roots, step_size, state->prd_values);
    generate_time(state->paramset->unknown_time_set, step_size,
        state->time_values);
}

double sa_metric(void* xp, void* yp)
{
    State* state1 = (State*)xp;
    State* state2 = (State*)yp;
    double distance = 0;
    for (auto itr = state1->prd_values.begin(); itr != state1->prd_values.end();
         itr++) {
        distance += itr->second - state2->prd_values[itr->first];
    }
    for (auto itr = state1->time_values.begin(); itr != state1->time_values.end();
         itr++) {
        distance += itr->second - state2->time_values[itr->first];
    }
    return distance;
}

void sa_copy(void* source, void* dest)
{
    State* src = (State*)source;
    State* dst = (State*)dest;
    dst->paramset = src->paramset;
    dst->prd_values = src->prd_values;
    dst->time_values = src->time_values;
    dst->trajectories = src->trajectories;
}

void* sa_copy_construct(void* xp) { return new State((State*)xp); }

void sa_destroy(void* xp)
{
    State* state = (State*)xp;
    delete state;
}

void do_mine(State* state, pt::ptree config)
{
    printf("Initial State: \n");
    sa_print(state);
    printf("-------------------------\n");

    gsl_siman_params_t params = { config.get("N_TRIES", N_TRIES),
        config.get("ITERS_FIXED_T", ITERS_FIXED_T),
        config.get("STEP_SIZE", STEP_SIZE),
        config.get("K", K),
        config.get("T_INITIAL", T_INITIAL),
        config.get("MU_T", MU_T),
        config.get("T_MIN", T_MIN) };
    const gsl_rng_type* T;
    gsl_rng* r;
    gsl_rng_env_setup();

    T = gsl_rng_default;
    r = gsl_rng_alloc(T);
    gsl_siman_solve(r, state, sa_loss, sa_step, sa_metric, sa_print, sa_copy,
        sa_copy_construct, sa_destroy, 0, params);
}
