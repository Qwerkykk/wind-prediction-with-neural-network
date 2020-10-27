#include "../CurveToVector/CurveToVector.h"
#include "../Curve/Curve.h"
#include <iostream>
#include <algorithm>


#include "../FileUtils/FileUtils.h"


//Read Curves and insert them to LSH structure
CurveToVector **init(int L, std::vector<Curve> &curveDataset) {

    //one CurveToVector structure for each curve
    CurveToVector **LSH = new CurveToVector *[curveDataset.size()];

    for (int i = 0; i < curveDataset.size(); i++) {
        LSH[i] = new CurveToVector(L, 2, curveDataset.at(i).getID());
        LSH[i]->LshInsert(curveDataset[i]);
    }

    std::cout << "Structures :Initialized" << std::endl;
    return LSH;
}