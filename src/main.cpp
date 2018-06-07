
#include "scamp.h"
#include "sim.h"

#include "ocv_stages/vj_core_ocv_stage_1.h"
#include "ocv_stages/vj_core_ocv_stage_2.h"
#include "ocv_stages/vj_core_ocv_stage_3.h"
#include "ocv_stages/vj_core_ocv_stage_4.h"
#include "ocv_stages/vj_core_ocv_stage_5.h"
#include "ocv_stages/vj_core_ocv_stage_6.h"
#include "ocv_stages/vj_core_ocv_stage_7.h"
#include "ocv_stages/vj_core_ocv_stage_8.h"
#include "ocv_stages/vj_core_ocv_stage_9.h"
#include "ocv_stages/vj_core_ocv_stage_10.h"
#include "ocv_stages/vj_core_ocv_stage_11.h"
#include "ocv_stages/vj_core_ocv_stage_12.h"
#include "ocv_stages/vj_core_ocv_stage_13.h"
#include "ocv_stages/vj_core_ocv_stage_14.h"
#include "ocv_stages/vj_core_ocv_stage_15.h"
#include "ocv_stages/vj_core_ocv_stage_16.h"
#include "ocv_stages/vj_core_ocv_stage_17.h"
#include "ocv_stages/vj_core_ocv_stage_18.h"
#include "ocv_stages/vj_core_ocv_stage_19.h"
#include "ocv_stages/vj_core_ocv_stage_20.h"
#include "ocv_stages/vj_core_ocv_stage_21.h"
#include "ocv_stages/vj_core_ocv_stage_22.h"
#include "ocv_stages/vj_core_ocv_stage_23.h"
#include "ocv_stages/vj_core_ocv_stage_24.h"
#include "ocv_stages/vj_core_ocv_stage_25.h"


using namespace SCAMP;


int main(){

    Sim simulation;
    Scamp scamp(&simulation);
    scamp.make_global();

    // simulation.source_video("../data/faces.mp4");
    simulation.source_camera();

    simulation.add_window(scamp.analog(A), "Captured (A)");
    simulation.add_window(scamp.analog(B), "Evidence accumulator (B)");
 //   simulation.add_window(scamp.analog(C), "Result (C)");
 //   simulation.add_window(scamp.analog(D), "Static values (D)");
    simulation.add_window(scamp.analog(E), "Thresholding (E)");
 //   simulation.add_window(scamp.digital(R4), "Digital Move (R4)");
    simulation.add_window(scamp.digital(R5), "Accumulated res (R5)");
 //   simulation.add_window(scamp.digital(FLAG), "FLAG");


    simulation.start_ui();

    bool quit = false;
	while (!quit) {
		simulation.acquire_frame();

        int i_scale = 10;
        int i_angle = 8;
        rpix(A);
        res(B);
        res(C);
        res(D);
        res(E);
        res(F);
        d_load(R4, false);
        d_load(R5, true);
        d_load(R11, false);
        d_load(R12, false);
        vj_stage_1();
        vj_stage_2();
        vj_stage_3();
        vj_stage_4();
        vj_stage_5();
        vj_stage_6();
        vj_stage_7();
        vj_stage_8();
//        vj_stage_9();
//        vj_stage_10();
//        vj_stage_11();
//        vj_stage_12();
//        vj_stage_13();
//        vj_stage_14();
//        vj_stage_15();
//        vj_stage_16();
//        vj_stage_17();
//        vj_stage_18();
//        vj_stage_19();
//        vj_stage_20();
//        vj_stage_21();
//        vj_stage_22();
//        vj_stage_23();
//        vj_stage_24();
//        vj_stage_25();


//        cv::FileStorage file("evidence.txt", cv::FileStorage::WRITE);
//        cv::Mat local;
//        scamp.analog(E).copyTo(local);
//        file << "thershold" << local;
//
//        scamp.digital(FLAG).copyTo(local);
//        file << "thershold" << local;


        simulation.update_ui();

	}
}

