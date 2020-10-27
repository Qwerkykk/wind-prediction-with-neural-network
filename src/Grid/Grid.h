#ifndef CURVEGRIDLSH_GRID_H
#define CURVEGRIDLSH_GRID_H

#include <vector>
#include "../Curve/Curve.h"

class Grid {
private:
    std::vector<double> t;  //(real) vector t uniformly ∈ [0, d)^δ
    double d;               //δ
public:
    Grid(double d);
    ~Grid();
    std::vector<double>* createGridCurve(std::vector<Point>& v);

    const std::vector<double> &getT() const;
};



#endif //CURVEGRIDLSH_GRID_H
