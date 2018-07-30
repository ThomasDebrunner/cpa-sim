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

class RegisterReferenceFunctor {
public:
    virtual cv::Mat operator() () = 0;
};


class Sim {
private:
    cv::VideoCapture *cap;
    cv::Mat frame;
    vector<reference_wrapper<RegisterReferenceFunctor>> access_functors;
    vector<const string> window_names;
    chrono::time_point<chrono::high_resolution_clock> last_frame_download;

public:
    Sim();
    void acquire_frame();
    void source_camera();
    void source_video(const string& filename);
    void add_window(RegisterReferenceFunctor& reg, const string& name);
    void start_ui();
    void update_ui();
    const cv::Mat &get_frame() const;
    ~Sim();
};


#endif //CPA_SIM_SIM_H
