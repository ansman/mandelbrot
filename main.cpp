#include "main.h"

using namespace std;

unsigned int width, height;

/*CalcType x0 = -0.74364065;
CalcType xlength = 0.000012068*2000;
CalcType y0 = 0.13182733;
CalcType ylength = 0.000012068*2000;*/

CalcType x0 = -1.9;
CalcType xlength = 2.4;
CalcType y0 = -1.2;
CalcType ylength = 2.4;

void calc(int quality) {
	cl_int err = 0;
	
	float ssquality = 1.0f;
	switch (quality) {
		case CALC_QUALITY_LOW:
			ssquality = 0.25f;
			break;
		case CALC_QUALITY_NORMAL:
			ssquality = 1.0;
			break;
		case CALC_QUALITY_SUPERSAMPLE:
			ssquality = 2.0f;
	}
	
	unsigned int sswidth  = width * ssquality;
	unsigned int ssheight = height * ssquality;
	
	cl_mem output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * sswidth * ssheight, NULL, &err);
	
	if(!output)
		check_error(err, __LINE__, "main.cpp");
	
	CalcType stepx = xlength/sswidth;
	CalcType stepy = ylength/ssheight;
	
	err  = clSetKernelArg(kernel, 0, sizeof(CalcType), &x0);
	err |= clSetKernelArg(kernel, 1, sizeof(CalcType), &y0);
	err |= clSetKernelArg(kernel, 2, sizeof(CalcType), &stepx);
	err |= clSetKernelArg(kernel, 3, sizeof(CalcType), &stepy);
	err |= clSetKernelArg(kernel, 4, sizeof(unsigned int), &sswidth);
	err |= clSetKernelArg(kernel, 5, sizeof(unsigned int), &ssheight);
	err |= clSetKernelArg(kernel, 6, sizeof(unsigned int), &ITERATIONS);
	err |= clSetKernelArg(kernel, 7, sizeof(cl_mem), &output);
	check_error(err, __LINE__, "main.cpp");
	
	
	size_t global[2] = {sswidth, ssheight};
	err = clEnqueueNDRangeKernel(commands, kernel, 2, NULL, global, NULL, 0, NULL, NULL);
	check_error(err, __LINE__, "main.cpp");
	
	clFinish(commands);
	
	float * tmp = (float *)malloc(sizeof(float)*sswidth*ssheight);
	
	err = clEnqueueReadBuffer(commands, output, CL_TRUE, 0, sizeof(float) * sswidth * ssheight, tmp, 0, NULL, NULL);  
	check_error(err, __LINE__, "main.cpp");
	
	
	for (unsigned int i = 0; preTextureProcessing[i] != NULL; ++i)
		preTextureProcessing[i]->process(tmp, sswidth, ssheight, x0, y0, stepx, stepy);
	
	color * texture = Coloration::applyColor(tmp, sswidth, ssheight);
	
	
	for (unsigned int i = 0; postTextureProcessing[i] != NULL; ++i)
		postTextureProcessing[i]->process(texture, sswidth, ssheight);
	glBindTexture(GL_TEXTURE_2D, TEX_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sswidth, ssheight, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
	
	free(texture);
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	glBindTexture(GL_TEXTURE_2D, TEX_ID);
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

int mouseX = 0;
int mouseY = 0;
void mouseDown(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouseX = x;
		mouseY = y;
	} else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		CalcType xc = x0 + ((CalcType) x / (CalcType) width) * xlength;
		CalcType yc = y0 + ((CalcType) (height - y) / (CalcType) height) * ylength;
				
		xlength = xlength / 2.0f;
		ylength = ylength / 2.0f;
		x0 = xc - xlength / 2.0f;
		y0 = yc - ylength / 2.0f;
				
		calc(CALC_QUALITY_SUPERSAMPLE);
		glutPostRedisplay();
	} else if (state == GLUT_UP) {
		calc(CALC_QUALITY_SUPERSAMPLE);
		glutPostRedisplay();
	}
}

void mouseMove(int x, int y) {
	x0 += ((CalcType) (mouseX - x) / (CalcType) width) * xlength;
	y0 += ((CalcType) (y - mouseY) / (CalcType) height) * ylength;
	mouseX = x;
	mouseY = y;
	
	calc(CALC_QUALITY_LOW);
	glutPostRedisplay();
}

int main (int argc, char ** argv) {
    glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutCreateWindow(WINDOW_TITLE);
	
	glutDisplayFunc(&render);
	glutReshapeFunc(&reshape);
	glutMouseFunc(&mouseDown);
	glutMotionFunc(&mouseMove);
	
	glEnable(GL_TEXTURE_2D);
	
	width = WINDOW_WIDTH;
	height = WINDOW_HEIGHT;
	// Load OpenCL-stuff
	setup();
	Coloration::setupColoration();
	calc(CALC_QUALITY_SUPERSAMPLE);
	
	glutMainLoop();
	return 0;
}
