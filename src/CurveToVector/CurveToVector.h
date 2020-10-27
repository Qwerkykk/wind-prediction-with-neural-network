#ifndef CURVEGRIDLSH_CURVETOVECTOR_H
#define CURVEGRIDLSH_CURVETOVECTOR_H

#include "../Grid/Grid.h"
#include "../Curve/Curve.h"

class CurveToVector {
private:
    std::vector<Grid> G;             //array of grids
    std::string id;
    int L;              //Number of k Grids
public:
    CurveToVector(int L, int dim, std::string id);
    ~CurveToVector();
    std::vector<double> * createGridCurveVector(std::vector<Point > v,int& index);
    void LshInsert(Curve& curve);
};


#endif //CURVEGRIDLSH_CURVETOVECTOR_H
