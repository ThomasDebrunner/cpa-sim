//
// Created by najiji on 21.05.18.
//

#include "scamp.h"
#include "sim.h"
#include "opencv2/opencv.hpp"


using namespace cv;
using namespace std;
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



Scamp::Scamp(Sim *simulator) :
        A(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_32F)),
        B(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_32F)),
        C(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_32F)),
        D(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_32F)),
        E(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_32F)),
        F(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_32F)),
        NEWS(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_32F)),
        _AWORK(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_32F)),
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
        _DWORK(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)),
        FIXED_PATTERN_NOISE(UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U)) {
    sim_ptr = simulator;

    randn(FIXED_PATTERN_NOISE, .0, PATTERN_NOISE_STDDEV);
}

void Scamp::make_global() {
    scamp_ptr = this;
}


ScampAccess Scamp::register_reference_analog(areg_t reg) {
    ScampAccess f(analog(reg), false);
    return f;
}

ScampAccess Scamp::register_reference_digital(dreg_t reg) {
    ScampAccess f(digital(reg), true);
    return f;
}


void Scamp::enable_noise() {
    noise_enabled = true;
}

void Scamp::disable_noise() {
    noise_enabled = false;
}


void Scamp::add_noise(const UMat& reg) const {
    if (!noise_enabled) {
        return;
    }
    // signal dependent noise
    multiply(reg, Scalar(1.-SIGNAL_NON_LINEARITY), _AWORK);

    // fixed pattern noise
    add(_AWORK, FIXED_PATTERN_NOISE, reg, FLAG);

    // time dependent noise
    randn(_AWORK, Scalar(.0), GAUSSIAN_NOISE_STDDEV);
    add(reg, _AWORK, NEWS, FLAG);
    NEWS.copyTo(reg);
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
    bool write_on_read = (r1 == r2 || r1 == r3) && r1 != 0;
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
            cropFrame.convertTo(target, CV_32F, 1, -128);
            break;
        }
        case RES: {
            UMat c = target;
            c.setTo(0.00, FLAG);
            break;
        }
        case ADD: {
            add(source1, source2, target, FLAG);
            add_noise(target);
            break;
        }
        case ADDNEG: {
            add(source1, source2, target, FLAG);
            bitwise_not(source1, target, FLAG);
            add_noise(target);
            break;
        }
        case SUB: {
            subtract(source1, source2, target, FLAG);
            add_noise(target);
            break;
        }
        case MOV: {
            source1.copyTo(target, FLAG);
            add_noise(target);
            break;
        }
        case NORTH: {
            auto destination = write_on_read ? _AWORK : target;
            source1(Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT-1)).copyTo(destination(Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT-1)),
                                                                    FLAG(Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT-1)));
            destination(Rect(0, 0, SCAMP_WIDTH, 1)).setTo(Scalar(0), FLAG(Rect(0, 0, SCAMP_WIDTH, 1)));
            if (write_on_read) {
                _AWORK.copyTo(target, FLAG);
            }
            add_noise(target);
            break;
        }
        case EAST: {
            auto destination = write_on_read ? _AWORK : target;
            source1(Rect(1, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)).copyTo(destination(Rect(0, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)),
                                                                    FLAG(Rect(0, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)));
            destination(Rect(SCAMP_WIDTH-1, 0, 1, SCAMP_HEIGHT)).setTo(Scalar(0), FLAG(Rect(SCAMP_WIDTH-1, 0, 1, SCAMP_HEIGHT)));
            if (write_on_read) {
                _AWORK.copyTo(target, FLAG);
            }
            add_noise(target);
            break;
        }
        case SOUTH: {
            auto destination = write_on_read ? _AWORK : target;
            source1(Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT-1)).copyTo(destination(Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT-1)),
                                                                    FLAG(Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT-1)));
            destination(Rect(0, SCAMP_HEIGHT-1, SCAMP_WIDTH, 1)).setTo(Scalar(0), FLAG(Rect(0, SCAMP_HEIGHT-1, SCAMP_WIDTH, 1)));
            if (write_on_read) {
                _AWORK.copyTo(target, FLAG);
            }
            add_noise(target);
            break;
        }
        case WEST: {
            auto destination = write_on_read ? _AWORK : target;
            source1(Rect(0, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)).copyTo(destination(Rect(1, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)),
                                                                    FLAG(Rect(1, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)));
            destination(Rect(0, 0, 1, SCAMP_HEIGHT)).setTo(Scalar(0), FLAG(Rect(0, 0, 1, SCAMP_HEIGHT)));
            if (write_on_read) {
                _AWORK.copyTo(target, FLAG);
            }
            add_noise(target);
            break;
        }
        case DIV2: {
            multiply(source1, 0.5, _AWORK);
            _AWORK.copyTo(target, FLAG);
            add_noise(target);
            break;
        }
        case NEG:
        case INV: {
            bitwise_not(source1, target, FLAG);
            add_noise(target);
            break;
        }
        case WHERE: {
            threshold(target, _AWORK, 0, 255, THRESH_BINARY);
            _AWORK.convertTo(FLAG, CV_8U, 2, 1);
            break;
        }
        default: {
            std::cerr << "Opcode " << op << " not implemented" << std::endl;
            break;
        }
    }
#ifdef SUPER_DEBUG
    sim_ptr->update_ui();
#endif
}


void Scamp::perform_operation_digital(opcode_t op, dreg_t r1, dreg_t r2, dreg_t r3) const {
    auto target = digital(r1);
    auto source1 = digital(r2);
    auto source2 = digital(r3);
    bool write_on_read = (r1 == r2 || r1 == r3) && r1 != 0;
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
            auto destination = write_on_read ? _DWORK : target;
            source1(Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT-1)).copyTo(destination(Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT-1)),
                                                                    FLAG(Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT-1)));
            destination(Rect(0, 0, SCAMP_WIDTH, 1)).setTo(Scalar(0), FLAG(Rect(0, 0, SCAMP_WIDTH, 1)));
            if (write_on_read) {
                _DWORK.copyTo(target, FLAG);
            }
            break;
        }
        case EAST: {
            auto destination = write_on_read ? _DWORK : target;
            source1(Rect(1, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)).copyTo(destination(Rect(0, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)),
                                                                    FLAG(Rect(0, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)));
            destination(Rect(SCAMP_WIDTH-1, 0, 1, SCAMP_HEIGHT)).setTo(Scalar(0), FLAG(Rect(SCAMP_WIDTH-1, 0, 1, SCAMP_HEIGHT)));
            if (write_on_read) {
                _DWORK.copyTo(target, FLAG);
            }
            break;
        }
        case SOUTH: {
            auto destination = write_on_read ? _DWORK : target;
            source1(Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT-1)).copyTo(destination(Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT-1)),
                                                                    FLAG(Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT-1)));
            destination(Rect(0, SCAMP_HEIGHT-1, SCAMP_WIDTH, 1)).setTo(Scalar(0), FLAG(Rect(0, SCAMP_HEIGHT-1, SCAMP_WIDTH, 1)));
            if (write_on_read) {
                _DWORK.copyTo(target, FLAG);
            }
            break;
        }
        case WEST: {
            auto destination = write_on_read ? _DWORK : target;
            source1(Rect(0, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)).copyTo(destination(Rect(1, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)),
                                                                    FLAG(Rect(1, 0, SCAMP_WIDTH-1, SCAMP_HEIGHT)));
            destination(Rect(0, 0, 1, SCAMP_HEIGHT)).setTo(Scalar(0), FLAG(Rect(0, 0, 1, SCAMP_HEIGHT)));
            if (write_on_read) {
                _DWORK.copyTo(target, FLAG);
            }
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
#ifdef SUPER_DEBUG
    sim_ptr->update_ui();
#endif
}


void Scamp::perform_operation_analog_io(opcode_t op, areg_t r, int a) const {
    switch(op) {
        case IN: {
            UMat c = analog(r);
            c.setTo(a, FLAG);
            break;
        }
        default: {
            std::cerr << "Opcode " << op << " not implemented" << std::endl;
            break;
        }
    }
#ifdef SUPER_DEBUG
    sim_ptr->update_ui();
#endif
}

void Scamp::perform_operation_digital_io(opcode_t op, dreg_t r, int a, int b, int c, int d) const {
    switch (op) {
        case IN: {
            UMat temp = digital(r);
            temp.setTo(a, FLAG);
            break;
        }
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
#ifdef SUPER_DEBUG
    sim_ptr->update_ui();
#endif
}

void Scamp::perform_operation_analog_transform(areg_t r1, areg_t r2, int x, int y, int s, bool n) const {
    auto target = analog(r1);
    auto source = analog(r2);
    int ax = abs(x);
    int ay = abs(y);
    UMat dest = _AWORK;
    dest.setTo(0.00, FLAG);
    auto source_rect = Rect(x>0?ax:0, y<0?ay:0, SCAMP_WIDTH-ax, SCAMP_HEIGHT-ay);
    auto target_rect = Rect(x<0?ax:0, y>0?ay:0, SCAMP_WIDTH-ax, SCAMP_HEIGHT-ay);
    source(source_rect).copyTo(dest(target_rect), FLAG(target_rect));

    if (s != 0) {
        multiply(_AWORK, pow(2., -s), _AWORK);
    }

    if (n) {
        subtract(0, _AWORK, _AWORK);
    }
    _AWORK.copyTo(target, FLAG);

    int steps = abs(x)+abs(y)+abs(s)+n;
    while (steps > 0) {
        add_noise(target);
        steps--;
    }

#ifdef SUPER_DEBUG
        sim_ptr->update_ui();
#endif
}

void Scamp::perform_operation_digital_transform(dreg_t r1, dreg_t r2, int x, int y) const {
    auto target = digital(r1);
    auto source = digital(r2);
    int ax = abs(x);
    int ay = abs(y);
    UMat dest = _DWORK;
    dest.setTo(0, FLAG);
    auto source_rect = Rect(x>0?ax:0, y<0?ay:0, SCAMP_WIDTH-ax, SCAMP_HEIGHT-ay);
    auto target_rect = Rect(x<0?ax:0, y>0?ay:0, SCAMP_WIDTH-ax, SCAMP_HEIGHT-ay);
    source(source_rect).copyTo(dest(target_rect), FLAG(target_rect));
#ifdef SUPER_DEBUG
    sim_ptr->update_ui();
#endif
}
