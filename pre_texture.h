#ifndef PRE_TEXTURE_H_
#define PRE_TEXTURE_H_

#include "global.h"

class PreTexture {
public:
	virtual void process(CalcType * data, unsigned int width, unsigned int height, CalcType x0, CalcType y0, CalcType xstep, CalcType ystep) = 0;
};

#endif /* PRE_TEXTURE_H_ */
