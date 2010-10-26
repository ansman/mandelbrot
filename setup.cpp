#include "setup.h"

cl_device_id device_id;
cl_context context;
cl_command_queue commands;
cl_program program;
cl_kernel kernel;

cl_mem input;
cl_mem output;

const char *KernelSource = "\n" \
"__kernel mandelcalc(double xs, double ys, double stepx, double stepy, unsigned int width, unsigned int height, unsigned int iterations, __global float * output)                                                       \n" \
"{                                                                                       \n" \
"    int index = get_global_id(0);                                                       \n" \
"	                                                                                     \n" \
"    double x0 = xs + stepx*(double)(index%width);                                       \n" \
"    double y0 = ys + stepy*(double)(index%height);                                      \n" \
"                                                                                        \n" \
"    double x = 0;                                                                       \n" \
"	 double y = 0;                                                                       \n" \
"    unsigned int it = 0;                                                                \n" \
"	                                                                                     \n" \
"    while (it < iterations) {                                                           \n" \
"       double xx = x*x;                                                                 \n" \
"		double yy = y*y;                                                                 \n" \
"        if (xx+yy > 4)                                                                  \n" \
"            break;                                                                      \n" \
"        y = 2*x*y + y0;                                                                 \n" \
"        x = xx - yy + x0;                                                               \n" \
"        ++it;                                                                           \n" \
"    }                                                                                   \n" \
"	                                                                                     \n" \
"    output[index] = it/iterations;                                                      \n" \
"}                                                                                       \n" \
"\n";

void setup() {
	int err;
	
    err = clGetDeviceIDs(NULL, USE_GPU ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
    if (err != CL_SUCCESS) {
		std::cerr << "Error: Failed to create a device group!" << std::endl;
		exit(1);
    }
	
	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
	if (!context) {
		std::cerr << "Error: Failed to create a compute context!" << std::endl;
		exit(1);
	}
	
	commands = clCreateCommandQueue(context, device_id, 0, &err);
	if (!commands) {
		std::cerr << "Error: Failed to create a command commands!" << std::endl;
		exit(1);
	}
	
//	std::ifstream in;
//	std::stringstream s;
//	
//	in.open(programFile, std::ios_base::binary);
//	
//	if(in.fail()) {
//		std::cerr << "Error: Could not open program code from " << programFile  << std::endl;
//		exit(1);
//	}
//	
//	while(!in.eof() && in.peek() != -1) {
//		s.put((char)in.get());
//	}
//	
//	in.close();
//		
//	const char * programCodeString = s.str().c_str();
	
	program = clCreateProgramWithSource(context, 1, (const char **) &KernelSource, NULL, &err);
	if (!program) {
		std::cerr << "Error: Failed to create compute program!" << std::endl;
		exit(1);
	}
	
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err != CL_SUCCESS) {
		size_t len;
		char buffer[2048];
		
		printf("Error: Failed to build program executable!\n");
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		printf("%s\n", buffer);
		exit(1);
	}
	
	kernel = clCreateKernel(program, "mandelcalc", &err);
	if (!kernel || err != CL_SUCCESS) {
		printf("Error: Failed to create compute kernel!\n");
		exit(1);
	}
}
