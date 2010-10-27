#include <vector>
#include <iostream>
#include <iomanip>
#include <GLUT/GLUT.h>
#include <map>
#include <algorithm>

#include "setup.h"

using namespace std;

const int WINDOW_HEIGHT = 512;
const unsigned int ITERATIONS = 1000000;

const float point_size = 1.0f;

float x0 = -1.0;
float x1 = 1.0;
float y0 = -1.0;
float y1 = 1.0;

/*float x0 = -0.74364065-0.000012068;
float x1 = -0.74364065+0.000012068;
float y0 = 0.13182733-0.000012068;
float y1 = 0.13182733+0.000012068;*/


/*float x0 = -0.75914;
float x1 = -0.72692;
float y0 = 0.126433;
float y1 = 0.142543;*/

/*float x0 = -0.738382593158;
float x1 = -0.736517264747;
float y0 = 0.207338038681;
float y1 = 0.207338038681+0.001865328411;*/

unsigned int width, height;

float minper, maxper;

struct Color {
	float percent, r, g, b;
	Color() : percent(0.0f), r(0.0f), g(0.0f), b(0.0f) {}
	Color(float percent, float r, float g, float b) : percent(percent), r(r), g(g), b(b) {}
};

ostream & operator<<(ostream &out, const Color &c) {
	out << setprecision(3) << fixed << "Color: (" << c.r << ", " << c.g << ", " << c.b << ")";
	return out;
}

vector<vector<Color> > image;
map<float, Color> color_map;

float calc(const double &x0, const double &y0) {
	float x = 0, y = 0;
	
	int iteration = 0;
	
	while(iteration < ITERATIONS && x*x + y*y <= 4) {
		double xtemp = x*x - y*y + x0;
		
		y = x*y*2 + y0;
		x = xtemp;
		++iteration;
	}
	
	float ret = (double)iteration/(double)ITERATIONS;
	
	if(ret < minper)
		minper = ret;
	if(ret > maxper)
		maxper = ret;
	
	return ret;
}

Color get_color(float percent) {
	map<float, Color>::iterator it;
	
	it = color_map.find(percent);
	
	if(it != color_map.end())
		return it->second;
	
	map<float, Color>::iterator upper = color_map.lower_bound(percent);
	map<float, Color>::iterator lower = upper;
	--lower;
	
	float scale = (percent - lower->first) / (upper->first - lower->first);
	
	Color c1 = lower->second, c2 = upper->second;
	
	float r, g, b;
	
	r = c1.r + scale * (c2.r-c1.r);
	g = c1.g + scale * (c2.g-c1.g);
	b = c1.b + scale * (c2.b-c1.b);
	
	return Color(percent, r, g, b);
}

void recalc() {
	minper = 1;
	maxper = 0;
	
	float ratio1 = (x1-x0)/(y1-y0);
	float ratio2 = (double)width/(double)height;
	
	int w, h;
	
	if (ratio1 > ratio2) {
		w = width;
		h = (float)width / ratio1;
	}
	else {
		h = height;
		w = (float)height * ratio1;
	}

	vector<vector<Color> > new_image(w, vector<Color>(h));
	vector<vector<float> > tmp(w, vector<float>(h));
	
	cl_mem output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * width * height, NULL, NULL);

	if(!output) {
		printf("Error: Failed to allocate device memory!\n");
		exit(1);
	}
	
	float stepx = (x1-x0)/width;
	float stepy = (y1-y0)/height;
	
	int err = 0;
	err  = clSetKernelArg(kernel, 0, sizeof(float), &x0);
	err |= clSetKernelArg(kernel, 1, sizeof(float), &y0);
	err |= clSetKernelArg(kernel, 2, sizeof(float), &stepx);
	err |= clSetKernelArg(kernel, 3, sizeof(float), &stepy);
	err |= clSetKernelArg(kernel, 4, sizeof(unsigned int), &width);
	err |= clSetKernelArg(kernel, 5, sizeof(unsigned int), &height);
	err |= clSetKernelArg(kernel, 6, sizeof(unsigned int), &ITERATIONS);
	err |= clSetKernelArg(kernel, 7, sizeof(cl_mem), &output);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to set kernel arguments! %d\n", err);
		exit(1);
	}
	
	err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	if(err != CL_SUCCESS) {
		printf("Error: Failed to retrieve kernel work group info! %d\n", err);
		exit(1);
	}
	
	err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(global), &global, NULL);
	if(err != CL_SUCCESS) {
		printf("Error: Fail %d\n", err);
		exit(1);
	}
	
	
	
	local = 512;
	global = 512;
	err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
	if(err != CL_SUCCESS) {
		printf("%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", 
			   CL_INVALID_PROGRAM_EXECUTABLE,
			   CL_INVALID_COMMAND_QUEUE,
			   CL_INVALID_KERNEL,
			   CL_INVALID_CONTEXT,
			   CL_INVALID_KERNEL_ARGS,
			   CL_INVALID_WORK_DIMENSION,
			   CL_INVALID_WORK_GROUP_SIZE,
			   CL_INVALID_WORK_ITEM_SIZE,
			   CL_INVALID_GLOBAL_OFFSET,
			   CL_OUT_OF_RESOURCES,
			   CL_MEM_OBJECT_ALLOCATION_FAILURE,
			   CL_INVALID_EVENT_WAIT_LIST,
			   CL_OUT_OF_HOST_MEMORY
		);
		printf("Error: Failed to execute kernel! %d\n", err);
		exit(1);
	}
	
	clFinish(commands);
	
	double data[512][512];
	
	err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(float) * width * height, data, 0, NULL, NULL );  
	if(err != CL_SUCCESS) {
		printf("Error: Failed to read output array! %d\n", err);
		exit(1);
	}
	
	for (int x = 0; x < w; ++x) {
		for (int y = 0; y < h; ++y) {
			//cout << data[x][y] << std::endl;
			if (data[x][y] < minper)
				minper = data[x][y];
			if (data[x][y] > maxper)
				maxper = data[x][y];
		}
	}
	
	exit(0);
	
	/*for (int x = 0; x < w; ++x) {
		for (int y = 0; y < h; ++y)
			tmp[x][y] = calc(x0 + (float)x/(w-1)*(x1-x0), y0 + (float)y/(h-1)*(y1-y0));
	}*/
	
	if(minper != maxper) {
		for (int x = 0; x < w; ++x) {
			for (int y = 0; y < h; ++y) {
				//new_image[x][y] = get_color((data[x][y]-minper)/(maxper-minper));
				new_image[x][y] = get_color(data[x][y]);
			}
		}
	}
	image = new_image;
}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glPointSize(point_size);
	glBegin(GL_POINTS);
	
	for (int x = 0; x < image.size(); ++x) {
		for (int y = 0; y < image[x].size(); ++y) {
			Color c = image[x][y];
			glColor3f(c.r, c.g, c.b);
			//glColor3f(c.percent, c.percent, c.percent);
			glVertex3f(x*point_size+point_size/2, y*point_size+point_size/2, 0.0);
		}
	}
	glEnd();	
	glFlush();
}

void reshape(int w, int h) {
	width = w/point_size;
	height = h/point_size;
	width = height = 512;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	glOrtho(0, w, h, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	recalc();
}

int main (int argc, char ** argv) {
	// Load OpenCL-stuff
	setup();
	
	color_map[0.00] = Color(0.00, 0.556862745098039, 0, 0);
	color_map[0.25] = Color(0.25, 1, 0.780392156862745, 0.184313725490196);
	color_map[0.50] = Color(0.50, 1, 1, 1);
	color_map[0.75] = Color(0.75, 0, 0.376470588235294, 0.694117647058824);
	color_map[1.00] = Color(1.00, 0, 0, 0);
	
    glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize((x1-x0)/(y1-y0) * WINDOW_HEIGHT, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutCreateWindow("Mandelbrot");
	
	glutDisplayFunc(&renderScene);
	glutReshapeFunc(&reshape);
		
	glutMainLoop();
	return 0;
}
