#ifndef MAIN_H_
#define MAIN_H_

#include <vector>
#include <iostream>
#include <iomanip>
#include <GLUT/GLUT.h>
#include <map>
#include <algorithm>

#include "setup.h"
#include "coloration.h"
#include "global.h"

#define CALC_QUALITY_LOW 1
#define CALC_QUALITY_NORMAL 2
#define CALC_QUALITY_SUPERSAMPLE 3

void calc(int quality);
void render();
void reshape(int, int);
void mouseDown(int button, int state, int x, int y);
void mouseMove(int x, int y);

#endif /* MAIN_H_ */
