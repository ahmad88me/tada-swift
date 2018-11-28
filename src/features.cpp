#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <list>
#include <set>
#include <numeric>
#include "features.h"

using namespace std;

 double Features::stdev(std::list<double>* values, double mean_value){
    int i;
    double numerator_sum = 0;
    double diff;
    double std_value;
    for(auto it=values->cbegin(); it!=values->cend();it++){
        diff = (*it) - mean_value;
        numerator_sum+=  diff*diff;
    }

    if(values->size()==0){
        std_value = 0;
    }
    else{
        std_value = numerator_sum/values->size();
        std_value = sqrt(std_value);
    }

    /*
    std_value = numerator_sum/values->size();
    std_value = sqrt(std_value);
    */
    return std_value;
}

 double Features::mean(std::list<double>* values){

    if(values->size()==0){
        return 0;
    }

    return std::accumulate(values->cbegin(), values->cend(), 0)/values->size();
}

 double Features::median(std::list<double>* values){
    bool even;
    int i=0;
    even = values->size()%2==0;
    values->sort();

    if(values->size()==0){
        return 0;
    }

    if(even){
        for(auto it=values->cbegin();it!=values->cend();it++, i++){
            if(i==values->size()/2-1){
                return ((*it) + *(++it))/2;
            }
        }
    }
    else{
        for(auto it=values->cbegin();it!=values->cend();it++, i++){
            if(i==values->size()/2){
                return (*it);
            }
        }
    }
    return 0;// just for testing
}
