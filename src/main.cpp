
#include "scamp.h"
#include "demos/rot_scale.h"
#include "sim.h"
#include <math.h>


using namespace SCAMP;


int main(){

    Sim simulation;
    Scamp scamp(&simulation);
    scamp.make_global();

    simulation.source_camera();

    ScampAccess A_access = scamp.register_reference_analog(A);
    simulation.add_window(A_access, "Capture (A)");
    ScampAccess B_access = scamp.register_reference_analog(B);
    simulation.add_window(B_access, "Out");

    simulation.start_ui();

    bool quit = false;
	while (!quit) {
		simulation.update_ui();
		simulation.acquire_frame();

        int i_scale = 10;
        int i_angle = -30;
        rpix(A);
        mov(B, A);

		double scale_factor = i_scale / 100.;
        scale(B, scale_factor);


        double angle = 0.25*3.15*(i_angle/100.);
        rotate(B, angle);

	}
}

