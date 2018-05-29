//
// Created by najiji on 19.05.18.
//

#ifndef CPA_SIM_SIM_H
#define CPA_SIM_SIM_H

#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include "opencv2/opencv.hpp"

using namespace std;

typedef int window_ref_t;

class Sim {
private:
    cv::VideoCapture *cap;
    cv::Mat frame;
    vector<const cv::UMat> gpu_images;
    vector<const cv::Mat> display_images;
    chrono::time_point<chrono::high_resolution_clock> last_frame_download;
    thread* ui_thread;
    mutex download_guard;

    void ui_routine();
    bool quit = false;

public:
    Sim();
    void acquire_frame();
    void source_camera();
    window_ref_t add_window(const cv::UMat& reg);
    void start_ui();
    const cv::Mat &get_frame() const;
    ~Sim();
};


#endif //CPA_SIM_SIM_H
