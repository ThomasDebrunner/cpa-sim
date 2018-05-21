//
// Created by najiji on 19.05.18.
//

#ifndef CPA_SIM_SIM_H
#define CPA_SIM_SIM_H

#include <string>
#include <vector>
#include <thread>
#include "opencv2/opencv.hpp"

using namespace std;

typedef int window_ref_t;

class Sim {
private:
    cv::VideoCapture *cap;
    cv::Mat frame;
    vector<window_ref_t> windows;
    thread* ui_thread;

public:
    Sim();
    void acquire_frame();
    void source_camera();
    const cv::Mat &get_frame() const;

    window_ref_t create_window();
    ~Sim();
};


#endif //CPA_SIM_SIM_H
