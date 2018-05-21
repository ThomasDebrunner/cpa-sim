//
// Created by najiji on 21.05.18.
//

#include "scamp.h"
#include "sim.h"
#include "opencv2/opencv.hpp"


using namespace cv;

Scamp::Scamp(const Sim *simulator) :
        A(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        B(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        C(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        D(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        E(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        F(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        NEWS(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R0(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R1(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R2(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R3(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R4(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R5(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R6(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R7(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R8(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R9(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R10(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R11(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        R12(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        FLAG(cuda::GpuMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)) {
    sim_ptr = simulator;
}

void Scamp::make_global() {
    scamp_ptr = this;
}

const cuda::GpuMat &Scamp::analog(areg_t a) const {
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

const cuda::GpuMat &Scamp::digital(dreg_t a) const {
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
        case RPIX:
            if (!this->sim_ptr) {
                std::cerr << "[Error] Simulator instance not referenced" << std::endl;
            }
            Mat in_frame = sim_ptr->get_frame();
            cuda::GpuMat work_frame;
            cuda::cvtColor(in_frame, work_frame, COLOR_BGR2GRAY);
            cuda::resize(work_frame, analog(r1), cvSize(SCAMP_WIDTH, SCAMP_HEIGHT));
        case ADD:
            cuda::add(analog(r2), analog(r3), analog(r1), FLAG);
            return;
        case SUB:
            cuda::subtract(analog(r2), analog(r3), analog(r1), FLAG);
            return;
        case MOV:
            analog(r2).copyTo(analog(r1));
            return;
        default:
            std::cerr << "Opcode " << op << " not implemented" << std::endl;
            return;
    }
}


void Scamp::perform_operation_digital(opcode_t op, dreg_t r1, dreg_t r2, dreg_t r3) const {
    switch(op) {
        default:
            std::cerr << "Opcode " << op << " not implemented" << std::endl;
            return;
    }
}


void Scamp::perform_operation_analog_io(opcode_t op, areg_t r, int a) const {
    switch(op) {
        case IN:
            cuda::GpuMat c(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S, a);
            c.copyTo(analog(r));
        default:
            std::cerr << "Opcode " << op << " not implemented" << std::endl;
            return;
    }
}

void Scamp::perform_operation_digital_io(opcode_t op, dreg_t r, int a, int b, int c, int d) const {
    switch(op) {
        default:
            std::cerr << "Opcode " << op << " not implemented" << std::endl;
            return;
    }
}




