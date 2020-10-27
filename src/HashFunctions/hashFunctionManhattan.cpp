#include <iostream>
#include <chrono>
#include <random>
#include "hashFunctionManhattan.h"

/* Get random float number */
/* Type 0: uniform float   */
/* Type 1: standard float  */
/* Type 2: uniform float   */
float getRandom(int type) {
    /* Set random engine */
    static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::default_random_engine generator(seed);

    /* Set uniform and standard distribution */
    static std::uniform_int_distribution<int> uniformDist2(-5, 5);
    static std::normal_distribution<float> normalDist(0, 1);

    if (type == 2)
        return (float) uniformDist2(generator);

}

hashFunctionManhattan::hashFunctionManhattan(int dimension, int k, int w, int tableSize) : k(k), w(w),
                                                                                           tableSize(tableSize) {

    this->H.reserve(k);

    hManhattan *newFunction;

    for (int i = 0; i < this->k; i++) {          //pick k hash functions

        newFunction = new hManhattan(w, k, dimension);
        if (newFunction == NULL) {
            std::cout << "Failed allocation hManhattan" << std::endl;
            break;
        }

        H[i] = newFunction;
    }

    this->R.reserve(k);
    /* Pick random R values */
    for (int i = 0; i < this->k; i++)
        this->R[i] = (int) getRandom(2);
}

hashFunctionManhattan::~hashFunctionManhattan() {

    for (int i = 0; i < this->k; i++) {
        delete this->H[i];
    }
}

std::string toBinary(int n) {
    std::string r;
    while (n != 0) {
        r = (n % 2 == 0 ? "0" : "1") + r;
        n /= 2;
    }
    return r;
}

//bitwise concatenation
unsigned int hashFunctionManhattan::concatenateH(std::vector<unsigned int> vect, int mod) {

    std::string str;
    unsigned int result = 1;

    for (int i = 0; i < vect.size(); i++) {
        result = result | (unsigned int)vect[i];
        result = result << vect.size();
    }
    return result;
}

//calculate g(x) = [h1(x)|h2(x)|...|hk(x)] mod table size
unsigned int hashFunctionManhattan::hash(DataVector &x) {

    unsigned int currentHashVal;
    std::vector<unsigned int > hashValues(k);

    for (int i = 0; i < k; i++) {

        currentHashVal = this->H[i]->hash(x);

        hashValues[i] = currentHashVal;
    }

    //concatenate h functions
    unsigned int g = concatenateH(hashValues,tableSize);
    g = g % this->tableSize;
    return g;
}


//compare two hash functions
int hashFunctionManhattan::compare(hashFunctionManhattan &x) {

    if (this->k != x.k || this->tableSize != x.tableSize || this->w != x.w) {
        return 1;
    }

    //Compare h functions
    for (int i = 0; i < this->k; i++) {
        if (this->H[i]->compare(*(x.H[i])) != 0)
            return 1;
    }


    return 0;
}