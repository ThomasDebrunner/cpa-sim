//
// Created by najiji on 21.05.18.
//

#include "scamp.h"
#include "sim.h"
#include "opencv2/opencv.hpp"


using namespace cv;
Scamp * scamp_ptr = NULL;


struct PatternGenerator
{
public:
    PatternGenerator(
            int row_needle,
            int col_needle,
            int row_mask,
            int col_mask) :
        row_needle(row_needle),
        col_needle(col_needle),
        row_mask(row_mask),
        col_mask(col_mask) {}

    void operator ()(uint8_t &pixel, const int * position) const {
        pixel = !((position[0]&row_mask)^row_needle) && !((position[1]&col_mask)^col_needle) ? (uint8_t)255:(uint8_t)0;
    }

private:
    int row_needle, col_needle, row_mask, col_mask;
};



Scamp::Scamp(const Sim *simulator) :
        A(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S)),
        B(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S)),
        C(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S)),
        D(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S)),
        E(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S)),
        F(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S)),
        NEWS(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S)),
        _AWORK(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S)),
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
        FLAG(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U, 255)),
        _DWORK(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)) {
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
        default: return A;
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
        default: return A;
    }
}


void Scamp::perform_operation_analog(opcode_t op, areg_t r1, areg_t r2, areg_t r3) const {
    auto target = analog(r1);
    auto source1 = analog(r2);
    auto source2 = analog(r3);
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
            resize(cropFrame, cropFrame, cvSize(SCAMP_WIDTH, SCAMP_HEIGHT));
            cropFrame.convertTo(target, CV_8S, 1, -128);
            break;
        }
        case ADD: {
            add(source1, source2, target, FLAG);
            break;
        }
        case SUB: {
            subtract(source1, source2, target, FLAG);
            break;
        }
        case MOV: {
            source1.copyTo(target, FLAG);
            break;
        }
        case NORTH: {
            source1(Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT-1)).copyTo(target(Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT-1)),
                                                                    FLAG(Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT-1)));
            target(Rect(0, 0, SCAMP_WIDTH, 1)).setTo(Scalar(0), FLAG(Rect(0, 0, SCAMP_WIDTH, 1)));
            break;
        }
        case EAST: {
            source1(Rect(1, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)).copyTo(target(Rect(0, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)),
                                                                    FLAG(Rect(0, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)));
            target(Rect(SCAMP_WIDTH-1, 0, 1, SCAMP_HEIGHT)).setTo(Scalar(0), FLAG(Rect(SCAMP_WIDTH-1, 0, 1, SCAMP_HEIGHT)));
            break;
        }
        case SOUTH: {
            source1(Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT-1)).copyTo(target(Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT-1)),
                                                                    FLAG(Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT-1)));
            target(Rect(0, SCAMP_HEIGHT-1, SCAMP_WIDTH, 1)).setTo(Scalar(0), FLAG(Rect(0, SCAMP_HEIGHT-1, SCAMP_WIDTH, 1)));
            break;
        }
        case WEST: {
            source1(Rect(0, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)).copyTo(target(Rect(1, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)),
                                                                    FLAG(Rect(1, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)));
            target(Rect(0, 0, 1, SCAMP_HEIGHT)).setTo(Scalar(0), FLAG(Rect(0, 0, 1, SCAMP_HEIGHT)));
            break;
        }
        case DIV2: {
            multiply(analog(r2), 0.5, _AWORK);
            _AWORK.copyTo(analog(r1), FLAG);
            break;
        }
        case WHERE: {
            threshold(source1, _AWORK, 0, 255, THRESH_BINARY);
            _AWORK.convertTo(FLAG, CV_8U, 2, 1);
            break;
        }
        default: {
            std::cerr << "Opcode " << op << " not implemented" << std::endl;
            break;
        }
    }
}


void Scamp::perform_operation_digital(opcode_t op, dreg_t r1, dreg_t r2, dreg_t r3) const {
    auto target = digital(r1);
    auto source1 = digital(r2);
    auto source2 = digital(r3);
    switch(op) {
        case WHERE: {
            target.copyTo(FLAG);
            break;
        }
        case ALL: {
            Mat one(FLAG.size(), FLAG.type(), Scalar(255));
            one.copyTo(FLAG);
            break;
        }
        case MOV: {
            source1.copyTo(target, FLAG);
            break;
        }
        case NORTH: {
            source1(Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT-1)).copyTo(target(Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT-1)),
                                                                    FLAG(Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT-1)));
            target(Rect(0, 0, SCAMP_WIDTH, 1)).setTo(Scalar(0), FLAG(Rect(0, 0, SCAMP_WIDTH, 1)));
            break;
        }
        case EAST: {
            source1(Rect(1, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)).copyTo(target(Rect(0, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)),
                                                                    FLAG(Rect(0, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)));
            target(Rect(SCAMP_WIDTH-1, 0, 1, SCAMP_HEIGHT)).setTo(Scalar(0), FLAG(Rect(SCAMP_WIDTH-1, 0, 1, SCAMP_HEIGHT)));
            break;
        }
        case SOUTH: {
            source1(Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT-1)).copyTo(target(Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT-1)),
                                                                    FLAG(Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT-1)));
            target(Rect(0, SCAMP_HEIGHT-1, SCAMP_WIDTH, 1)).setTo(Scalar(0), FLAG(Rect(0, SCAMP_HEIGHT-1, SCAMP_WIDTH, 1)));
            break;
        }
        case WEST: {
            source1(Rect(0, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)).copyTo(target(Rect(1, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)),
                                                                    FLAG(Rect(1, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)));
            target(Rect(0, 0, 1, SCAMP_HEIGHT)).setTo(Scalar(0), FLAG(Rect(0, 0, 1, SCAMP_HEIGHT)));
            break;
        }
        case NOT: {
            bitwise_not(source1, target, FLAG);
            break;
        }
        case NOR: {
            bitwise_or(source1, source2, target, FLAG);
            bitwise_not(target, target, FLAG);
            break;
        }
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
        case SELECT_PATTERN: {
            auto row_mask = ~c;
            auto col_mask = ~d;
            auto row_needle = a & row_mask;
            auto col_needle = b & col_mask;

            PatternGenerator generator(row_needle, col_needle, row_mask, col_mask);
            Mat pattern(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U);
            pattern.forEach<uint8_t>(generator);
            pattern.copyTo(digital(r));
            break;
        }
        default: {
            std::cerr << "Opcode " << op << " not implemented" << std::endl;
            return;
        }
    }
}




