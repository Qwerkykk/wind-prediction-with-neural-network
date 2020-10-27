#ifndef  INITIALIZATION_H
#define  INITIALIZATION_H

#include <vector>

class Cluster;

class ClusterCurve;

class DataVector;

class Curve;

void randomInitialization(std::vector<Cluster> &clusters, std::vector<DataVector> &dataset);

void kmeansPlusPlusInit(std::vector<Cluster> &clusters, std::vector<DataVector> &dataset);

void randomInitializationCurve(std::vector<ClusterCurve> &clusters, std::vector<Curve> &dataset);

void kmeansPlusPlusInitCurve(std::vector<ClusterCurve> &clusters, std::vector<Curve> &dataset);

#endif // INITIALIZATION_H
