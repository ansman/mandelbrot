#include <GLUT/GLUT.h>

#include "setup.h"

void render();
void reshape(int, int);

int main (int argc, char ** argv) {
	// Load OpenCL-stuff
	setup();
	
    glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(512, 512);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutCreateWindow("Mandelbrot");
	
	glutDisplayFunc(&render);
	glutReshapeFunc(&reshape);
	
	glutMainLoop();
	return 0;
}

void render() {
	
}

void reshape(int w, int h) {
	
}