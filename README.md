# CPA-SIM

A (very) simple **SIMD Cellular Processor Array Simulator** (also known as Focal-Plane Processors, Vision-Chips etc.) based on **OpenCV**.

* Customizable -> You can hack in your own **CPA**
* Based on **OpenCV** with **OpenCL** modules. Runs on **CPU** and **GPU**

## SCAMP Module

The only CPA currently implemented is loosely based on the *SCAMP 5* chip.
There are some notable differences to the physical chip:

* No noise
* Values are hard-capped
* The `NEWS` register does not update on neighbor transfers
* The `F` register does not update on arithmetic operations
* Some reset-on-write logic (R12?) is not implemented
* All operations can work in-place, where there are some limitations to this on hardware
* No async operations
* Higher level instructions

The following instructions are supported:

**Instruction**   | **Explanation**
------------------|-----------------------------------------------
`rpix`            | Reads in value from camera
`add`             | Adds two values
`sub`             | Subtracts two values
`north`           | Takes value from north cell
`east`            | Takes value from east cell
`south`           | Takes value from south cell
`west`            | Takes value from west cell
`mov`             | Copies value from one register to other
`in`              | Inputs a value to a register
`where`           | Set `FLAG` on pixels where `value > 0`
`all`             | Set `FLAG` on all pixels
`div2`            | Divides value by 2
`d_nor`           | NOR operation (or NOT)
`d_mov`           | Moves digital values from one register to other
`d_north`         | Get digital value from north cell
`d_east`          | Get digital value from east cell
`d_south`         | Get digital value from south cell
`d_west`          | Get digital value from west cell
`d_where`         | Set `FLAG` on pixels where digital register 1
`d_select_pattern`| Select pattern with addresses/don't cares
`d_load`          | Loads a digital value

## Getting Started

### Compiling
```
mkdir build
cd build
cmake ..
make
./cpa-sim
```

### Setting the OpenCL device
One can set the device to run the code on by setting th `OPENCV_OPENCL_DEVICE` Environment variable

For example, to run on the first GPU of the system you could set the variable to
`OPENCV_OPENCL_DEVICE=:GPU:`

### Results
I could only run this on a pretty bad Macbook GPUs *(GeForce 750m and Radeon Pro 560)*, which provide limited speedup compared to CPU. I can imagine that a truly powerful GPU could provide larger speedups.

## Debugging
ALl the computations happen on a remote device, such as a GPU. Therefore, in normal operation, the images are only downloaded for the UI when `update_ui()` is called on the simulator instance.

This speeds up things considerably. However, one can no longer step through a program with a debugger and see the effect of every instruction.

To do this, there is a **super-debug** mode, that downloads the image and updates the UI after every instruction. *This is incredibly slow and only meant to manually step through the program!*

**Enable super-debug mode:**

```
cmake -DSUPER_DEBUG=ON ..
```

(Or create a launch configuration for this in your IDE)


## License

MIT License

Copyright (c) 2018 Thomas Debrunner

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
