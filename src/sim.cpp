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

// -- Grid layout definition for the window alignments
#define TILE_WIDTH 256
#define TILE_HEIGHT 280
#define TILE_COUNT_H 2

using namespace std;

Sim::Sim() :
        last_frame_download(chrono::high_resolution_clock::now()) {
    cout << cv::getBuildInformation() << std::endl;
}

Sim::~Sim() {
    free(cap);
}

void Sim::start_ui () {
    for (int i=0; i<access_functors.size(); i++) {
        cv::namedWindow(window_names[i], cv::WINDOW_AUTOSIZE);
        cv::moveWindow(window_names[i], i%TILE_COUNT_H*TILE_WIDTH, i/TILE_COUNT_H*TILE_HEIGHT);
    }
}

void Sim::acquire_frame() {
    if (cap == nullptr) {
        std::cerr << "[Error] No capture source specified" << std::endl;
    }
    *cap >> frame;
}

void Sim::update_ui() {
    auto time_since_last_frame = chrono::high_resolution_clock::now() - last_frame_download;
    std::cout << (1e3/chrono::duration_cast<chrono::milliseconds>(time_since_last_frame).count()) << "fps\r";
    if (time_since_last_frame >= chrono::milliseconds(MIN_PERIOD)) {
        last_frame_download = chrono::high_resolution_clock::now();
        for (int i=0; i<access_functors.size(); i++) {
            cv::imshow(window_names[i], access_functors[i]());
            cv::waitKey(1);
        }
    }
}

void Sim::source_camera() {
    cap = new cv::VideoCapture(0);
}

void Sim::source_video(const string& filename) {
    cap = new cv::VideoCapture(filename);
}

const cv::Mat &Sim::get_frame() const {
    return frame;
}

void Sim::add_window(RegisterReferenceFunctor& reg_functor, const string& name) {
    access_functors.push_back(reg_functor);
    window_names.push_back(name);
}


