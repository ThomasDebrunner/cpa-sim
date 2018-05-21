//
// Created by najiji on 19.05.18.
//

#ifndef CPA_SIM_CPA_H
#define CPA_SIM_CPA_H

#include "opencv2/opencv.hpp"
#include "sim.h"
#include "scamp_def.h"

#define SCAMP_WIDTH 256
#define SCAMP_HEIGHT 256


using namespace cv;
using namespace SCAMP;

class Scamp {

private:
    const Sim* sim_ptr;

    const cuda::GpuMat A;
    const cuda::GpuMat B;
    const cuda::GpuMat C;
    const cuda::GpuMat D;
    const cuda::GpuMat E;
    const cuda::GpuMat F;
    const cuda::GpuMat NEWS;

    const cuda::GpuMat R0;
    const cuda::GpuMat R1;
    const cuda::GpuMat R2;
    const cuda::GpuMat R3;
    const cuda::GpuMat R4;
    const cuda::GpuMat R5;
    const cuda::GpuMat R6;
    const cuda::GpuMat R7;
    const cuda::GpuMat R8;
    const cuda::GpuMat R9;
    const cuda::GpuMat R10;
    const cuda::GpuMat R11;
    const cuda::GpuMat R12;
    const cuda::GpuMat FLAG;

    const cuda::GpuMat &analog(areg_t a) const;
    const cuda::GpuMat &digital(dreg_t a) const;

public:
    Scamp(const Sim *simulator);
    void make_global();
    void perform_operation_analog(opcode_t op, areg_t r1, areg_t r2, areg_t r3) const;
    void perform_operation_digital(opcode_t op, dreg_t r1, dreg_t r2, dreg_t r3) const;
    void perform_operation_analog_io(opcode_t op, areg_t r, int a) const;
    void perform_operation_digital_io(opcode_t op, dreg_t r, int a, int b, int c, int d) const;
};


const Scamp *scamp_ptr;


inline void rpix(areg_t target) {
    scamp_ptr->perform_operation_analog(RPIX, target, NULL, NULL);
}

inline void add(areg_t target, areg_t source1, areg_t source2) {
    scamp_ptr->perform_operation_analog(ADD, target, source1, source2);
}

inline void sub(areg_t target, areg_t source1, areg_t source2) {
    scamp_ptr->perform_operation_analog(SUB, target, source1, source2);
}

inline void north(areg_t target, areg_t source) {
    scamp_ptr->perform_operation_analog(NORTH, target, source, NULL);
}

inline void east(areg_t target, areg_t source) {
    scamp_ptr->perform_operation_analog(EAST, target, source, NULL);
}

inline void south(areg_t target, areg_t source) {
    scamp_ptr->perform_operation_analog(SOUTH, target, source, NULL);
}

inline void west(areg_t target, areg_t source) {
    scamp_ptr->perform_operation_analog(WEST, target, source, NULL);
}

inline void mov(areg_t target, areg_t source) {
    scamp_ptr->perform_operation_analog(MOV, target, source, NULL);
}


inline void d_nor(dreg_t target, dreg_t source) {
    scamp_ptr->perform_operation_digital(NOT, target, source, NULL);
}

inline void d_nor(dreg_t target, dreg_t source1, dreg_t source2) {
    scamp_ptr->perform_operation_digital(NOR, target, source1, source2);
}

inline void d_mov(dreg_t target, dreg_t source) {
    scamp_ptr->perform_operation_digital(MOV, target, source, NULL);
}

inline void d_north(dreg_t target, dreg_t source) {
    scamp_ptr->perform_operation_digital(NORTH, target, source, NULL);
}

inline void d_east(dreg_t target, dreg_t source) {
    scamp_ptr->perform_operation_digital(EAST, target, source, NULL);
}

inline void d_south(dreg_t target, dreg_t source) {
    scamp_ptr->perform_operation_digital(SOUTH, target, source, NULL);
}

inline void d_west(dreg_t target, dreg_t source) {
    scamp_ptr->perform_operation_digital(WEST, target, source, NULL);
}

inline void where(areg_t a) {
    scamp_ptr->perform_operation_analog(WHERE, a, NULL, NULL);
}

inline void where(dreg_t a) {
    scamp_ptr->perform_operation_digital(WHERE, a, NULL, NULL);
}

inline void all() {
    scamp_ptr->perform_operation_digital(ALL, NULL, NULL, NULL);
}


inline void d_select_pattern(dreg_t, addr_t a1, addr_t a2, addr_t a3, addr_t a4) {
    scamp_ptr->perform_operation_digital_io(SELECT_PATTERN, a1, a2, a3, a4);
}


inline void in(areg_t target, dac_t value) {
    scamp_ptr->perform_operation_analog_io(IN, value, NULL, NULL, NULL);
}

inline void load(dreg_t target, bool value) {
    scamp_ptr->perform_operation_digital_io(IN, value, NULL, NULL, NULL);
}



#endif //CPA_SIM_CPA_H
