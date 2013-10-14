#ifndef HDR_H_
#define HDR_H_

#include "pre_texture.h"

class HDR : public PreTexture {
public:
    virtual void process(CalcType * data, unsigned int width, unsigned int height, CalcType x0, CalcType y0, CalcType xstep, CalcType ystep);
};

#endif /* HDR_H_ */
