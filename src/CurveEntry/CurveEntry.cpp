#include "CurveEntry.h"

CurveEntry::CurveEntry( std::vector<Point> &components, DataVector *gridVector) : components(&components),
                                                                                 gridVector(gridVector) {}

DataVector& CurveEntry::myContents() {
    return *gridVector;
}

int CurveEntry::getDimension(){
    return gridVector->getDimension();
}

const std::vector<Point> &CurveEntry::getComponents() const {
    return *components;
}

const std::string &CurveEntry::getId() const {
    return gridVector->getId();
}