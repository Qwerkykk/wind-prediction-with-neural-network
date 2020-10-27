#ifndef CURVES_EUCLEIDIAN_H
#define CURVES_EUCLEIDIAN_H


#include <cmath>
#include <vector>

#include "../Point/Point.h"

struct {
    template<class T>
    double operator()(T n1,T n2) {
        double sum = 0;

        sum += pow((n1.getx() - n2.getx()), 2);
        sum += pow((n1.gety() - n2.gety()), 2);

        return sqrt(sum);
    }
}Euclidean;


#endif //CURVES_EUCLEIDIAN_H
