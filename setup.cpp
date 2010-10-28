#include "setup.h"

cl_device_id device_id;
cl_context context;
cl_command_queue commands;
cl_program program;
cl_kernel kernel;

void setup() {
	cl_int err;
	
	err = clGetDeviceIDs(NULL, USE_GPU ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
	check_error(err, __LINE__, "setup.cpp");
	
	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
	if (!context)
		check_error(err, __LINE__, "setup.cpp");
	
	commands = clCreateCommandQueue(context, device_id, 0, &err);
	if (!commands)
		check_error(err, __LINE__, "setup.cpp");
	
	std::ifstream file(programFile, std::ios_base::binary);
	
	check_error(file.is_open() ? CL_SUCCESS : -1, __LINE__, "setup.cpp", "Failed to open file");
	
	std::string prog(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
	const char * s = prog.c_str();
	
	program = clCreateProgramWithSource(context, 1, (const char **) &s, NULL, &err);
	if (!program)
		check_error(err, __LINE__, "setup.cpp");
	
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err != CL_SUCCESS) {
		size_t len;
		char buffer[2048];
		
		std::cerr << "Error: " << get_error_string(err) << std::endl;
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		std::cerr << buffer << std::endl;
		exit(1);
	}
	
	kernel = clCreateKernel(program, "mandelcalc", &err);
	if (!kernel || err != CL_SUCCESS)
		check_error(err, __LINE__, "setup.cpp");
}


void check_error(cl_int err, int line, std::string file, std::string err_string) {
	if(err == CL_SUCCESS)
		return;
		
	if(err_string == "")
		err_string = get_error_string(err);
	
	std::cerr << "Error in " << file << ':' << line << std::endl;
	std::cerr << err_string << std::endl;
	exit(EXIT_FAILURE);
}


std::string get_error_string(cl_int err) {
	switch (err) {
		case CL_SUCCESS:						  return strdup("Success!");
		case CL_DEVICE_NOT_FOUND:				 return strdup("Device not found.");
		case CL_DEVICE_NOT_AVAILABLE:			 return strdup("Device not available");
		case CL_COMPILER_NOT_AVAILABLE:		   return strdup("Compiler not available");
		case CL_MEM_OBJECT_ALLOCATION_FAILURE:	return strdup("Memory object allocation failure");
		case CL_OUT_OF_RESOURCES:				 return strdup("Out of resources");
		case CL_OUT_OF_HOST_MEMORY:			   return strdup("Out of host memory");
		case CL_PROFILING_INFO_NOT_AVAILABLE:	 return strdup("Profiling information not available");
		case CL_MEM_COPY_OVERLAP:				 return strdup("Memory copy overlap");
		case CL_IMAGE_FORMAT_MISMATCH:			return strdup("Image format mismatch");
		case CL_IMAGE_FORMAT_NOT_SUPPORTED:	   return strdup("Image format not supported");
		case CL_BUILD_PROGRAM_FAILURE:			return strdup("Program build failure");
		case CL_MAP_FAILURE:					  return strdup("Map failure");
		case CL_INVALID_VALUE:					return strdup("Invalid value");
		case CL_INVALID_DEVICE_TYPE:			  return strdup("Invalid device type");
		case CL_INVALID_PLATFORM:				 return strdup("Invalid platform");
		case CL_INVALID_DEVICE:				   return strdup("Invalid device");
		case CL_INVALID_CONTEXT:				  return strdup("Invalid context");
		case CL_INVALID_QUEUE_PROPERTIES:		 return strdup("Invalid queue properties");
		case CL_INVALID_COMMAND_QUEUE:			return strdup("Invalid command queue");
		case CL_INVALID_HOST_PTR:				 return strdup("Invalid host pointer");
		case CL_INVALID_MEM_OBJECT:			   return strdup("Invalid memory object");
		case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:  return strdup("Invalid image format descriptor");
		case CL_INVALID_IMAGE_SIZE:			   return strdup("Invalid image size");
		case CL_INVALID_SAMPLER:				  return strdup("Invalid sampler");
		case CL_INVALID_BINARY:				   return strdup("Invalid binary");
		case CL_INVALID_BUILD_OPTIONS:			return strdup("Invalid build options");
		case CL_INVALID_PROGRAM:				  return strdup("Invalid program");
		case CL_INVALID_PROGRAM_EXECUTABLE:	   return strdup("Invalid program executable");
		case CL_INVALID_KERNEL_NAME:			  return strdup("Invalid kernel name");
		case CL_INVALID_KERNEL_DEFINITION:		return strdup("Invalid kernel definition");
		case CL_INVALID_KERNEL:				   return strdup("Invalid kernel");
		case CL_INVALID_ARG_INDEX:				return strdup("Invalid argument index");
		case CL_INVALID_ARG_VALUE:				return strdup("Invalid argument value");
		case CL_INVALID_ARG_SIZE:				 return strdup("Invalid argument size");
		case CL_INVALID_KERNEL_ARGS:			  return strdup("Invalid kernel arguments");
		case CL_INVALID_WORK_DIMENSION:		   return strdup("Invalid work dimension");
		case CL_INVALID_WORK_GROUP_SIZE:		  return strdup("Invalid work group size");
		case CL_INVALID_WORK_ITEM_SIZE:		   return strdup("Invalid work item size");
		case CL_INVALID_GLOBAL_OFFSET:			return strdup("Invalid global offset");
		case CL_INVALID_EVENT_WAIT_LIST:		  return strdup("Invalid event wait list");
		case CL_INVALID_EVENT:					return strdup("Invalid event");
		case CL_INVALID_OPERATION:				return strdup("Invalid operation");
		case CL_INVALID_GL_OBJECT:				return strdup("Invalid OpenGL object");
		case CL_INVALID_BUFFER_SIZE:			  return strdup("Invalid buffer size");
		case CL_INVALID_MIP_LEVEL:				return strdup("Invalid mip-map level");
		default:								  return strdup("Unknown");
	}
}