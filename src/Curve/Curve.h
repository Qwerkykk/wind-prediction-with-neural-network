
#ifndef CURVEGRIDLSH_CURVE_H
#define CURVEGRIDLSH_CURVE_H

#include <string>
#include <vector>
#include "../DataVector/DataVector.h"
#include "../Point/Point.h"


class Curve {
private:
    std::vector<Point>  components;
    std::vector<DataVector> gridVectors;
    std::string id;

public:
    std::vector<Point> &getComponents();

    Curve(std::vector<Point> _curvePoints,std::string& id);
    ~Curve();

    std::string getID() const;
    const std::vector<DataVector> &getGridVector() const;
    int getDimension();
    void addGridVector(DataVector *gridVector);
    std::vector<DataVector>& myContents();
    void addPadToGridVector(int maxLength, double value);
};


#endif //CURVEGRIDLSH_CURVE_H
