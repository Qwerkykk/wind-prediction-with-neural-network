#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H


//Uncommenting the line below disables ASSERT
//#define ASSERT

#ifndef ASSERT
#ifndef NDEBUG
#define ASSERT(...) assert(__VA_ARGS__)
#else
#define
#endif // NDEBUG
#endif // STATIC_ASSERT

#endif


#define RANDOM_INITIALIZATION "RandomInitialization"
#define K_MEANS_PLUS_PLUS_INITIALIZATION "K-MeansPlusPlusInitialization"
#define LLOYD_ASSIGNMENT "LloydAssignment"
#define RANGE_SEARCH_ASSIGNMENT "RangeSearchAssignment"
#define DBA_UPDATE "DBAUpdate"
#define K_MEANS_UPDATE "K-MeansUpdate"
#define PAM_LLOYD_UPDATE "PamLloydUpdate"