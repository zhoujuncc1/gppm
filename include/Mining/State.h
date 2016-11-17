#ifdef STATE_H_
#define STATE_H_
struct State{
    State(State* src){
        paramset=src->paramset;
        prd_values = src->prd_values;
        time_values = src->time_values;
    }
    ~State(){
        paramset=NULL;
        delete prd_values;
        delete time_values;
    };
    void assignValues(){
        for(auto itr = prd_values.begin(); itr!=prd_values.end(); itr++){
            paramset->all_set[itr->first]->value = itr->second;
        }
        for(auto itr = time_values.begin(); itr!=time_values.end(); itr++){
            paramset->all_set[itr->first]->value = itr->second;
        }
    }
    ParameterSet* paramset;
    map<std::string, double> prd_values;
    map<std::string, int> time_values;
};
#endif // !STATE_H_
