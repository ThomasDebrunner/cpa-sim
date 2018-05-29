
#include "scamp.h"
#include "sim.h"
#include <math.h>


#define _b;
#define _e;


using namespace SCAMP;



void shear_x(float lambda) {
	d_select_pattern(R5, 127, 0, 0, 255);   // single line in middle
	d_select_pattern(R6, 127, 0, 127, 255); // whole upper half plane
	d_select_pattern(R8, 128, 0, 0, 255);   // single line in middle
	d_select_pattern(R9, 128, 0, 127, 255); // whole lower half plane

	float acc = (lambda > 0) ? 1 : ((lambda < 0) ? -1 : 0);

	for (int i = 0; i<127; i++) {
		acc += lambda;
		if (abs(acc) > 1.) {
			if (acc > 0) {
				acc -= 1.;
				_b
				d_where(R6);
				west(B, B);
				all();
				d_where(R9);
				east(B, B);
				all();
				_e
			}
			else {
				acc += 1.;
				_b
				d_where(R6);
				east(B, B);
				all();
				d_where(R9);
				west(B, B);
				all();
				_e
			}

		}
		_b
		d_nor(R7, R6);
		d_nor(R6, R7, R5);
		d_south(R7, R5);
		d_mov(R5, R7);
		d_nor(R10, R9);
		d_nor(R9, R10, R8);
		d_north(R10, R8);
		d_mov(R8, R10);
		_e
	}
}


void shear_y(float lambda) {
	d_select_pattern(R5, 0, 127, 255, 0);   // single line in middle
	d_select_pattern(R6, 0, 127, 255, 127); // whole upper half plane
	d_select_pattern(R8, 0, 128, 255, 0);   // single line in middle
	d_select_pattern(R9, 0, 128, 255, 127); // whole lower half plane

	float acc = (lambda > 0) ? 1 : ((lambda < 0) ? -1 : 0);

	for (int i = 0; i<127; i++) {
		acc += lambda;
		if (abs(acc) > 1.) {
			if(acc >0) {
				acc -= 1.;
				_b
				d_where(R6);
				north(B, B);
				all();
				d_where(R9);
				south(B, B);
				all();
				_e
			}
			else {
				acc += 1.;
				_b
				d_where(R6);
				south(B, B);
				all();
				d_where(R9);
				north(B, B);
				all();
				_e
			}

		}
		_b

		d_nor(R7, R6);
		d_nor(R6, R7, R5);
		d_west(R7, R5);
		d_mov(R5, R7);
		d_nor(R10, R9);
		d_nor(R9, R10, R8);
		d_east(R10, R8);
		d_mov(R8, R10);
		_e
	}
}


void scale_y(float lambda) {
	d_select_pattern(R5, 127, 0, 0, 255);   // single line in middle
	d_select_pattern(R6, 127, 0, 127, 255); // whole upper half plane
	d_select_pattern(R8, 128, 0, 0, 255);   // single line in middle
	d_select_pattern(R9, 128, 0, 127, 255); // whole lower half plane

	float acc = (lambda > 0) ? 1 : ((lambda < 0) ? -1 : 0);

	for (int i = 0; i<127; i++) {
		acc += lambda;
		if (abs(acc) > 1.) {
			if (acc > 0) {
				acc -= 1.;
				_b
				d_where(R6);
				north(B, B);
				all();
				d_where(R9);
				south(B, B);
				all();
				_e
			}
			else {
				acc += 1.;
				_b
				d_where(R6);
				south(B, B);
				all();
				d_where(R9);
				north(B, B);
				all();
				_e
			}

		}
		_b

		d_nor(R7, R6);
		d_nor(R6, R7, R5);
		d_south(R7, R5);
		d_mov(R5, R7);
		d_nor(R10, R9);
		d_nor(R9, R10, R8);
		d_north(R10, R8);
		d_mov(R8, R10);
		_e
	}
}


void scale_x(float lambda) {
    // row-addr, col-addr, row-dc, col-dc
	d_select_pattern(R5, 0, 127, 255, 0);   // single line in middle
	d_select_pattern(R6, 0, 127, 255, 127); // whole upper half plane
	d_select_pattern(R8, 0, 128, 255, 0);   // single line in middle
	d_select_pattern(R9, 0, 128, 255, 127); // whole lower half plane

	float acc = (lambda > 0) ? 1 : ((lambda < 0) ? -1 : 0);

	for (int i = 0; i<127; i++) {
		acc += lambda;
		if (abs(acc) > 1.) {
			if (acc > 0) {
				acc -= 1.;
				_b
				d_where(R6);
				west(B, B);
				all();
				d_where(R9);
				east(B, B);
				all();
				_e
			}
			else {
				acc += 1.;
				_b
				d_where(R6);
				east(B, B);
				all();
				d_where(R9);
				west(B, B);
				all();
				_e
			}

		}
		_b

		d_nor(R7, R6);
		d_nor(R6, R7, R5);
		d_west(R7, R5);
		d_mov(R5, R7);
		d_nor(R10, R9);
		d_nor(R9, R10, R8);
		d_east(R10, R8);
		d_mov(R8, R10);
		_e
	}
}


int main(){

    Sim simulation;
    Scamp scamp(&simulation);
    scamp.make_global();

    simulation.source_camera();

    simulation.add_window(scamp.analog(A));
    simulation.add_window(scamp.analog(B));


    simulation.start_ui();

    bool quit = false;
	while (!quit) {
		simulation.update_ui();
		simulation.acquire_frame();

        int i_scale = 32;
        int i_angle = 19;
        rpix(A);
        mov(B, A);


		float scale = i_scale / 100.;
		scale_x(scale);
		scale_y(scale);

		float angle = 0.25*3.15*(i_angle/100.);
		float alpha = -tan(angle/2.);
		float beta = sin(angle);
		shear_x(alpha);
		shear_y(beta);
		shear_x(alpha);


	}
}

