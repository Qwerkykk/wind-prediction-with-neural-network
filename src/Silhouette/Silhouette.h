#ifndef  SILHOUETTE_H
#define  SILHOUETTE_H

#include <vector>

class Cluster;

class DataVector;

class ClusterCurve;

std::string Silhouette(std::vector<Cluster> &clusters);

std::string SilhouetteCurve(std::vector<ClusterCurve> &clusters);


#endif // SILHOUETTE_H
