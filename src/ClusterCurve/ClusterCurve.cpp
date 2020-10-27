#include "ClusterCurve.h"
#include <vector>
#include <iostream>
#include "../Point/Point.h"
#include "../Curve/Curve.h"


ClusterCurve::ClusterCurve() {
    centroid = nullptr;
}

void ClusterCurve::setCentroid(std::vector<Point> *_centroid) {
    centroid = _centroid;
}

std::vector<Point> *ClusterCurve::getCentroid() const {
    return centroid;
}

void ClusterCurve::addClusterCurve(Curve &clusterCurve, int secondClusterIndex) {
    clusterCurves.push_back(&clusterCurve);
    secondBestCluster.push_back(secondClusterIndex);
}

void ClusterCurve::printInfo() {
    std::cout << " num of elements :" << clusterCurves.size() << " with Centroid : ";
    for (Point component: *centroid) {
        std::cout << component.getx() << "," << component.gety() << " ";
    }
    std::cout << std::endl;

    std::cout << "================================================================\n";
}

void ClusterCurve::clearClusterCurves() {
    clusterCurves.clear();
}

const std::vector<Curve *> &ClusterCurve::getClusterCurves() const {
    return clusterCurves;
}

int &ClusterCurve::getSecondBestCLuster(int index) {
    return secondBestCluster[index];
}

int ClusterCurve::meanCurvesLength() {
    int length = 0;

    for (Curve *curve: clusterCurves) {
        length += curve->getDimension();
    }

    return length / (int) clusterCurves.size();
}

void ClusterCurve::clearCentroid() {
    centroid = nullptr;
}

const std::string &ClusterCurve::getCentroidId() const {
    return centroidId;
}

void ClusterCurve::setCentroid(std::vector<Point> *_centroid, std::string _id) {
    delete centroid;
    centroid = _centroid;
    centroidId = _id;
}

ClusterCurve::~ClusterCurve() {
    delete centroid;
}
