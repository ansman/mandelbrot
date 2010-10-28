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

void calc();
void render();
void reshape(int, int);
void mouseMove(int x, int y);

#endif /* MAIN_H_ */
