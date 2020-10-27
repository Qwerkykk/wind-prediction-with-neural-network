#ifndef CURVEGRIDLSH_CURVEENTRY_H
#define CURVEGRIDLSH_CURVEENTRY_H


#include "../DataVector/DataVector.h"
#include "../Point/Point.h"

class CurveEntry {
private:
    std::vector<Point> *components;
    DataVector *gridVector;
public:
    CurveEntry(std::vector<Point> &components, DataVector *gridVector);

    DataVector &myContents();

    int getDimension();

    const std::vector<Point> &getComponents() const;

    const std::string &getId() const;
};


#endif //CURVEGRIDLSH_CURVEENTRY_H
