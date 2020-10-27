#include "hManhattan.h"
#include <chrono>
#include <random>
#include <iostream>


float hManhattan::getRandomUniform(int w) {

    static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::default_random_engine generator(seed);

    static std::uniform_real_distribution<float> uniformDist(0, w);

    return uniformDist(generator);
}


hManhattan::hManhattan(int w, int k, int dimension) : w(w), k(k) {

    s.reserve(dimension);

    for (int i = 0; i < dimension; i++) {
        s[i] = getRandomUniform(this->w);    //pick a random s(i) - uniform distribution
    }

}

hManhattan::~hManhattan() {

}


int myMod(int x, int y){
    return ((x % y) + y) % y;
}

int expmod(int x,int i,int mod){

    if(i==0){
        return 1;
    }
    else if(i%2 !=0){
        //is odd
        return (expmod(x,i-1,mod)*x) % mod;
    }
    else{
        int y = expmod(x,i/2,mod);
        return (y*y) % mod;
    }
}

//a(i) = floor((x(i)-s(i))/W)
//h(i)(x) = a{d-1} + m a{d-2} + ... + m^(d-1) a0
unsigned int hManhattan::hash(DataVector &x) {

    int dim = x.getDimension();


    std::vector<int> temp(dim);         //keep a(i)'s in order to calculate h(x) after for

    //calculate a(i)
    for (int i = 0; i < dim - 1; i++) {


        double a = x.getComponents()[i] - s[i];        //x(i) - s(i)
        a = a / this->w;
        int result = floor(a);
        temp[i] = result;

    }

    int power = 32 / this->k;
    int M = (int) pow(2, power);      // M=2^(32/k)
    int m = (unsigned int) (pow(2, 32) - 5);       // m = 2^32 - 5

    //calculate h(x) = a{d-1} + m a{d-2} + ... + m^(d-1) a0 mod M
    int h = temp[dim - 1];

    for (int i = dim - 2; i > 0; i--) {

        int c = m* temp[i];
        h += myMod(c,M);

        m = expmod(abs(m),abs(m),M);           //update m - avoid overflow
    }

    return abs(h % M);
}

//Can't compare different hash functions
int hManhattan::compare(hManhattan &x) {
    return -1;
}

DataVector *hManhattan::getX() const {
    return x;
}

int hManhattan::getW() const {
    return w;
}

int hManhattan::getK() const {
    return k;
}

void hManhattan::setX(DataVector *x) {
    hManhattan::x = x;
}

void hManhattan::setW(int w) {
    hManhattan::w = w;
}

void hManhattan::setK(int k) {
    hManhattan::k = k;
}
