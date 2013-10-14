#include "supersampling.h"

void Supersampling::process(color * data, unsigned int width, unsigned int height) const {
    color * tempdata = (color *)malloc(sizeof(color)*width*height*3);
    memcpy(tempdata, data, sizeof(color)*width*height*3);

    for (unsigned int index = 0; index < width*height; ++index) {
        int count = 0;
        Coloration::Color c(0, 0, 0, 0), tmp;
        int r = 0, g = 0, b = 0;

        for (int i = -SUB_PIXEL_SIZE; i <= SUB_PIXEL_SIZE; i += SUB_PIXEL_SIZE) {
            for (int j = -SUB_PIXEL_SIZE; j <= SUB_PIXEL_SIZE; j += SUB_PIXEL_SIZE) {
                if (getColor(tempdata, index+width*j+i, width*height, tmp)) {
                    ++count;
                    r += tmp.r;
                    g += tmp.g;
                    b += tmp.b;
                }
            }
        }

        data[index*3 + 0] = r/count;
        data[index*3 + 1] = g/count;
        data[index*3 + 2] = b/count;
    }

    free(tempdata);
}

bool Supersampling::getColor(color * data, int index, unsigned int size, Coloration::Color & c) const {
    if (index < 0 || (unsigned int) index >= size)
        return false;

    c.r = data[index*3 + 0];
    c.g = data[index*3 + 1];
    c.b = data[index*3 + 2];
    return true;
}

