//
// Created by najiji on 19.05.18.
//
#include <iostream>
#include "sim.h"
#include <string>
#include "opencv2/opencv.hpp"



Sim::Sim() {
    std::cout << cv::getBuildInformation() << std::endl;
    ui_thread = new thread();
}

Sim::~Sim() {
    free(cap);
    free(ui_thread);
}

void Sim::acquire_frame() {
    if (cap == NULL) {
        std::cerr << "[Error] No capture source specified" << std::endl;
    }
    *cap >> frame;
}

void Sim::source_camera() {
    cap = new cv::VideoCapture(0);
}

const cv::Mat &Sim::get_frame() const {
    return frame;
}

window_ref_t create_window() {

}


