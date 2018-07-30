//
// Created by najiji on 30.07.18.
//
#include "sim.h"
#include "scamp.h"

#ifndef CPA_SIM_SCAMP_ACCESS_H
#define CPA_SIM_SCAMP_ACCESS_H


class ScampAccess : public RegisterReferenceFunctor {
private:
    const cv::UMat& reg;
    bool isAnalog;

public:
    ScampAccess(const cv::UMat& reg, bool isAnalog);
    cv::Mat operator() ();
    ~ScampAccess();
};


#endif //CPA_SIM_SCAMP_ACCESS_H
