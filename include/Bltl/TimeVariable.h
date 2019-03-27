#ifndef _TIME_VARIABLE_H_
#define _TIME_VARIABLE_H_

#include <iostream>
#include <unordered_set>
#include <utility>

#include "Parameter.h"

class TimeVariable : public Parameter {
public:
    TimeVariable();
    TimeVariable(char* str);
    int value;
    std::pair<int, int> range;
    double weight_sign;

private:
    std::string get_name();
    static std::unordered_set<std::string> pool;
};

#endif

// Local Variables:
// mode: c++
// End:
