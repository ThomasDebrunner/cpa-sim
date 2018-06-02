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


class Sim {
private:
    cv::VideoCapture *cap;
    cv::Mat frame;
    vector<const cv::UMat> gpu_images;
    vector<const cv::Mat> display_images;
    vector<const string> window_names;
    chrono::time_point<chrono::high_resolution_clock> last_frame_download;
    mutex download_guard;

public:
    Sim();
    void acquire_frame();
    void source_camera();
    void source_video(const string& filename);
    void add_window(const cv::UMat& reg);
    void add_window(const cv::UMat& reg, const string& name);
    void start_ui();
    void update_ui();
    const cv::Mat &get_frame() const;
    ~Sim();
};


#endif //CPA_SIM_SIM_H
