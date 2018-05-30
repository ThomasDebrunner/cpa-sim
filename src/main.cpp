
#include "scamp.h"
#include "demos/rot_scale.h"
#include "sim.h"
#include <math.h>

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

    simulation.source_camera();

    simulation.add_window(scamp.analog(A));
    simulation.add_window(scamp.analog(B));
    simulation.add_window(scamp.analog(C));
    simulation.add_window(scamp.analog(D));
    simulation.add_window(scamp.analog(E));
    simulation.add_window(scamp.digital(R4));
    simulation.add_window(scamp.digital(R5));
    simulation.add_window(scamp.digital(FLAG));



    simulation.start_ui();

    bool quit = false;
	while (!quit) {
		simulation.acquire_frame();

        int i_scale = 10;
        int i_angle = 8;
        rpix(A);
        d_load(R4, false);
        d_load(R5, true);
        d_load(R11, false);
        d_load(R12, false);
//        vj_stage_1();
//        vj_stage_2();
//        vj_stage_3();
//        vj_stage_4();
//        vj_stage_5();
//        vj_stage_6();
//        vj_stage_7();
//        vj_stage_8();
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


        in(B, 0.00);
        mov(C, A);
        north(D, C);
        north(D, D);
        north(D, D);
        sub(C, C, D);
        south(D, C);
        south(D, D);
        south(D, D);
        sub(C, C, D);
        east(D, C);
        east(D, D);
        east(D, D);
        east(D, D);
        add(D, C, D);
        west(C, C);
        west(C, C);
        west(C, C);
        west(C, C);
        add(C, D, C);
        north(D, C);
        add(D, C, D);
        south(C, C);
        add(C, D, C);
        west(D, C);
        east(C, C);
        add(D, C, D);
        west(C, D);
        add(C, D, C);
        in(D, -0.0315120);
        sub(E, C, D);
        where(E);

        cv::FileStorage file("evidence.txt", cv::FileStorage::WRITE);
        cv::Mat local;
        scamp.analog(E).copyTo(local);
        file << "thershold" << local;

        scamp.digital(FLAG).copyTo(local);
        file << "thershold" << local;


        d_mov(R4, FLAG);
        all();
        d_north(R4, R4);
        d_north(R4, R4);
        d_north(R4, R4);
        d_north(R4, R4);
        d_where(R4);
        in(D, -2.2172101);
        d_nor(FLAG, FLAG);
        in(D, 2.0875380);
        all();

        simulation.update_ui();
//        cv::FileStorage file("evidence.txt", cv::FileStorage::WRITE);
//        cv::Mat local;
//        scamp.analog(D).copyTo(local);
//        file << "evidence" << local;

//        in(B, 0.5);
//        sub(A, A, B);
//        where(A);
//
//        d_mov(R4, FLAG);



//
//		double scale_factor = i_scale / 100.;
//        scale(B, scale_factor);
//
//
//        double angle = 0.25*3.15*(i_angle/100.);
//        rotate(B, angle);

	}
}

