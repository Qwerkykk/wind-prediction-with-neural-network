#include "Curve.h"

#include <utility>


Curve::Curve(std::vector<Point>  _curvePoints,std::string& id): components(std::move(_curvePoints)){
    this->id = id;
}


Curve::~Curve(){
}


std::string Curve::getID() const {
    return id;
}

const std::vector<DataVector> &Curve::getGridVector() const {
    return gridVectors;
}

void Curve::addGridVector(DataVector *_gridVector) {
    this->gridVectors.push_back(*_gridVector);
}

std::vector<Point> &Curve::getComponents() {
    return components;
}

std::vector<DataVector>& Curve::myContents() {
    return this->gridVectors;
}
void Curve::addPadToGridVector(int maxLength, double value){
    for(auto & gridVector : gridVectors){
        gridVector.pad(maxLength,value);
    }
}

int Curve::getDimension() {
    return components.size();
}

