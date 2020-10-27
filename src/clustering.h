
#ifndef CLUSTERING_H
#define CLUSTERING_H

#include <vector>
#include <cmath>
#include "Config.h"
#include "DataVector/DataVector.h"
#include "Cluster/Cluster.h"
#include "FileUtils/FileUtils.h"

template<typename I, typename A, typename U>
void clustering(std::vector<DataVector> &dataset, Config &configs, I initFunction, A assignFunction, U updateFunction,
                const std::string &initialization,
                const std::string &assignment, const std::string &update) {

    int loops = 0;
    double previousObj, currentObj;
    std::vector<Cluster> clusters;

    for (int i = 0; i < configs.numberOfCluster; i++) {
        clusters.emplace_back(Cluster());
    }

    initFunction(clusters, dataset);
    currentObj = assignFunction(clusters, dataset);
    updateFunction(clusters, dataset);

    do {
        previousObj = currentObj;

        currentObj = assignFunction(clusters, dataset);
        updateFunction(clusters, dataset);

        loops++;

    } while (loops < 50 && fabs(previousObj - currentObj) > 0.005);

    exportVectorResults(clusters, initialization, assignment, update, configs);

}

template<typename I, typename A, typename U>
void
clusteringCurves(std::vector<Curve> &dataset, int maxCurvePoints, Config &configs, I initFunction, A assignFunction,
                 U updateFunction,
                 const std::string &initialization,
                 const std::string &assignment, const std::string &update) {

    int loops = 0;
    double previousObj, currentObj;
    std::vector<ClusterCurve> clusters;

    for (int i = 0; i < configs.numberOfCluster; i++) {
        clusters.emplace_back(ClusterCurve());
    }

    initFunction(clusters, dataset);
    currentObj = assignFunction(clusters, dataset, maxCurvePoints);
    updateFunction(clusters, dataset);

    do {
        previousObj = currentObj;

        currentObj = assignFunction(clusters, dataset, maxCurvePoints);
        updateFunction(clusters, dataset);

        loops++;

    } while (loops < 20 && fabs(previousObj - currentObj) > 0.005);

    exportCurveResults(clusters, initialization, assignment, update, configs);
}

#endif //CLUSTERING_H
