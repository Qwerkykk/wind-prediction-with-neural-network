#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <string>

class DataVector;

class Cluster {
private:
    std::vector<DataVector*> clusterVectors;
    std::vector<double>* centroid;
    std::vector<int> secondBestCluster;             //for every Datavector keep the second best min cluster (sihouette)
    std::string centroidId;
    int n;
    int k;

public:

    Cluster();

    ~Cluster();

    const std::vector<DataVector *> &getClusterVectors() const;

    void setClusterVectors(const std::vector<DataVector *> &clusterVectors);

    std::vector<double> *getCentroid() const;

    void setCentroid(std::vector<double> * centroid);

    void setCentroid(std::vector<double> * centroid,std::string id);

    const std::string &getCentroidId() const;

    void clearClusterVectors();

    void addClusterVector(DataVector& clusterVector,int secondClusterIndex);

    int& getSecondBestCLuster(int index);

    void printInfo();



};




#endif //CLUSTER_H
