#include <cmath>
#include <iostream>
#include <utility>
#include "DataVector.h"

DataVector::DataVector() {
}

DataVector::DataVector(std::string id, std::vector<double> &components) {
    this->id = std::move(id);
    this->components = &components;
    this->dimension = components.size();
}

DataVector::~DataVector() {
}


const std::string &DataVector::getId() const {
    return id;
}

void DataVector::setId(const std::string &id) {
    DataVector::id = id;
}

const std::vector<double> &DataVector::getComponents() const {
    return *components;
}


void DataVector::addComponent(double component) {
    this->components->push_back(component);
    dimension++;
}


void DataVector::setComponents( std::vector<double>& components) {
    this->components = &components;
}

void DataVector::setDimension(int dim) {
    DataVector::dimension = dim;
}

int DataVector::getDimension() {
    return dimension;
}

DataVector& DataVector::myContents() {
    return *this;
}
void DataVector::pad(int maxLength, double value){
    while(this->dimension < maxLength){
        addComponent(value);
    }
}

void DataVector::clear() {
        delete components;
}
