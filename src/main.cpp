#include <iostream>
#include <fstream>
#include <sstream>
#include "DataVector/DataVector.h"
#include "Curve/Curve.h"
#include "FileUtils/FileUtils.h"
#include "Cluster/Cluster.h"
#include "ClusterCurve/ClusterCurve.h"
#include "Initialization/Initialization.h"
#include "Assignment/Assignment.h"
#include "Update/Update.h"
#include "Silhouette/Silhouette.h"
#include "Utils/DTW.h"
#include "Config.h"
#include "clustering.h"
#include "Utils/Utils.h"


//VECTOR Args: -i ../Dataset/Ex2_Datasets/DataVectors_10_10000x100.csv -o outputFile.txt -c ../cluster.conf

//CURVE Args: -i ../Dataset/curves_clustering/input_projection6.csv -o outputFile.txt -c ../cluster.conf


int main(int argc, char **argv) {

    Config configs;

    if (argc != 7 && argc != 8) {
        std::cout << "WRONG NUMBER OF ARGUMENTS" << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if ((std::string) argv[i] == "-i")
            configs.inputFile = argv[i + 1];
        if ((std::string) argv[i] == "-c")
            configs.configurationFile = argv[i + 1];
        if ((std::string) argv[i] == "-o")
            configs.outputFile = argv[i + 1];
        if ((std::string) argv[i] == "-complete")
            configs.complete = true;
    }

    std::cout << "The dataset is consisted by vectors or curves?:";
    std::string answer;
    std::cin >> answer;
    while (answer != "vectors" && answer != "curves") {
        std::cout << "Wrong Answer! Type vectors or curves to choose: \n";
        std::cin >> answer;
    }

    std::ifstream file;
    std::string line;
    std::string field;
    std::string value;

    file.open(configs.configurationFile);
    while (getline(file, line)) {

        std::istringstream buffer(line);
        buffer >> field;
        buffer >> value;

        if (field == "number_of_clusters:")
            configs.numberOfCluster = stoi(value);

        if (field == "number_of_grids:")
            configs.numberOfGrids = stoi(value);

        if (field == "number_of_vector_hash_table:")
            configs.numberOfVectorHashTables = stoi(value);

        if (field == "number_of_vector_hash_functions:")
            configs.numberOfVectorHashFunction = stoi(value);
    }

    if (answer == "vectors") {

        /// Menu option ///

        std::cout << "---------------------------------------------------\n"
                  << "Initialization: 1) Random selection of k points  2) K-means++\n"
                  << "Assignment:     1) Lloyd's                       2) Range Search with LSH\n"
                  << "Update:         1) PAM a la Lloyd                2) Mean Vector\n"
                  << "---------------------------------------------------\n";
        std::cout << "Select Initialization algorithm ";
        std::string init;
        std::cin >> init;
        std::cout << "Select Assignment algorithm ";
        std::string assi;
        std::cin >> assi;
        std::cout << "Select Update algorithm ";
        std::string upd;
        std::cin >> upd;
        int in = std::stoi(init);
        int as = std::stoi(assi);
        int up = std::stoi(upd);

        std::vector<DataVector> vectorDataset;
        readVectorDataset(configs.inputFile, vectorDataset);


        if (in == 1 && as == 1 && up == 2) {

            // RandomInitialization_LLoydAssignment_MeanVector      1-1-2
            clustering(vectorDataset, configs, randomInitialization, lloydAssignment, meanVectorUpdate,
                       RANDOM_INITIALIZATION, LLOYD_ASSIGNMENT, K_MEANS_UPDATE);
        } else if (in == 1 && as == 1 && up == 1) {

            // RandomInitialization_LLoydAssignment_PamLloydUpdate     1-1-1
            clustering(vectorDataset, configs, randomInitialization, lloydAssignment, pamLloydUpdate,
                       RANDOM_INITIALIZATION, LLOYD_ASSIGNMENT, PAM_LLOYD_UPDATE);
        } else if (in == 1 && as == 2 && up == 2) {

            // RandomInitialization__RangeSearchAssignment_MeanVectorUpdate     1-2-2
            clustering(vectorDataset, configs, randomInitialization, rangeSearchAssignment, meanVectorUpdate,
                       RANDOM_INITIALIZATION, RANGE_SEARCH_ASSIGNMENT, K_MEANS_UPDATE);
        } else if (in == 1 && as == 2 && up == 1) {

            // RandomInitialization__RangeSearchAssignment_PamLloydUpdate       1-2-1
            clustering(vectorDataset, configs, randomInitialization, rangeSearchAssignment, pamLloydUpdate,
                       RANDOM_INITIALIZATION, RANGE_SEARCH_ASSIGNMENT, PAM_LLOYD_UPDATE);
        } else if (in == 2 && as == 1 && up == 2) {

            // KmeansPlusPlusInitialization_LLoydAssignment_MeanVectorUpdate    2-1-2
            clustering(vectorDataset, configs, kmeansPlusPlusInit, lloydAssignment, meanVectorUpdate,
                       K_MEANS_PLUS_PLUS_INITIALIZATION, LLOYD_ASSIGNMENT, K_MEANS_UPDATE);
        } else if (in == 2 && as == 1 && up == 1) {

            // KmeansPlusPlusInitialization_LLoydAssignment_PamLloydUpdate      2-1-1
            clustering(vectorDataset, configs, kmeansPlusPlusInit, lloydAssignment, pamLloydUpdate,
                       K_MEANS_PLUS_PLUS_INITIALIZATION, LLOYD_ASSIGNMENT, PAM_LLOYD_UPDATE);
        } else if (in == 2 && as == 2 && up == 2) {

            // KmeansPlusPlusInitialization__RangeSearchAssignment_MeanVectorUpdate     2-2-2
            clustering(vectorDataset, configs, kmeansPlusPlusInit, rangeSearchAssignment, meanVectorUpdate,
                       K_MEANS_PLUS_PLUS_INITIALIZATION, RANGE_SEARCH_ASSIGNMENT, K_MEANS_UPDATE);
        } else if (in == 2 && as == 2 && up == 1) {

            // KmeansPlusPlusInitialization__RangeSearchAssignment_PamLloydUpdate       2-2-1
            clustering(vectorDataset, configs, kmeansPlusPlusInit, rangeSearchAssignment, pamLloydUpdate,
                       K_MEANS_PLUS_PLUS_INITIALIZATION, RANGE_SEARCH_ASSIGNMENT, PAM_LLOYD_UPDATE);
        }

        for(auto it = vectorDataset.begin() ; it < vectorDataset.end(); it ++){
            it->clear();
        }

    } else if (answer == "curves") {

        /// Menu option ///

        std::cout << "---------------------------------------------------\n"
                  << "Initialization: 1) Random selection of k curves   2) K-means++\n"
                  << "Assignment:     1) Lloyd's                        2) Range Search with LSH\n"
                  << "Update:         1) PAM a la Lloyd                 2) DBA\n"
                  << "---------------------------------------------------\n";
        std::cout << "Select Initialization algorithm ";
        std::string init;
        std::cin >> init;
        std::cout << "Select Assignment algorithm ";
        std::string assi;
        std::cin >> assi;
        std::cout << "Select Update algorithm ";
        std::string upd;
        std::cin >> upd;
        int in = std::stoi(init);
        int as = std::stoi(assi);
        int up = std::stoi(upd);

        std::vector<Curve> curveDataset;
        int maxCurvePoints = readCurveDataset(configs.inputFile, curveDataset);

        if(in == 1 && as == 1 && up == 2){

            // RandomInitialization_LLoydAssignment_DBAUpdate       1-1-2
            clusteringCurves(curveDataset, maxCurvePoints, configs, randomInitializationCurve, lloydAssignmentCurve,
                             DBAUpdate,
                             RANDOM_INITIALIZATION, LLOYD_ASSIGNMENT, DBA_UPDATE);
        }else if(in == 1 && as == 1 && up == 1){

            // RandomInitialization_LLoydAssignment_PamLloydUpdate      1-1-1
            clusteringCurves(curveDataset, maxCurvePoints, configs, randomInitializationCurve, lloydAssignmentCurve,
                             pamLloydUpdateCurve,
                             RANDOM_INITIALIZATION, LLOYD_ASSIGNMENT, PAM_LLOYD_UPDATE);
        }else if(in == 1 && as == 2 && up == 2){

            // RandomInitialization_RangeSearchAssignment_DBAUpdate     1-2-2
            clusteringCurves(curveDataset, maxCurvePoints, configs, randomInitializationCurve, rangeSearchAssignCurves,
                             DBAUpdate,
                             RANDOM_INITIALIZATION, RANGE_SEARCH_ASSIGNMENT, DBA_UPDATE);
        }else if(in == 1 && as == 2 && up == 1){

            // RandomInitialization_RangeSearchAssignment_PamLloydUpdate        1-2-1
            clusteringCurves(curveDataset, maxCurvePoints, configs, randomInitializationCurve, rangeSearchAssignCurves,
                             pamLloydUpdateCurve,
                             RANDOM_INITIALIZATION, RANGE_SEARCH_ASSIGNMENT, PAM_LLOYD_UPDATE);
        }else if(in == 2 && as == 1 && up == 2){

            // KmeansPlusPlusInitialization_LLoydAssignment_DBAUpdate       2-1-2
            clusteringCurves(curveDataset, maxCurvePoints, configs, kmeansPlusPlusInitCurve, lloydAssignmentCurve,
                             DBAUpdate,
                             K_MEANS_PLUS_PLUS_INITIALIZATION, LLOYD_ASSIGNMENT, DBA_UPDATE);
        }else if(in == 2 && as == 1 && up == 1){

            // KmeansPlusPlusInitialization_LLoydAssignment_PamLloydUpdate      2-1-1
            clusteringCurves(curveDataset, maxCurvePoints, configs, kmeansPlusPlusInitCurve, lloydAssignmentCurve,
                             pamLloydUpdateCurve,
                             K_MEANS_PLUS_PLUS_INITIALIZATION, LLOYD_ASSIGNMENT, PAM_LLOYD_UPDATE);
        } else if(in == 2 && as == 2 && up == 2){

            //  KmeansPlusPlusInitialization_RangeSearchAssignment_DBAUpdate        2-2-2
            clusteringCurves(curveDataset, maxCurvePoints, configs, kmeansPlusPlusInitCurve, rangeSearchAssignCurves,
                             DBAUpdate,
                             K_MEANS_PLUS_PLUS_INITIALIZATION, RANGE_SEARCH_ASSIGNMENT, DBA_UPDATE);
        }else if(in == 2 && as == 2 && up == 1){

            //  KmeansPlusPlusInitialization_RangeSearchAssignment_PamLloydUpdate       2-2-1
            clusteringCurves(curveDataset, maxCurvePoints, configs, kmeansPlusPlusInitCurve, rangeSearchAssignCurves,
                             pamLloydUpdateCurve,
                             K_MEANS_PLUS_PLUS_INITIALIZATION, RANGE_SEARCH_ASSIGNMENT, PAM_LLOYD_UPDATE);
        }
    }
    return 0;
}