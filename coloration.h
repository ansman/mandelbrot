#ifndef COLORATION_H_
#define COLORATION_H_

#include <cassert>
#include <map>
#include <iostream>

#include "global.h"

namespace Coloration {
	struct Color {
		float percent;
		color r, g, b;
		Color() : percent(0), r(0), g(0), b(0) {}
		Color(float percent, color r, color g, color b) : percent(percent), r(r), g(g), b(b) {}
		
		Color operator+(const Color & ref) const {
			return Color(percent + ref.percent, r + ref.r, g + ref.g, b + ref.b);
		}
		
		Color & operator+=(const Color & ref) {
			percent += ref.percent;
			r += ref.r;
			g += ref.g;
			b += ref.b;
			return *this;
		}
		
		Color operator/(int i) const {
			return Color(percent/i, r/i, g/i, b/i);
		}
		
		Color & operator/=(int i) {
			percent /= i;
			r /= i;
			g /= i;
			b /= i;
			return *this;
		}
	};
	
	inline std::ostream & operator<<(std::ostream & out, Coloration::Color & ref) {
		unsigned int r = ref.r, g = ref.g, b = ref.b;
		out << "Color: (" << ref.percent << ", " << r << ", " << g << ", " << b << ")";
		return out;
	}
	
	void setupColoration();
	color * applyColor(CalcType * data, int width, int height);
};

#endif /* COLORATION_H_ */
