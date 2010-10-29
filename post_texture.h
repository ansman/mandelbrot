#ifndef POST_TEXTURE_H_
#define POST_TEXTURE_H_

#include "global.h"

class PostTexture {
public:
	virtual void process(color * data, unsigned int width, unsigned int height) const = 0;
};

#endif /* POST_TEXTURE_H_ */
