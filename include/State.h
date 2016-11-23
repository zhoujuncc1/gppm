#ifndef STATE_H_
#define STATE_H_
#include "ParameterSet.h"
#include <map>
#include <string>
#include <sstream>

class Trajectory;
class State{
public:
    State(ParameterSet* params){
        paramset=params;
    }
    State(State* src){
        paramset=src->paramset;
        prd_values = src->prd_values;
        time_values = src->time_values;
        trajectories = src->trajectories;
    }
    ~State(){
        paramset=NULL;
    }
    void assignValues(){
        for(auto itr = prd_values.begin(); itr!=prd_values.end(); itr++){
            paramset->all_set[itr->first]->value = itr->second;
        }
        for(auto itr = time_values.begin(); itr!=time_values.end(); itr++){
            paramset->all_set[itr->first]->value = itr->second;
        }
    }

    std::string to_string() {
	    std::stringstream ss;
	    ss<<"{";
	    for(auto itr=prd_values.begin();itr!=prd_values.end();itr++)
	        ss<<itr->first<<": "<<itr->second<<", ";
	    for(auto itr=time_values.begin();itr!=time_values.end();itr++)
	        ss<<itr->first<<": "<<itr->second<<", ";
	    ss<<"}";
	    return ss.str();
	}
    ParameterSet* paramset;
    std::map<std::string, double> prd_values;
    std::map<std::string, int> time_values;
    std::vector<Trajectory> * trajectories;
};
#endif // !STATE_H_
