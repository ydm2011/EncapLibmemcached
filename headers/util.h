#ifndef UTIL_H
#define UTIL_H
#include <ctime>
class TimeStatcis
{
public:
    TimeStatcis(const char* test_term);
    ~TimeStatcis();
private:
    time_t time_eclipse;
    const char* test_term;
    time_t time_start;
};
#endif // UTIL_H
