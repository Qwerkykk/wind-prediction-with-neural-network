#ifndef PROJECT_FILEUTILS_H
#define PROJECT_FILEUTILS_H

#include <string>
#include <vector>
#include "../DataVector/DataVector.h"
#include "../Curve/Curve.h"
#include "../Config.h"

class Cluster;

class ClusterCurve;



void readVectorDataset(const std::string &fileName, std::vector<DataVector> &vectorDataset);

bool fileExist(const std::string &fileName);

int readCurveDataset(const std::string &fileName, std::vector<Curve> &curveDataset);

void exportVectorResults(std::vector<Cluster> &clusters, const std::string &initialization,
                         const std::string &assignment, const std::string &update, Config& configs);

void exportCurveResults(std::vector<ClusterCurve> &clusters, const std::string &initialization,
                        const std::string &assignment, const std::string &update,Config& configs);

#endif
