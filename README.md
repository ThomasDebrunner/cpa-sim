# CPA-SIM

A (very) simple **Cellular Processor Array Simulator** based on **OpenCV**

* Customizable -> You can hack in your own CPA
* Uses OpenCL, works best on powerful GPUs

## SCAMP Module

The only CPA currently implemented is loosely based on the *SCAMP 5* chip.
There are some differences to the physical chip:

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
```
mkdir build
cd build
cmake ..
make
./cpa-sim
```

## Setting the OpenCL device
One can set the device to run the code on by setting th `OPENCV_OPENCL_DEVICE` Environment variable

For example, to run on the first GPU of the system you could set the variable to
`OPENCV_OPENCL_DEVICE=:GPU:`