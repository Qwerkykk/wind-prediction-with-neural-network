#ifndef CONFIG_H
#define CONFIG_H


#include <string>

typedef struct config Config;

struct config{
    std::string inputFile;
    std::string configurationFile;
    std::string outputFile;
    bool complete;

    int numberOfCluster;
    int numberOfGrids;
    int numberOfVectorHashTables;
    int numberOfVectorHashFunction;

    config(): complete(false), numberOfGrids(2), numberOfVectorHashTables(3) {}

};

#endif //CONFIG_H
