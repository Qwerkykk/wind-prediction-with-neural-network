#include <sstream>
#include <unistd.h>
#include <iostream>
#include <cassert>
#include <fstream>
#include <cfloat>
#include <cmath>


#include "FileUtils.h"
#include "../Utils/Utils.h"
#include "../DataVector/DataVector.h"
#include "../Config.h"
#include "../Cluster/Cluster.h"
#include "../Silhouette/Silhouette.h"
#include "../ClusterCurve/ClusterCurve.h"


void readVectorDataset(const std::string &fileName, std::vector<DataVector> &vectorDataset) {
    ASSERT(fileExist(fileName) && "File doesn't exist");
    ASSERT(vectorDataset.empty() && "vectorDataset is not empty");
    std::ifstream file;
    std::string line;
    std::string id;
    std::string _id;
    file.open(fileName);
    while (getline(file, line)) {
        auto *components = new std::vector<double>();
        std::istringstream buffer(line);

        buffer >> id;
        buffer >> _id;

        id = id + " " + _id;

        for (std::string word; buffer >> word;) {
            components->push_back(stof(word));
        }
        vectorDataset.emplace_back(id, *components);
    }
}

bool fileExist(const std::string &fileName) {
    return (access(fileName.c_str(), F_OK) != -1);
}




int readCurveDataset(const std::string &fileName, std::vector<Curve> &curveDataset) {
    ASSERT(fileExist(fileName) && "File doesn't exist");
    ASSERT(curveDataset.empty() && "vectorDataset is not empty");
    std::ifstream file;
    std::string line;
    std::string id;
    std::string pointQuantity;

    int maxPoints = 0;
    file.open(fileName);
    getline(file, line);
    while (getline(file, line)) {
        auto *components = new std::vector<Point>();
        std::istringstream buffer(line);

        buffer >> id;
        buffer >> pointQuantity;


        if (maxPoints < stoi(pointQuantity)) {
            maxPoints = stoi(pointQuantity);
        }

        for (std::string word; buffer >> word;) {

            std::string x, y;

            x = word.substr(1, word.size() - 2);

            buffer >> word;

            y = word.substr(0, word.size() - 2);

            components->emplace_back(atof(x.c_str()), atof(y.c_str()));
        }

        curveDataset.emplace_back(*components, id);

        delete components;

    }
    return maxPoints;
}


void exportVectorResults(std::vector<Cluster> &clusters, const std::string &initialization,
                         const std::string &assignment, const std::string &update, Config &configs) {

    std::fstream output;
    output.open(configs.outputFile, std::fstream::in | std::fstream::out | std::fstream::app);
    output << "\nAlgorithm: " << initialization + "_" + assignment + "_" + update << std::endl;

    int index = 1;
    for (Cluster &cluster: clusters) {
        output << "CLUSTER-" << index << " { size: " << cluster.getClusterVectors().size() << ", centroid: ";

        if (update == K_MEANS_UPDATE) {
            int i = 1;
            for (double component: *cluster.getCentroid()) {
                output << component;

                if (i == cluster.getCentroid()->size()) {
                    output << " }" << std::endl;
                } else {
                    output << ", ";
                }
                i++;
            }
        } else {
            output << cluster.getCentroidId() << " }" << std::endl;
        }

        index++;
    }

    output << Silhouette(clusters);

    if (configs.complete) {
        int index = 1;
        for (Cluster &cluster: clusters) {
            output << "CLUSTER-" + std::to_string(index) + " { ";


            for (DataVector *vec : cluster.getClusterVectors()) {
                output << vec->getId();

                if (cluster.getClusterVectors().at(cluster.getClusterVectors().size() - 1)->getId() == vec->getId())
                    output << " }\n";
                else
                    output << ", ";
            }
            index++;
        }
    }
    output.close();
}

void exportCurveResults(std::vector<ClusterCurve> &clusters, const std::string &initialization,
                        const std::string &assignment, const std::string &update, Config &configs) {
    std::fstream output;
    output.open(configs.outputFile, std::fstream::in | std::fstream::out | std::fstream::app);
    output << "\nAlgorithm: " << initialization + "_" + assignment + "_" + update << std::endl;

    int index = 1;
    for (ClusterCurve &cluster: clusters) {
        output << "CLUSTER-" << index << " { size: " << cluster.getClusterCurves().size() << ", centroid: ";

        if (update == DBA_UPDATE) {
            int i = 1;
            for (Point component: *cluster.getCentroid()) {
                output << "(" << component.x << "," << component.y << ") ";

                if (i == cluster.getCentroid()->size()) {
                    output << " }" << std::endl;
                } else {
                    output << ", ";
                }
                i++;
            }
        } else {
            output << cluster.getCentroidId() << " }" << std::endl;
        }

        index++;
    }

    output << SilhouetteCurve(clusters);

    if (configs.complete) {
        int index = 1;
        for (ClusterCurve &cluster: clusters) {
            output << "CLUSTER-" + std::to_string(index) + " { ";


            for (Curve *vec : cluster.getClusterCurves()) {
                output << vec->getID();

                if (cluster.getClusterCurves().at(cluster.getClusterCurves().size() - 1)->getID() == vec->getID())
                    output << " }\n";
                else
                    output << ", ";
            }
            index++;
        }
    }
    output.close();
}
