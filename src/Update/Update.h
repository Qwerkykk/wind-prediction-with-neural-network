#ifndef  UPDATE_H
#define  UPDATE_H

#include <vector>

class Cluster;

class Curve;

class ClusterCurve;

class DataVector;

//Return false if there wasn't any update else true
void meanVectorUpdate(std::vector<Cluster> &clusters, std::vector<DataVector> &dataset);

void pamLloydUpdate(std::vector<Cluster> &clusters, std::vector<DataVector> &dataset);

void pamLloydUpdateCurve(std::vector<ClusterCurve> &clusters, std::vector<Curve> &dataset);

void DBAUpdate(std::vector<ClusterCurve> &clusters,std::vector<Curve>& dataset);

#endif // UPDATE_H
