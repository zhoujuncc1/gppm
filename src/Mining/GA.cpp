#include <stdexcept>  // runtime_error
#include <iostream>    // cout
#include <strstream>  // ostrstream, istrstream
#include <eo>
#include <es.h>
#include "../../include/State.h"
#include "../../include/Mining.h"

typedef State Indi;

double fitness(const Indi & _indi) {
    return loss(state);
}
void do_mine(State *state, pt::ptree config)
{
 // all parameters are hard-coded!
 const unsigned int SEED = 42; // seed for random number generator
 const unsigned int VEC_SIZE = 8; // Number of object variables in genotypes
 const unsigned int POP_SIZE = 20; // Size of population
 const unsigned int T_SIZE = 3; // size for tournament selection
 const unsigned int MAX_GEN = 500; // Maximum number of generation before STOP
 const float CROSS_RATE = 0.8; // Crossover rate
 const double EPSILON = 0.01;  // range for real uniform mutation
 const float MUT_RATE = 0.5;    // mutation rate
 //////////////////////////
 //  Random seed
 //////////////////////////
 //reproducible random seed: if you don't change SEED above,
 // you'll aways get the same result, NOT a random run
 rng.reseed(SEED);
 /////////////////////////////
 // Fitness function
 ////////////////////////////
 // Evaluation: from a plain C++ fn to an EvalFunc Object
 eoEvalFuncPtr<Indi> eval(  fitness );
 ////////////////////////////////
 // Initilisation of population
 ////////////////////////////////
 // declare the population
 eoPop<Indi> pop;
 // fill it!
 for (unsigned int igeno=0; igeno<POP_SIZE; igeno++)
     {
         Indi v = createState(state);
         eval(v);                                  // evaluate it
         pop.push_back(v);                // and put it in the population
     }
 // sort pop before printing it!
 pop.sort();
 // Print (sorted) intial population (raw printout)
 cout << "Initial Population" << endl;
 cout << pop;
 /////////////////////////////////////
 // selection and replacement
 ////////////////////////////////////
 // The robust tournament selection
 eoDetTournamentSelect<Indi> select(T_SIZE);            // T_SIZE in [2,POP_SIZE]
 // eoSGA uses generational replacement by default
 // so no replacement procedure has to be given
 //////////////////////////////////////
 // termination condition
 /////////////////////////////////////
 // stop after MAX_GEN generations
 eoGenContinue<Indi> continuator(MAX_GEN);
 
 //////////////////////////////////////
 // The variation operators
 //////////////////////////////////////
 // offspring(i) uniformly chosen in [parent(i)-epsilon, parent(i)+epsilon]
 eoUniformMutation<Indi>  mutation(EPSILON);
 // offspring(i) is a linear combination of parent(i)
 eoSegmentCrossover<Indi> xover;
 /////////////////////////////////////////
 // the algorithm
 ////////////////////////////////////////
 // standard Generational GA requires
 // selection, evaluation, crossover and mutation, stopping criterion

 eoSGA<Indi> gga(select, xover, CROSS_RATE, mutation, MUT_RATE,
                                   eval, continuator);
 // Apply algo to pop - that's it!
 gga(pop);
 
 // Print (sorted) intial population
 pop.sort();
 cout << "FINAL Population\n" << pop << endl;
}