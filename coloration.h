#ifndef COLORATION_H_
#define COLORATION_H_

#include <cassert>
#include <map>

#include "global.h"

namespace Coloration {
	void setupColoration();
	color * applyColor(CalcType * data, int width, int height);
};

#endif /* COLORATION_H_ */
