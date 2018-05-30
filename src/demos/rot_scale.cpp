//
// Created by Thomas Debrunner on 30.05.18.
//

#include "rot_scale.h"

void shear_x(areg_t reg, double lambda) {
    d_select_pattern(R5, 127, 0, 0, 255);   // single line in middle
    d_select_pattern(R6, 127, 0, 127, 255); // whole upper half plane
    d_select_pattern(R8, 128, 0, 0, 255);   // single line in middle
    d_select_pattern(R9, 128, 0, 127, 255); // whole lower half plane

    double acc = (lambda > 0) ? 1 : ((lambda < 0) ? -1 : 0);

    for (int i = 0; i<127; i++) {
        acc += lambda;
        if (abs(acc) > 1.) {
            if (acc > 0) {
                acc -= 1.;
                d_where(R6);
                west(reg, reg);
                all();
                d_where(R9);
                east(reg, reg);
                all();
            }
            else {
                acc += 1.;
                d_where(R6);
                east(reg, reg);
                all();
                d_where(R9);
                west(reg, reg);
                all();
            }

        }
        d_nor(R7, R6);
        d_nor(R6, R7, R5);
        d_south(R7, R5);
        d_mov(R5, R7);
        d_nor(R10, R9);
        d_nor(R9, R10, R8);
        d_north(R10, R8);
        d_mov(R8, R10);
    }
}


void shear_y(areg_t reg, double lambda) {
    d_select_pattern(R5, 0, 127, 255, 0);   // single line in middle
    d_select_pattern(R6, 0, 127, 255, 127); // whole upper half plane
    d_select_pattern(R8, 0, 128, 255, 0);   // single line in middle
    d_select_pattern(R9, 0, 128, 255, 127); // whole lower half plane

    double acc = (lambda > 0) ? 1 : ((lambda < 0) ? -1 : 0);

    for (int i = 0; i<127; i++) {
        acc += lambda;
        if (abs(acc) > 1.) {
            if(acc >0) {
                acc -= 1.;
                d_where(R6);
                north(reg, reg);
                all();
                d_where(R9);
                south(reg, reg);
                all();
            }
            else {
                acc += 1.;
                d_where(R6);
                south(reg, reg);
                all();
                d_where(R9);
                north(reg, reg);
                all();
            }

        }
        d_nor(R7, R6);
        d_nor(R6, R7, R5);
        d_east(R7, R5);
        d_mov(R5, R7);
        d_nor(R10, R9);
        d_nor(R9, R10, R8);
        d_west(R10, R8);
        d_mov(R8, R10);
    }
}


void scale_y(areg_t reg, double lambda) {
    d_select_pattern(R5, 127, 0, 0, 255);   // single line in middle
    d_select_pattern(R6, 127, 0, 127, 255); // whole upper half plane
    d_select_pattern(R8, 128, 0, 0, 255);   // single line in middle
    d_select_pattern(R9, 128, 0, 127, 255); // whole lower half plane

    double acc = (lambda > 0) ? 1 : ((lambda < 0) ? -1 : 0);

    for (int i = 0; i<127; i++) {
        acc += lambda;
        if (abs(acc) > 1.) {
            if (acc > 0) {
                acc -= 1.;
                d_where(R6);
                north(reg, reg);
                all();
                d_where(R9);
                south(reg, reg);
                all();
            }
            else {
                acc += 1.;
                d_where(R6);
                south(reg, reg);
                all();
                d_where(R9);
                north(reg, reg);
                all();
            }

        }

        d_nor(R7, R6);
        d_nor(R6, R7, R5);
        d_south(R7, R5);
        d_mov(R5, R7);
        d_nor(R10, R9);
        d_nor(R9, R10, R8);
        d_north(R10, R8);
        d_mov(R8, R10);
    }
}


void scale_x(areg_t reg, double lambda) {
    // row-addr, col-addr, row-dc, col-dc
    d_select_pattern(R5, 0, 127, 255, 0);   // single line in middle
    d_select_pattern(R6, 0, 127, 255, 127); // whole upper half plane
    d_select_pattern(R8, 0, 128, 255, 0);   // single line in middle
    d_select_pattern(R9, 0, 128, 255, 127); // whole lower half plane

    double acc = (lambda > 0) ? 1 : ((lambda < 0) ? -1 : 0);

    for (int i = 0; i<127; i++) {
        acc += lambda;
        if (abs(acc) > 1.) {
            if (acc > 0) {
                acc -= 1.;
                d_where(R6);
                west(reg, reg);
                all();
                d_where(R9);
                east(reg, reg);
                all();
            }
            else {
                acc += 1.;
                d_where(R6);
                east(reg, reg);
                all();
                d_where(R9);
                west(reg, reg);
                all();
            }

        }

        d_nor(R7, R6);
        d_nor(R6, R7, R5);
        d_east(R7, R5);
        d_mov(R5, R7);
        d_nor(R10, R9);
        d_nor(R9, R10, R8);
        d_west(R10, R8);
        d_mov(R8, R10);
    }
}

void scale(areg_t reg, double scale) {
    scale_x(reg, scale);
    scale_y(reg, scale);
}

void rotate(areg_t reg, double angle) {
    double alpha = -tan(angle/2.);
    double beta = sin(angle);
    shear_x(reg, alpha);
    shear_y(reg, beta);
    shear_x(reg, alpha);
}