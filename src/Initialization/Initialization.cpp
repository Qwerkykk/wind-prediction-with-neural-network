#include "Initialization.h"
#include "../Manhattan.h"
#include "../DataVector/DataVector.h"
#include "../Curve/Curve.h"
#include "../Point/Point.h"
#include "../Cluster/Cluster.h"
#include "../ClusterCurve/ClusterCurve.h"
#include "../Utils/DTW.h"
#include <ctime>
#include <algorithm>
#include <random>
#include <chrono>

void randomInitialization(std::vector<Cluster> &clusters, std::vector<DataVector> &dataset) {
    int index,flag;
    srand(time(NULL));
    std::vector<int> visited;

    for (auto it = clusters.begin(); it < clusters.end(); it++) {
        auto *centroid = new std::vector<double>;
        flag = 0;
        while (flag == 0) {

            index = rand() % dataset.size();
            if (std::find(visited.begin(), visited.end(), index) != visited.end()) {
                flag = 0;           //this centroid has been already choosen
            } else {
                flag = 1;
                visited.push_back(index);
            }
            std::vector<double> components = dataset.at(index).getComponents();


            for (double component: components) {
                centroid->push_back(component);
            }

            it->setCentroid(centroid,dataset.at(index).getId());


        }
    }
}

//search in the partial sums table
int myBinarySearch(std::vector<std::vector<double> > &P, double x) {

    int middle, left = 0, right = P.size();
    while (1) {

        middle = (left + right) / 2;

        if (P[middle - 1][0] < x && P[middle][0] >= x) {
            return P[middle][1];
        }

        if (P[middle][0] < x && P[middle + 1][0] >= x) {
            return P[middle + 1][1];
        }

        if (right - 1 == left) {
            return P[right][1];
        }

        if (P[middle][0] < x) {
            left = middle;
        } else {
            right = middle;
        }
    }
}

void kmeansPlusPlusInit(std::vector<Cluster> &clusters, std::vector<DataVector> &dataset) {

    srand(time(NULL));
    std::vector<int> centersIndexes;
    std::vector<double> minDistances;
    double currDistance, minDist, temp;

    //pick initial cluster randomly
    int index = rand() % dataset.size();
    std::vector<double> components = dataset.at(index).getComponents();

    auto *centroid = new std::vector<double>;
    for (double component: components) {
        centroid->push_back(component);
    }
    clusters[0].setCentroid(centroid,dataset.at(index).getId());
    centersIndexes.push_back(index);            //mark as visited

    //calculate initial distances - dist(item, initial cluster)
    for (int i = 0; i < dataset.size(); i++) {
        currDistance = ManhattanDistance(dataset[i].getComponents(), *clusters[0].getCentroid());
        minDistances.push_back(currDistance);
    }

    for (int i = 1; i < clusters.size(); i++) {

        std::vector<std::vector<double>> P;     //keep partial sums
        for (int j = 0; j < dataset.size(); j++) {
            P.push_back(std::vector<double>());
        }
        P[0].push_back(0);


        for (int w = 0; w < dataset.size(); w++) {

            std::vector<int>::iterator it;
            it = find(centersIndexes.begin(), centersIndexes.end(), w);
            if (it != centersIndexes.end()) {     //element is cluster
                P[w + 1].push_back(P[w][0]);     //add previous partial sum
                P[w + 1].push_back(P[w][1]);     //add previous position
            } else {
                minDist = minDistances[w];

                if (i > 1) {   //if previous cluster is closer to the current item
                    currDistance = ManhattanDistance(dataset[w].getComponents(), *clusters[i - 1].getCentroid());
                    if (currDistance < minDist) {
                        minDist = currDistance;
                        minDistances[w] = minDist;
                    }
                }

                temp = pow(minDist, 2);
                temp += P[w][0];

                P[w + 1].push_back(temp);
                P[w + 1].push_back(P[w][1] + 1);
            }
        }

        //Pick a uniformly distributed float x ∈ [0, P (n − t )]
        std::uniform_real_distribution<double> uniformDist(0, P[dataset.size()][0]);
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        double x = uniformDist(generator);

        if (x == 0) {
            index = P[1][1];
        } else {
            //find new cluster position - binary search
            index = myBinarySearch(P, x);
        }

        components = dataset.at(index).getComponents();
        auto *centroid2 = new std::vector<double>;
        for (double component: components) {
            centroid2->push_back(component);
        }
        clusters[i].setCentroid(centroid2,dataset.at(index).getId());
        centersIndexes.push_back(index);            //mark as visited


    }
}


void randomInitializationCurve(std::vector<ClusterCurve> &clusters, std::vector<Curve> &dataset) {

    int index, flag;
    srand(time(NULL));
    std::vector<int> visited;

    for (auto it = clusters.begin(); it < clusters.end(); it++) {
        auto *centroid = new std::vector<Point>;
        flag = 0;
        while (flag == 0) {

            index = rand() % dataset.size();
            if (std::find(visited.begin(), visited.end(), index) != visited.end()) {
                flag = 0;           //this centroid has been already choosen
            } else {
                flag = 1;
                visited.push_back(index);
            }
            std::vector<Point> components = dataset.at(index).getComponents();

            for (Point component: components) {
                double x = component.getx();
                double y = component.gety();
                Point newComp(x, y);
                centroid->push_back(newComp);
            }
            it->setCentroid(centroid,dataset.at(index).getID());


        }
    }
}

void kmeansPlusPlusInitCurve(std::vector<ClusterCurve> &clusters, std::vector<Curve> &dataset) {

    srand(time(NULL));
    std::vector<int> centersIndexes;
    std::vector<double> minDistances;
    double currDistance, minDist, temp;

    //pick initial cluster randomly
    int index = rand() % dataset.size();
    std::vector<Point> components = dataset.at(index).getComponents();

    auto *centroid = new std::vector<Point>;
    for (Point component: components) {
        double x = component.getx();
        double y = component.gety();
        Point newComp(x, y);
        centroid->push_back(newComp);
    }
    clusters[0].setCentroid(centroid);
    centersIndexes.push_back(index);            //mark as visited

    //calculate initial distances - dist(item, initial cluster)
    for (int i = 0; i < dataset.size(); i++) {
        currDistance = DTW(dataset[i].getComponents(), *clusters[0].getCentroid());
        minDistances.push_back(currDistance);
    }

    for (int i = 1; i < clusters.size(); i++) {

        std::vector<std::vector<double>> P;     //keep partial sums
        for (int j = 0; j < dataset.size(); j++) {
            P.push_back(std::vector<double>());
        }
        P[0].push_back(0);

        for (int w = 0; w < dataset.size(); w++) {

            std::vector<int>::iterator it;
            it = find(centersIndexes.begin(), centersIndexes.end(), w);
            if (it != centersIndexes.end()) {     //element is cluster
                P[w + 1].push_back(P[w][0]);     //add previous partial sum
                P[w + 1].push_back(P[w][1]);     //add previous position
            } else {
                minDist = minDistances[w];

                if (i > 1) {   //if previous cluster is closer to the current item
                    currDistance = DTW(dataset[w].getComponents(), *clusters[i - 1].getCentroid());
                    if (currDistance < minDist) {
                        minDist = currDistance;
                        minDistances[w] = minDist;
                    }
                }

                temp = pow(minDist, 2);
                temp += P[w][0];

                P[w + 1].push_back(temp);
                P[w + 1].push_back(P[w][1] + 1);
            }
        }

        //Pick a uniformly distributed float x ∈ [0, P (n − t )]
        std::uniform_real_distribution<double> uniformDist(0, P[dataset.size()][0]);
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        double x = uniformDist(generator);

        if (x == 0) {
            index = P[1][1];
        } else {
            //find new cluster position - binary search
            index = myBinarySearch(P, x);
        }

        components = dataset.at(index).getComponents();
        auto *centroid2 = new std::vector<Point>;
        for (Point component: components) {
            double x = component.getx();
            double y = component.gety();
            Point newComp(x, y);
            centroid2->push_back(newComp);
        }
        clusters[i].setCentroid(centroid2,dataset.at(index).getID());
        centersIndexes.push_back(index);            //mark as visited
    }
}