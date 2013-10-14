#include "hdr.h"

void HDR::process(CalcType * data, unsigned int width, unsigned int height, CalcType x0, CalcType y0, CalcType xstep, CalcType ystep) {
    CalcType min = 1, max = 0;
    
    for (unsigned int i = 0; i < width*height; ++i) {
        if (data[i] < min)
            min = data[i];
        
        if (data[i] > max)
            max = data[i];
    }
    
    if (min == max)
        return;
    
    for (unsigned int i = 0; i < width*height; ++i)
        data[i] = (data[i]-min)/(max-min);
}
