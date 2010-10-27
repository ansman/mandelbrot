#include "setup.h"

cl_device_id device_id;
cl_context context;
cl_command_queue commands;
cl_program program;
cl_kernel kernel;

cl_mem input;
cl_mem output;

size_t local, global;

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
	
	std::ifstream in;
	std::stringstream s;
	
	in.open(programFile, std::ios_base::binary);
	
	if(in.fail()) {
		std::cerr << "Error: Could not open program code from " << programFile  << std::endl;
		exit(1);
	}
	
	while(!in.eof() && in.peek() != -1) {
		s.put((char)in.get());
	}
	
	in.close();
		
	const char * programCodeString = s.str().c_str();
	
	program = clCreateProgramWithSource(context, 1, (const char **) &programCodeString, NULL, &err);
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
