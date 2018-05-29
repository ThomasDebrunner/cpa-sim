//
// Created by najiji on 21.05.18.
//

#include "scamp.h"
#include "sim.h"
#include "opencv2/opencv.hpp"


using namespace cv;
Scamp * scamp_ptr = NULL;


Scamp::Scamp(const Sim *simulator) :
        A(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        B(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        C(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        D(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        E(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        F(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        NEWS(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R0(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R1(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R2(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R3(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R4(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R5(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R6(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R7(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R8(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R9(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R10(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R11(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R12(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        FLAG(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U, 255)) {
    sim_ptr = simulator;
}

void Scamp::make_global() {
    scamp_ptr = this;
}

const UMat &Scamp::analog(areg_t a) const {
    switch(a) {
        case SCAMP::A: return A;
        case SCAMP::B: return B;
        case SCAMP::C: return C;
        case SCAMP::D: return D;
        case SCAMP::E: return E;
        case SCAMP::F: return F;
        case SCAMP::NEWS: return NEWS;
    }
}

const UMat &Scamp::digital(dreg_t a) const {
    switch(a) {
        case SCAMP::R0: return R0;
        case SCAMP::R1: return R1;
        case SCAMP::R2: return R2;
        case SCAMP::R3: return R3;
        case SCAMP::R4: return R4;
        case SCAMP::R5: return R5;
        case SCAMP::R6: return R6;
        case SCAMP::R7: return R7;
        case SCAMP::R8: return R8;
        case SCAMP::R9: return R9;
        case SCAMP::R10: return R10;
        case SCAMP::R11: return R11;
        case SCAMP::R12: return R12;
        case SCAMP::FLAG: return FLAG;
    }
}


void Scamp::perform_operation_analog(opcode_t op, areg_t r1, areg_t r2, areg_t r3) const {
    switch(op) {
        case RPIX: {
            if (!this->sim_ptr) {
                std::cerr << "[Error] Simulator instance not referenced" << std::endl;
            }
            Mat in_frame = sim_ptr->get_frame();
            cvtColor(in_frame, in_frame, CV_BGR2GRAY);
            int width = in_frame.cols;
            int height = in_frame.rows;
            Mat cropFrame = in_frame(Rect((width-height)/2, 0, height-1, height-1));
            UMat work_frame(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U);
            cropFrame.copyTo(work_frame);
            double factor = SCAMP_HEIGHT/work_frame.cols;
            resize(work_frame, analog(r1), cvSize(SCAMP_WIDTH, SCAMP_HEIGHT), factor, factor);
            break;
        }
        case ADD: {
            add(analog(r2), analog(r3), analog(r1), FLAG);
            break;
        }
        case SUB: {
            subtract(analog(r2), analog(r3), analog(r1), FLAG);
            break;
        }
        case MOV: {
            analog(r2).copyTo(analog(r1));
            break;
        }
        case NORTH: {
            Matx<double, 2, 3> trans_mat(1, 0, 0, 0, 1, 1);
            warpAffine(analog(r2), analog(r1), trans_mat, cvSize(SCAMP_WIDTH, SCAMP_HEIGHT));
            break;
        }
        case EAST: {
            Matx<double, 2, 3> trans_mat(1, 0, -1, 0, 1, 0);
            warpAffine(analog(r2), analog(r1), trans_mat, cvSize(SCAMP_WIDTH, SCAMP_HEIGHT));
            break;
        }
        case SOUTH: {
            Matx<double, 2, 3> trans_mat(1, 0, 0, 0, 1, -1);
            warpAffine(analog(r2), analog(r1), trans_mat, cvSize(SCAMP_WIDTH, SCAMP_HEIGHT));
            break;
        }
        case WEST: {
            Matx<double, 2, 3> trans_mat(1, 0, 1, 0, 1, 0);
            warpAffine(analog(r2), analog(r1), trans_mat, cvSize(SCAMP_WIDTH, SCAMP_HEIGHT));
            break;
        }
        case DIV2: {
            multiply(analog(r2), 0.5, analog(r1));
        }
        default: {
            std::cerr << "Opcode " << op << " not implemented" << std::endl;
            break;
        }
    }
}


void Scamp::perform_operation_digital(opcode_t op, dreg_t r1, dreg_t r2, dreg_t r3) const {
    switch(op) {
        default: {
            std::cerr << "Opcode " << op << " not implemented" << std::endl;
            break;
        }
    }
}


void Scamp::perform_operation_analog_io(opcode_t op, areg_t r, int a) const {
    switch(op) {
        case IN: {
            UMat c(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S, a);
            c.copyTo(analog(r));
            break;
        }
        default: {
            std::cerr << "Opcode " << op << " not implemented" << std::endl;
            break;
        }
    }
}

void Scamp::perform_operation_digital_io(opcode_t op, dreg_t r, int a, int b, int c, int d) const {
    switch(op) {
        default: {
            std::cerr << "Opcode " << op << " not implemented" << std::endl;
            return;
        }
    }
}




