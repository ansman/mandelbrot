#include <vector>
#include <iostream>
#include <iomanip>
#include <GLUT/GLUT.h>
#include <map>
#include <algorithm>

#include "setup.h"

#define CALC_TYPE float

using namespace std;

const int WINDOW_HEIGHT = 768;
const int WINDOW_WIDTH = 768;
const char * WINDOW_TITLE = "Mandelbrot";
const unsigned int ITERATIONS = 1000;

int width, height;

CALC_TYPE x0 = -1.5;
CALC_TYPE xlength = 2;
CALC_TYPE y0 = -1;
CALC_TYPE ylength = 2;

const int texid = 1;

char * texture = NULL;

void calc() {
	// Call open cl
	texture = (char *)malloc(width*3*height);
	
	cl_mem output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * width * height, NULL, NULL);
	
	if(!output) {
		printf("Error: Failed to allocate device memory!\n");
		exit(EXIT_FAILURE);
	}
	
	CALC_TYPE stepx = xlength/width;
	CALC_TYPE stepy = ylength/height;
	
	cl_int err = 0;
	err  = clSetKernelArg(kernel, 0, sizeof(CALC_TYPE), &x0);
	err |= clSetKernelArg(kernel, 1, sizeof(CALC_TYPE), &y0);
	err |= clSetKernelArg(kernel, 2, sizeof(CALC_TYPE), &stepx);
	err |= clSetKernelArg(kernel, 3, sizeof(CALC_TYPE), &stepy);
	err |= clSetKernelArg(kernel, 4, sizeof(unsigned int), &width);
	err |= clSetKernelArg(kernel, 5, sizeof(unsigned int), &height);
	err |= clSetKernelArg(kernel, 6, sizeof(unsigned int), &ITERATIONS);
	err |= clSetKernelArg(kernel, 7, sizeof(cl_mem), &output);
	check_error(err, __LINE__, "main.cpp");
	
	err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	check_error(err, __LINE__, "main.cpp");
	
	err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(global), &global, NULL);
	check_error(err, __LINE__, "main.cpp");
	
	size_t global[2] = {width, height};
	
	err = clEnqueueNDRangeKernel(commands, kernel, 2, NULL, global, NULL, 0, NULL, NULL);
	check_error(err, __LINE__, "main.cpp");
	
	clFinish(commands);
	
	float * tmp = (float *)malloc(sizeof(float)*width*height);
	
	err = clEnqueueReadBuffer(commands, output, CL_TRUE, 0, sizeof(float) * width * height, tmp, 0, NULL, NULL);  
	check_error(err, __LINE__, "main.cpp");
	
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			texture[(y*width + x)*3+0] = texture[(y*width + x)*3+1] = texture[(y*width + x)*3+2] = 255.0f*tmp[(y*width + x)];
		}
	}
	
	
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	glBindTexture(GL_TEXTURE_2D, texid);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd();
	
	glFlush();
}

void reshape(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	glOrtho(0.0, 1.0, 0.0, 1.0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main (int argc, char ** argv) {
    glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutCreateWindow(WINDOW_TITLE);
	
	glutDisplayFunc(&render);
	glutReshapeFunc(&reshape);
	
	glEnable(GL_TEXTURE_2D);
	
	width = WINDOW_WIDTH;
	height = WINDOW_HEIGHT;
	// Load OpenCL-stuff
	setup();
	calc();
	
	glutMainLoop();
	return 0;
}
