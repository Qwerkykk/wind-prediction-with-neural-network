
#ifndef  CLUSTERCURVE_H
#define  CLUSTERCURVE_H

#include <vector>
#include <string>
#include "../Point/Point.h"

class Curve;
class ClusterCurve {
private:
    std::vector<Curve *> clusterCurves;
    std::vector<Point> *centroid;
    std::vector<int> secondBestCluster;             //for every Datavector keep the second best min cluster (sihouette)
    std::string centroidId;
public:
    const std::string &getCentroidId() const;

private:
    int n;
    int k;
public:
    ClusterCurve();

    void setCentroid(std::vector<Point> *centroid);

    void setCentroid(std::vector<Point> * centroid,std::string id);

    std::vector<Point> *getCentroid() const;

    void addClusterCurve(Curve &clusterCurve, int secondClusterIndex);

    void printInfo();

    virtual ~ClusterCurve();

    void clearCentroid();

    void clearClusterCurves();

    int& getSecondBestCLuster(int index);

    const std::vector<Curve *> &getClusterCurves() const;

    int meanCurvesLength();
};

#endif // CLUSTERCURVE_H
