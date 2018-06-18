PiEstimator
============

This IP core carries out a Monte Carlo experiment that will estimate the value
of pi.

Using the IP in Vivado
------------
1. Create a new MicroBlaze or Zynq block design by following the [Getting
Started with Vivado IP Integrator](https://reference.digilentinc.com/vivado/getting-started-with-ipi/start)
tutorial.
2. Open the provided _addPiEstimators.tcl_ script and set the *num_instances*
variable according to how many instances of this IP core you would like to add
to the block design. Each instance requires 8 DSP slices. Divide the number of
DSP slices that your FPGA has by eight to determine the maximum number of
instances that your FPGA can support. The number of DSP slices that your FPGA
has can be found at the resource center for your particular board. The resource
centers for Digilent's FPGA boards are linked from [this page](https://reference.digilentinc.com/).

3. In Vivado's Tcl console, use the __cd__ command to navigate to the directory
containing the _addPiEstimators.tcl_ script. Then, run the script by typing
"source ./addPiEstimators.tcl".

4. Continue following the [Getting Started with Vivado IP Integrator](https://reference.digilentinc.com/vivado/getting-started-with-ipi/start)
tutorial until you have launched the Xilinx SDK and created a new __Empty
Application__ project.

5. Copy the __main.c__ file for running the Monte Carlo experiment to the
__src__ directory within the project you just created. The __main.c__ file can
be found by following this path in the __Project Explorer:__

   design_1_wrapper_hw_platform_0 > drivers > PiEstimator_v1_0 > examples.

   The first directory listed in the path above may be named differently if you
named your block design differently from the default name, but will still
contain the words "wrapper" and "platform."

6. Continue following the [Getting Started with Vivado IP Integrator](https://reference.digilentinc.com/vivado/getting-started-with-ipi/start)
tutorial.
