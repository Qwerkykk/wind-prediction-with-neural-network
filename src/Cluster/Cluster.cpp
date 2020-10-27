#include <chrono>
#include <iostream>

#include "Cluster.h"
#include "../DataVector/DataVector.h"



const std::vector<DataVector *> &Cluster::getClusterVectors() const {
    return clusterVectors;
}

void Cluster::setClusterVectors(const std::vector<DataVector *> &clusterVectors) {
    Cluster::clusterVectors = clusterVectors;
}

std::vector<double> *Cluster::getCentroid() const {
    return centroid;
}

void Cluster::setCentroid(std::vector<double> *_centroid) {
    delete centroid;
    centroid = _centroid;
}

Cluster::Cluster() {
    centroid = nullptr;
}

Cluster::~Cluster() {
    delete centroid;
}


void Cluster::clearClusterVectors() {
    clusterVectors.clear();
}

void Cluster::addClusterVector(DataVector& clusterVector, int secondClusterIndex) {
    clusterVectors.push_back(&clusterVector);
    secondBestCluster.push_back(secondClusterIndex);
}

void Cluster::printInfo() {
    std::cout << " num of elements :"<<clusterVectors.size() <<" with Centroid : ";
    for(double component: *centroid){
        std::cout<<component<<" ";
    }
    std::cout<<std::endl;



    std::cout<<"================================================================\n";
}


int& Cluster::getSecondBestCLuster(int index){
    return secondBestCluster[index];
}

void Cluster::setCentroid(std::vector<double> *_centroid, std::string id) {
    delete centroid;
    centroid = _centroid;
    centroidId = id;
}

const std::string &Cluster::getCentroidId() const {
    return centroidId;
}


