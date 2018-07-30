#include "opencv2/opencv.hpp"
#include "scamp_access.h"


//
// Created by najiji on 30.07.18.
//

ScampAccess::ScampAccess(const cv::UMat& reg, bool isAnalog) :
    reg(reg),
    isAnalog(isAnalog) {}


cv::Mat ScampAccess::operator() () {
    Mat m(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    reg.convertTo(m, CV_8S, 1, 0);
    return m;
}


ScampAccess::~ScampAccess() {}