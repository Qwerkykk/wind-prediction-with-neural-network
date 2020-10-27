#include "Update.h"
#include "../Cluster/Cluster.h"
#include "../DataVector/DataVector.h"
#include "../Manhattan.h"
#include "../ClusterCurve/ClusterCurve.h"
#include "../Curve/Curve.h"
#include "../Utils/DTW.h"


void meanVectorUpdate(std::vector<Cluster> &clusters, std::vector<DataVector> &dataset) {


    for (auto cluster = clusters.begin(); cluster != clusters.end(); cluster++) {


        if(cluster->getClusterVectors().size() == 0){
            //if cluster is empty pick a random centroid
            srand(time(NULL));
            int flag = 0, index;
            while (flag == 0) {
                index = rand() % dataset.size();
                flag = 1;
                for (int i = 0; i < clusters.size(); i++) {         //check if centroid already exists(as centroid)
                    if (ManhattanDistance(dataset[index].getComponents(),*clusters[i].getCentroid()) < 0.003) {
                        flag = 0;
                        break;
                    }
                }
            }
            std::vector<double> components = dataset.at(index).getComponents();
            auto *centroid = new std::vector<double>;
            for (double component: components) {
                centroid->push_back(component);
            }
            cluster->setCentroid(centroid);

            std::cout<<"IF: ";
            for(double comp: *centroid){
                std::cout<<" " << comp;
            }
            std::cout<<std::endl;
        }else{
            auto *currentCentroid = new std::vector<double>;

            for (int i = 0; i < cluster->getClusterVectors().at(0)->getComponents().size(); i++) {
                double sum = 0;

                for (DataVector *vector: cluster->getClusterVectors()) {
                    sum += vector->getComponents().at(i);
                }
                currentCentroid->push_back(sum / cluster->getClusterVectors().size());
            }

            cluster->setCentroid(currentCentroid);

        }

    }

}

void DBAUpdate(std::vector<ClusterCurve> &clusters, std::vector<Curve> &dataset) {
    srand(time(NULL));
    for (auto cluster = clusters.begin(); cluster < clusters.end(); cluster++) {
        if (cluster->getClusterCurves().size() == 0) {
            cluster->setCentroid(&dataset.at(rand() % dataset.size()).getComponents());
            continue;
        }
        int length = cluster->meanCurvesLength();
        std::vector<int> positionOfVectorWithLength;

        int index = 0;
        for (Curve *curve: cluster->getClusterCurves()) {
            if (curve->getDimension() >= length)
                positionOfVectorWithLength.push_back(index);
            index++;
        }

        index = rand() % positionOfVectorWithLength.size();

        int subSequencePos;
        if (cluster->getClusterCurves().at(positionOfVectorWithLength.at(index))->getDimension() == length)
            subSequencePos = 0;
        else
            subSequencePos = rand() %
                             (cluster->getClusterCurves().at(positionOfVectorWithLength.at(index))->getDimension() -
                              length);


        std::vector<Point>::const_iterator first =
                cluster->getClusterCurves().at(positionOfVectorWithLength.at(index))->getComponents().begin() +
                subSequencePos;

        std::vector<Point>::const_iterator last = cluster->getClusterCurves().at(
                positionOfVectorWithLength.at(index))->getComponents().end();

        auto *C = new std::vector<Point>(first, first + length);

        std::vector<Point> C_temp;
        C_temp.emplace_back(999999999, 99999999);

        while (DTW(*C, C_temp) > 10) {

            C_temp.clear();
            C_temp = *C;

            auto *A = new std::vector<std::vector<Point> *>;

            for (int j = 0; j < length; j++) {
                A->push_back(new std::vector<Point>);
            }

            for (Curve *clusterCurve: cluster->getClusterCurves()) {
                std::vector<Position> *path = DTW.findPath(*C, clusterCurve->getComponents());
                for (Position pos: *path) {
                    A->at(pos.x)->push_back(clusterCurve->getComponents().at(pos.y));
                }
                delete path;
            }

            int i = 0;
            for (auto pairUnion = A->begin(); pairUnion < A->end(); pairUnion++) {
                double x = 0, y = 0;
                for (auto pair = (*pairUnion)->begin(); pair < (*pairUnion)->end(); pair++) {
                    x += pair->x;
                    y += pair->y;
                }


                C->at(i).x = x / (*pairUnion)->size();
                C->at(i).y = y / (*pairUnion)->size();
                i++;
            }
            delete A;
        }

        cluster->setCentroid(C);
    }
}

void pamLloydUpdate(std::vector<Cluster> &clusters, std::vector<DataVector> &dataset) {

    double currDistance, tempDistance, minDistance = 99999999999;
    int newMinClusterIndex;

    for (int i = 0; i < clusters.size(); i++) {
        //update each cluster with its medoid
        newMinClusterIndex = -1;
        std::vector<DataVector *> clusterVectors = clusters[i].getClusterVectors();
        for (int j = 0; j < clusterVectors.size(); j++) {

            tempDistance = 0;
            for (int w = 0; w < clusterVectors.size(); w++) {

                if (j == w) {
                    continue;   //don't check with itself
                }
                currDistance = ManhattanDistance(clusterVectors[j]->getComponents(),
                                                 clusterVectors[w]->getComponents());
                tempDistance += currDistance;
            }
            // search curves_clusteringcurves_clusteringwhere the sum of distances in the cluster is minimized
            tempDistance = tempDistance / (double) clusterVectors.size();     //average distance
            if (j == 0) {
                minDistance = tempDistance;
                newMinClusterIndex = j;
            } else if (minDistance > tempDistance) {
                minDistance = tempDistance;
                newMinClusterIndex = j;
            }

        }
        //set new medoid
        if (newMinClusterIndex != -1) {

            std::vector<double> components = clusterVectors[newMinClusterIndex]->getComponents();
            auto *centroid = new std::vector<double>;
            for (double component: components) {
                centroid->push_back(component);
            }
            clusters[i].setCentroid(centroid,clusterVectors[newMinClusterIndex]->getId());
        } else {
            //if cluster is empty pick a random centroid
            srand(time(NULL));
            int flag = 0, index;
            while (flag == 0) {
                index = rand() % dataset.size();
                flag = 1;
                for (int i = 0; i < clusters.size(); i++) {         //check if centroid already exists(as centroid)
                    if (ManhattanDistance(dataset[index].getComponents(),*clusters[i].getCentroid()) < 0.003) {
                        flag = 0;
                        break;
                    }
                }
            }
            std::vector<double> components = dataset.at(index).getComponents();
            auto *centroid = new std::vector<double>;
            for (double component: components) {
                centroid->push_back(component);
            }
            clusters[i].setCentroid(centroid,dataset.at(index).getId());
        }
    }
}

void pamLloydUpdateCurve(std::vector<ClusterCurve> &clusters, std::vector<Curve> &dataset) {

    double currDistance, tempDistance, minDistance = 99999999999;
    int newMinClusterIndex;

    for (int i = 0; i < clusters.size(); i++) {

        //update each cluster with its medoid
//        clusters[i].clearCentroid();
        newMinClusterIndex = -1;
        std::vector<Curve *> clusterCurves = clusters[i].getClusterCurves();
        for (int j = 0; j < clusterCurves.size(); j++) {
            tempDistance = 0;
            for (int w = 0; w < clusterCurves.size(); w++) {

                if (j == w) {
                    continue;   //don't check with itself
                }
                currDistance = DTW(clusterCurves[j]->getComponents(),
                                   clusterCurves[w]->getComponents());
                tempDistance += currDistance;
            }
//            tempDistance = tempDistance / (double) clusterCurves.size();     //average distance
            if (j == 0) {
                minDistance = tempDistance;
                newMinClusterIndex = j;
            } else if (minDistance > tempDistance) {
                minDistance = tempDistance;
                newMinClusterIndex = j;
            }
        }
        //set new medoid
        if (newMinClusterIndex != -1) {
//            std::cout << "Cluster " << i << " updated" << std::endl;
            std::vector<Point> components = clusterCurves[newMinClusterIndex]->getComponents();
            auto *centroid = new std::vector<Point>;
            for (Point component: components) {
                double x = component.getx();
                double y = component.gety();
                Point newComp(x, y);
                centroid->push_back(newComp);
            }
            clusters[i].setCentroid(centroid,clusterCurves[newMinClusterIndex]->getID());
        } else {
            //if cluster is empty pick a random centroid
            srand(time(NULL));
            int flag = 0, index;
            while (flag == 0) {
                index = rand() % dataset.size();
                flag = 1;
                for (int z = 0; z < clusters.size(); z++) {        //check if centroid already exists(as centroid)
                    if(i == z) continue;
                    double temp = DTW(dataset[index].getComponents(),
                                      *clusters[z].getCentroid());
                    if( temp < 0.002){
                        flag = 0;
                        break;
                    }
                }
            }
            std::vector<Point> components = dataset[index].getComponents();
            auto *centroid = new std::vector<Point>;
            for (Point component: components) {
                double x = component.getx();
                double y = component.gety();
                Point newComp(x, y);
                centroid->push_back(newComp);
            }
            clusters[i].setCentroid(centroid,dataset[index].getID());
        }
    }
}