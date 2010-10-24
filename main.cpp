#include <vector>
#include <iostream>
#include <iomanip>
#include <GLUT/GLUT.h>
#include <map>
#include <algorithm>

using namespace std;

const int WINDOW_HEIGHT = 950;
const int ITERATIONS = 800;


/*long double x0 = -2.5;
long double x1 = 1.0;
long double y0 = -1.0;
long double y1 = 1.0;*/

long double x0 = -0.74364065-0.000012068;
long double x1 = -0.74364065+0.000012068;
long double y0 = 0.13182733-0.000012068;
long double y1 = 0.13182733+0.000012068;


/*long double x0 = -0.75914;
long double x1 = -0.72692;
long double y0 = 0.126433;
long double y1 = 0.142543;*/

/*long double x0 = -0.743643887088449;
long double x1 = -0.743643886985851;
long double y0 = 0.131825904154031;
long double y1 = 0.131825904256629;*/


/*long double x0 = -0.738382593158;
long double x1 = -0.736517264747;
long double y0 = 0.207338038681;
long double y1 = 0.207338038681+0.001865328411;*/

int width, height;

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
	long double x = 0, y = 0;
	
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
	
	long double ratio1 = (x1-x0)/(y1-y0);
	long double ratio2 = (double)width/(double)height;
	
	int w, h;
	
	if (ratio1 > ratio2) {
		w = width;
		h = (double)width / ratio1;
	}
	else {
		h = height;
		w = (double)height * ratio1;
	}

	vector<vector<Color> > new_image(w, vector<Color>(h));
	vector<vector<float> > tmp(w, vector<float>(h));
	
	for (int x = 0; x < w; ++x) {
		for (int y = 0; y < h; ++y)
			tmp[x][y] = calc(x0 + (long double)x/(w-1)*(x1-x0) , y0 + (long double)y/(h-1)*(y1-y0));
	}
	
	if(minper != maxper) {
		for (int x = 0; x < w; ++x) {
			for (int y = 0; y < h; ++y) {
				new_image[x][y] = get_color((tmp[x][y]-minper)/(maxper-minper));
			}
		}
	}
	image = new_image;
}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	glBegin(GL_POINTS);
	
	for (int x = 0; x < image.size(); ++x) {
		for (int y = 0; y < image[x].size(); ++y) {
			Color c = image[x][y];
			//glColor3f(c.r, c.g, c.b);
			glColor3f(c.percent, c.percent, c.percent);
			glVertex3f(x, y, 0.0);
		}
	}
	glEnd();	
	glFlush();
}

void reshape(int w, int h) {
	width = w;
	height = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	glOrtho(0, width, height, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	recalc();
}

int main (int argc, char ** argv) {
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
