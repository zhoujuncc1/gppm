#ifndef _TIME_VARIABLE_H_
#define _TIME_VARIABLE_H_

#include <string>
#include <unordered_set>
class TimeVariable:Parameter
{
public:
    TimeVariable();
    TimeVariable(char* str);
    int value;
    auto operator+=(const TimeVariable &rhs) -> TimeVariable &;
private:
    auto get_name() -> std::string;
    friend auto to_string(const TimeVariable &v) -> std::string;
    static std::unordered_set<std::string> pool;
};

auto operator+(TimeVariable lhs, const TimeVariable &rhs) -> TimeVariable;
auto operator<<(std::ostream &os, const TimeVariable &v) -> std::ostream &;

#endif

// Local Variables:
// mode: c++
// End:
