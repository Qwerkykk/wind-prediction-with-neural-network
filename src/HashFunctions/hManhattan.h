
#ifndef  HMANHATTAN_H
#define  HMANHATTAN_H

#include "../DataVector/DataVector.h"

//a(i) = floor((x(i)-s(i))/W)
class hManhattan {
private:
    DataVector *x;          //point
    //float s;            //s(i) random float [0,w) - Uniform distribution
    int w;           //window size
    int k;
    std::vector<float> s;

public:
    hManhattan(int w, int k, int dimension);

    ~hManhattan();

    unsigned int hash(DataVector &x);

    int compare(hManhattan &x);

    void setX(DataVector *x);

    void setW(int w);

    void setK(int k);

    DataVector *getX() const;

    int getW() const;

    int getK() const;

    float getRandomUniform(int w);
};


#endif // HMANHATTAN_H
