//
// Created by najiji on 19.05.18.
//
#include <iostream>
#include "sim.h"
#define MIN_PERIOD 32
#include <mutex>
#include <string>
#include "opencv2/opencv.hpp"
#include <chrono>

using namespace std;

Sim::Sim() :
        last_frame_download(chrono::high_resolution_clock::now()) {
    cout << cv::getBuildInformation() << std::endl;
}

Sim::~Sim() {
    free(cap);
}

void Sim::start_ui () {
    for (int i=0; i<display_images.size(); i++) {
        string window_name = "Output_" + to_string(i);
        cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    }
}

void Sim::acquire_frame() {
    if (cap == nullptr) {
        std::cerr << "[Error] No capture source specified" << std::endl;
    }
    *cap >> frame;
}

void Sim::update_ui() {
    if (chrono::high_resolution_clock::now() - last_frame_download >= chrono::milliseconds(MIN_PERIOD)) {
        lock_guard<mutex> l(download_guard);
        last_frame_download = chrono::high_resolution_clock::now();
        for (int i=0; i<gpu_images.size(); i++) {
            gpu_images[i].copyTo(display_images[i]);
        }
        for (int i=0; i<display_images.size(); i++) {
            string window_name = "Output_" + to_string(i);
            cv::imshow(window_name, display_images[i]);
            cv::waitKey(1);
        }
    }
}

void Sim::source_camera() {
    cap = new cv::VideoCapture(0);
}

const cv::Mat &Sim::get_frame() const {
    return frame;
}

window_ref_t Sim::add_window(const cv::UMat& reg) {
    gpu_images.push_back(reg);
    cv::Mat display_image(reg.size(), reg.type());
    {
        lock_guard<mutex> l(download_guard);
        display_images.push_back(display_image);
    }
}


