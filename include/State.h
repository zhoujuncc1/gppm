#ifndef STATE_H_
#define STATE_H_
#include "Bltl/BltlChecker.h"
#include "ParameterSet.h"
#include "Loss.h"
#include <map>
#include <sstream>
#include <string>

class Trajectory;
class State
{
  public:
    State(ParameterSet *params, Loss *loss)
    {
        paramset = params;
        this->loss = loss;
    }
    State(ParameterSet *params)
    {
        paramset = params;
        this->loss = new Loss();
    }
    State(State *src)
    {
        paramset = src->paramset;
        prd_values = src->prd_values;
        time_values = src->time_values;
        trajectories = src->trajectories;
        bltlChecker = src->bltlChecker;
        loss = src->loss;
    }
    ~State() { paramset = NULL; }
    void assignValues()
    {
        for (auto itr = prd_values.begin(); itr != prd_values.end(); itr++)
        {
            paramset->all_set[itr->first]->value = itr->second;
        }
        for (auto itr = time_values.begin(); itr != time_values.end(); itr++)
        {
            paramset->all_set[itr->first]->value = itr->second;
        }
    }

    std::string to_string()
    {
        std::stringstream ss;
        for (auto itr = prd_values.begin(); itr != prd_values.end(); itr++)
            ss << "\'" << itr->first << "\'"
               << ": " << itr->second << ", ";
        for (auto itr = time_values.begin(); itr != time_values.end(); itr++)
            ss << "\'" << itr->first << "\'"
               << ": " << itr->second << ", ";
        std::string str = ss.str();
        str.pop_back();
        return '{' + str + '}';
    }
    ParameterSet *paramset;
    std::map<std::string, double> prd_values;
    std::map<std::string, int> time_values;
    std::vector<Trajectory> *trajectories;
    BltlChecker *bltlChecker;
    Loss *loss;
};
#endif // !STATE_H_
