#ifndef FEATURES_H
#define FEATURES_H

#include <list>


using namespace std;

class Features{
    public:
        static double stdev(std::list<double>*, double); // standard deviation
        static double median(std::list<double>*); // the value in the middle
        static double mean(std::list<double>*); // avg
};
#endif
