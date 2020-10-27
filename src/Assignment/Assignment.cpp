#include "Assignment.h"
#include "../Cluster/Cluster.h"
#include "../DataVector/DataVector.h"
#include "../Manhattan.h"
#include "../lshManhattan.h"
#include "../CurveGridInit/init.h"
#include "../Utils/DTW.h"
#include "../CurveEntry/CurveEntry.h"
#include "../CurveToVector/CurveToVector.h"
#include "../ClusterCurve/ClusterCurve.h"

double lloydAssignment(std::vector<Cluster> &clusters, std::vector<DataVector> &dataset) {
    int minClusterIndex, secondMinClusterIndex;
    double distance, minClusterDistance, secondMinClusterDistance, objectiveValue = 0;

    for (Cluster &cluster : clusters) {
        cluster.clearClusterVectors();
    }

    for (DataVector &vector : dataset) {
        minClusterIndex = 0;
        secondMinClusterIndex = -1;
        minClusterDistance = 99999999999;
        secondMinClusterDistance = 99999999999;

        int i = 0;
        for (Cluster &cluster: clusters) {
            distance = ManhattanDistance(vector.getComponents(), *cluster.getCentroid());
            if (distance < minClusterDistance) {
                minClusterDistance = distance;
                minClusterIndex = i;
            } else if (distance < secondMinClusterDistance) {
                secondMinClusterDistance = distance;
                secondMinClusterIndex = i;
            }
            i++;
        }
        clusters.at(minClusterIndex).addClusterVector(vector, secondMinClusterIndex);
        objectiveValue += minClusterDistance;
    }
    return objectiveValue;
}

int checkNeighbor(DataVector &item, std::vector<DataVector> &dataset) {
    for (int i = 0; i < dataset.size(); i++) {
        if (dataset[i].getComponents() == item.getComponents()) {
            return i;
        }
    }
    return -1;
}

int findunassignedItems(std::vector<int> &unassignedItems) {

    int counter = 0;
    for (int i = 0; i < unassignedItems.size(); i++) {
        if (unassignedItems[i] == 0) {
            counter++;
        }
    }
    return counter;
}

double radiusInit(std::vector<Cluster> &clusters) {

    double distance = 99999999999, temp;
    for (int i = 0; i < clusters.size(); i++) {
        for (int j = 0; j < clusters.size(); j++) {

            if (i == j) {
                continue;
            }

            temp = ManhattanDistance(*clusters[i].getCentroid(), *clusters[j].getCentroid());
            if (temp < distance) {
                distance = temp;
            }
        }
    }
    return distance / 2;
}

std::vector<int> &findSecondBestCluster(std::vector<Cluster> &clusters, std::vector<DataVector> &dataset,
                                        std::vector<int> &unassignedItems) {
    //for every vector find its second best min cluster
    std::vector<int> *secondBestClusters = new std::vector<int>();

    for (int i = 0; i < dataset.size(); i++) {
        double minDistance = 9999999999;
        int secondMinIndex = -1;
        for (int j = 0; j < clusters.size(); j++) {
            if (unassignedItems[i] == j) {
                continue;       //discard the first best option(real cluster)
            }
            double distance = ManhattanDistance(dataset[i].getComponents(), *clusters[j].getCentroid());
            if (distance < minDistance) {
                minDistance = distance;
                secondMinIndex = j;
            }
        }
        secondBestClusters->push_back(secondMinIndex);
    }
    return *secondBestClusters;
}


double rangeSearchAssignment(std::vector<Cluster> &clusters, std::vector<DataVector> &dataset) {

    int radius = radiusInit(clusters);
    double objectiveValue = 0;
    std::vector<double> minDistances;
    std::vector<int> unassignedItems(dataset.size(), -1);       // -1 = unassigned

    //repeat until #unassigned items < k
    while (findunassignedItems(unassignedItems) >= clusters.size()) {

        for (int i = 0; i < clusters.size(); i++) {

            lshManhattan<DataVector> *apprModel = new lshManhattan<DataVector>(5, 4, 5467);
            apprModel->fillStructures(dataset);


            DataVector q;
            q.setComponents(*clusters[i].getCentroid());
            q.setDimension((*clusters[i].getCentroid()).size());
            q.setId(std::to_string(i));

            std::vector<DataVector> query;
            query.push_back(q);

            std::vector<DataVector> neighbors;
            std::vector<double> neighborsDistances;

            apprModel->approximateNN(query, dataset, neighbors, &neighborsDistances, radius, 500, ManhattanDistance);

            for (int j = 0; j < neighbors.size(); j++) {

                //check if neighbor belongs to another cluster
                int index = checkNeighbor(neighbors[j], dataset);
                if (unassignedItems[index] != -1 && i != 0) {

                    //item belongs in many clusters
                    if (minDistances[index] > neighborsDistances[j]) {
                        objectiveValue = objectiveValue - minDistances[index] + neighborsDistances[j];
                        minDistances[index] = neighborsDistances[j];    //keep min distance
                    }
                    unassignedItems[index] = i;      //change cluster

                } else {   //unassigned item
                    objectiveValue += neighborsDistances[j];
                    minDistances[index] = neighborsDistances[j];
                    unassignedItems[index] = i;    //assign vector to cluster
                }
            }
        }
        radius = radius * 2;
    }


    //assign items with no cluster
    for (int i = 0; i < dataset.size(); i++) {
        if (unassignedItems[i] == -1) {

            double minDist = ManhattanDistance(dataset[i].getComponents(), *clusters[0].getCentroid());
            int minPos = 0;

            double currDistance;
            //scan clusters and find nearest
            for (int j = 1; j < clusters.size(); j++) {

                currDistance = ManhattanDistance(dataset[i].getComponents(), *clusters[j].getCentroid());
                if (currDistance < minDist) {
                    minDist = currDistance;
                    minPos = j;
                }
            }
            unassignedItems[i] = minPos;         //assign to cluster
            objectiveValue += minDist;
        }
    }

    std::vector<int> secondBestCluster = findSecondBestCluster(clusters, dataset, unassignedItems);

    //assign items to their final clusters
    for (int i = 0; i < dataset.size(); i++) {
        int clusterIndex = unassignedItems[i];
        clusters[clusterIndex].addClusterVector(dataset[i], secondBestCluster[i]);
    }
    return objectiveValue;
}

int checkNeighborCurve(CurveEntry &item, std::vector<CurveEntry> &dataset) {

    std::vector<Point> q = item.getComponents();

    for (int i = 0; i < dataset.size(); i++) {
        std::vector<Point> d = dataset[i].getComponents();

        int flag = 0;
        for (int j = 0; j < d.size(); j++) {
            if (d[j].getx() == q[j].getx() && d[j].gety() == q[j].gety()) {
                flag = 1;
            } else {
                flag = 0;
                break;
            }
        }
        if (flag == 1) {
            return i;
        }
    }
    return -1;
}

double radiusInitCurves(std::vector<ClusterCurve> &clusters) {

    double distance = 99999999999, temp;
    for (int i = 0; i < clusters.size(); i++) {
        for (int j = 0; j < clusters.size(); j++) {

            if (i == j) {
                continue;
            }

            temp = DTW(*clusters[i].getCentroid(), *clusters[j].getCentroid());
            if (temp < distance) {
                distance = temp;
            }
        }
    }
    return distance / 2;
}

std::vector<int> &findSecondBestClusterCurve(std::vector<ClusterCurve> &clusters, std::vector<Curve> &dataset,
                                             std::vector<int> &unassignedItems) {
    //for every vector find its second best min cluster
    std::vector<int> *secondBestClusters = new std::vector<int>();

    for (int i = 0; i < dataset.size(); i++) {
        double minDistance = 9999999999;
        int secondMinIndex = -1;
        for (int j = 0; j < clusters.size(); j++) {
            if (unassignedItems[i] == j) {
                continue;       //discard the first best option(real cluster)
            }
            double distance = DTW(dataset[i].getComponents(), *clusters[j].getCentroid());
            if (distance < minDistance) {
                minDistance = distance;
                secondMinIndex = j;
            }
        }
        secondBestClusters->push_back(secondMinIndex);
    }
    return *secondBestClusters;
}

double rangeSearchAssignCurves(std::vector<ClusterCurve> &clusters, std::vector<Curve> &dataset, int maxCurvePoints) {

    int radius = radiusInitCurves(clusters);
    double objectiveValue = 0;
    std::vector<double> minDistances(dataset.size(), 0);
    std::vector<int> unassignedItems(dataset.size(), -1);       // -1 = unassigned

    //repeat until #unassigned items < k
    while (findunassignedItems(unassignedItems) >= clusters.size()) {

        for (int i = 0; i < clusters.size(); i++) {

            CurveToVector **LSHDataset = init(1, dataset);

            lshManhattan<CurveEntry> *apprModel = new lshManhattan<CurveEntry>(1, 4, 14);        //approximate NN
            std::vector<CurveEntry> curveEntries;

            for (auto c_it = dataset.begin(); c_it != dataset.end(); c_it++) {
                c_it->addPadToGridVector(maxCurvePoints, 10000.0f);
            }

            for (auto c_it = dataset.begin(); c_it != dataset.end(); c_it++) {
                std::vector<Point> *points = &c_it->getComponents();
                std::vector<DataVector> *vectors = &c_it->myContents();

                for (auto v_it = vectors->begin(); v_it != vectors->end(); v_it++) {
                    curveEntries.emplace_back(*points, &v_it->myContents());
                }
            }

            apprModel->fillStructures(curveEntries);

            std::string id = std::to_string(i);
            Curve q(*clusters[i].getCentroid(), id);
            std::vector<Curve> query;
            query.push_back(q);

            CurveToVector **LSHQuery = init(1, query);

            std::vector<CurveEntry> queryEntries;

            for (auto c_it = query.begin(); c_it != query.end(); c_it++) {
                c_it->addPadToGridVector(maxCurvePoints, 10000.0f);
            }

            for (auto c_it = query.begin(); c_it != query.end(); c_it++) {
                std::vector<Point> *points = &c_it->getComponents();
                std::vector<DataVector> *vectors = &c_it->myContents();

                for (auto v_it = vectors->begin(); v_it != vectors->end(); v_it++) {
                    queryEntries.emplace_back(*points, &v_it->myContents());
                }
            }

            std::vector<CurveEntry> neighbors;
            std::vector<double> neighborsDistances;

            apprModel->approximateNN(queryEntries, curveEntries, neighbors, &neighborsDistances, radius, 500, DTW);

            for (int j = 0; j < neighbors.size(); j++) {

                //check if neighbor belongs to another cluster
                int index = checkNeighborCurve(neighbors[j], curveEntries);
                if (unassignedItems[index] != -1 && i != 0) {

                    //item belongs in many clusters
                    if (minDistances[index] > neighborsDistances[j]) {
                        objectiveValue = objectiveValue - minDistances[index] + neighborsDistances[j];
                        minDistances[index] = neighborsDistances[j];    //keep min distance
                    }
                    unassignedItems[index] = i;      //change cluster

                } else {   //unassigned item
                    objectiveValue += neighborsDistances[j];
                    minDistances[index] = neighborsDistances[j];
                    unassignedItems[index] = i;    //assign vector to cluster
                }
            }
        }
        radius = radius * 2;
    }

    //assign curves with no cluster
    for (int i = 0; i < dataset.size(); i++) {
        if (unassignedItems[i] == -1) {

            double minDist = DTW(dataset[i].getComponents(), *clusters[0].getCentroid());
            int minPos = 0;

            double currDistance;
            //scan clusters and find nearest
            for (int j = 1; j < clusters.size(); j++) {

                currDistance = DTW(dataset[i].getComponents(), *clusters[j].getCentroid());
                if (currDistance < minDist) {
                    minDist = currDistance;
                    minPos = j;
                }
            }
            unassignedItems[i] = minPos;         //assign to cluster
            objectiveValue += minDist;
        }
    }

    std::vector<int> secondBestCluster = findSecondBestClusterCurve(clusters, dataset, unassignedItems);

    //assign items to their final clusters
    for (int i = 0; i < dataset.size(); i++) {

        int clusterIndex = unassignedItems[i];
        clusters[clusterIndex].addClusterCurve(dataset[i], secondBestCluster[i]);
    }
    return objectiveValue;
}

double lloydAssignmentCurve(std::vector<ClusterCurve> &clusters, std::vector<Curve> &dataset, int i) {
    int minClusterIndex, secondMinClusterIndex;
    double distance, minClusterDistance, secondMinClusterDistance , objectiveValue = 0;

    for (ClusterCurve &cluster : clusters) {
        cluster.clearClusterCurves();
    }

    for (Curve &c : dataset) {
        minClusterIndex = 0;
        secondMinClusterIndex = -1;
        minClusterDistance = 99999999999;
        secondMinClusterDistance = 99999999999;

        int i = 0;
        for (ClusterCurve &cluster: clusters) {
            distance = DTW(c.getComponents(), *cluster.getCentroid());
            if (distance < minClusterDistance) {
                minClusterDistance = distance;
                minClusterIndex = i;
            } else if (distance < secondMinClusterDistance) {
                secondMinClusterDistance = distance;
                secondMinClusterIndex = i;
            }
            i++;
        }
        clusters.at(minClusterIndex).addClusterCurve(c, secondMinClusterIndex);
        objectiveValue += minClusterDistance;
    }
    return objectiveValue;
}
