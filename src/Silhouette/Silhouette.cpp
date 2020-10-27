#include <vector>
#include "../Cluster/Cluster.h"
#include "../DataVector/DataVector.h"
#include "../Manhattan.h"
#include "Silhouette.h"
#include "../ClusterCurve/ClusterCurve.h"
#include "../Curve/Curve.h"
#include "../Utils/DTW.h"


std::string Silhouette(std::vector<Cluster> &clusters){

    std::vector<double> silhouette;
    double total =0;

    for (int i = 0; i < clusters.size(); i++) {

        std::vector<DataVector *> clusterVectors = clusters[i].getClusterVectors();
        silhouette.push_back(0);
        for (int j = 0; j < clusterVectors.size(); j++) {
            double a = 0;
            for (int w = 0; w < clusterVectors.size(); w++) {
                a += ManhattanDistance(clusterVectors[w]->getComponents(),
                                       clusterVectors[j]->getComponents());
            }

            double b = 0;
            int secondCluster = clusters[i].getSecondBestCLuster(j);
            std::vector<DataVector *> secondClusterVectors = clusters[secondCluster].getClusterVectors();
            for (int w = 0; w < secondClusterVectors.size(); w++) {
                b += ManhattanDistance(secondClusterVectors[w]->getComponents(),
                                       clusterVectors[j]->getComponents());
            }
            if(b == 0 && a == 0){
                continue;
            } else if(b > a){
                silhouette[i] += (b - a) / b;
            } else{
                silhouette[i] += (b - a) / a;
            }
        }
        if(clusterVectors.size() != 0){
            silhouette[i] = silhouette[i]/(double)clusterVectors.size();
            total += silhouette[i];
        }
    }
    total = total / (double)clusters.size();

    std::string result = "Silhouette: [";
    for (int i = 0; i < clusters.size(); i++) {
        result+= std::to_string(silhouette[i]);
        result+= ", ";
    }
    result+= std::to_string( total);
    result+= " ]\n";
    return result;
}

std::string SilhouetteCurve(std::vector<ClusterCurve> &clusters) {

    std::vector<double> silhouette;
    double total =0;

    for (int i = 0; i < clusters.size(); i++) {

        std::vector<Curve *> clusterVectors = clusters[i].getClusterCurves();
        silhouette.push_back(0);
        for (int j = 0; j < clusterVectors.size(); j++) {
            double a = 0;
            for (int w = 0; w < clusterVectors.size(); w++) {
                a += DTW(clusterVectors[w]->getComponents(),
                                       clusterVectors[j]->getComponents());
            }

            double b = 0;
            int secondCluster = clusters[i].getSecondBestCLuster(j);
            std::vector<Curve *> secondClusterVectors = clusters[secondCluster].getClusterCurves();
            for (int w = 0; w < secondClusterVectors.size(); w++) {
                b += DTW(secondClusterVectors[w]->getComponents(),
                                       clusterVectors[j]->getComponents());
            }
            if(b == 0 && a == 0){
                continue;
            } else if(b > a){
                silhouette[i] += (b - a) / b;
            } else{
                silhouette[i] += (b - a) / a;
            }
        }
        if(clusterVectors.size() != 0){
            silhouette[i] = silhouette[i]/(double)clusterVectors.size();
            total += silhouette[i];
        }
    }
    total = total / (double)clusters.size();

    std::string result = "Silhouette: [";
    for (int i = 0; i < clusters.size(); i++) {
        result+= std::to_string(silhouette[i]);
        result+= ", ";
    }
    result+= std::to_string( total);
    result+= " ]\n";
    return result;
}
