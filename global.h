#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <cstddef>

typedef unsigned char color;
typedef float CalcType;

const int WINDOW_HEIGHT = 768;
const int WINDOW_WIDTH = 768;
#define WINDOW_TITLE "Mandelbrot"
const unsigned int ITERATIONS = 100;
const int TEX_ID = 1;

#include "pre_texture.h"
#include "post_texture.h"

class PreTexture;
class PostTexture;
extern PreTexture * preTextureProcessing[];
extern PostTexture * postTextureProcessing[];

#endif /* GLOBAL_H_ */
