#ifndef LOSS_H_
#define LOSS_H_

#include <vector>

using namespace std;

#define BAYES_MIN 0.01
#define BAYES_MAX 100
#define RATE_MAX 0.9
#define LOSS_MAX 1000000

class State;

int count(vector<int> satArray);
double bayes_factor(vector<int> satArray, double theta,
    double alpha, double beta);

vector<int> modelchecking(State* state);

class Loss
{
public:
    virtual double operator() (State *state);
};

class Se_Loss:public Loss
{
  public:
    vector<int> labels;
    vector<int> class_count;
    int n_class;
    Se_Loss(vector<int> labels, vector<int> class_count);
    virtual double operator() (State *state);

};

#endif // !LOSS_H_
