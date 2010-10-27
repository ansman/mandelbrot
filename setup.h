#ifndef SETUP_H_
#define SETUP_H_

#include <OpenCL/opencl.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#define USE_GPU true

#define programFile "../../calculations.c"

extern cl_device_id device_id;
extern cl_context context;
extern cl_command_queue commands;
extern cl_program program;
extern cl_kernel kernel;

extern cl_mem output;

extern size_t local, global;

void setup();

#endif /* SETUP_H_ */