#ifndef _TIME_VARIABLE_H_
#define _TIME_VARIABLE_H_

#include "Parameter.h"
#include <string>
#include <unordered_set>
class TimeVariable: Parameter
{
public:
    TimeVariable();
    TimeVariable(char* str);
    int value;
	std::pair<int, int> range;
private:
    std::string get_name();
    static std::unordered_set<std::string> pool;
};



#endif

// Local Variables:
// mode: c++
// End:
