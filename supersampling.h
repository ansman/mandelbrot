#ifndef SUPERSAMPLING_H_
#define SUPERSAMPLING_H_

#include <iostream>

#include "post_texture.h"
#include "coloration.h"

class Supersampling : public PostTexture {
    static const int SUB_PIXEL_SIZE = 4;

    bool getColor(color * data, unsigned int index, unsigned int size, Coloration::Color & c) const;

public:
    virtual void process(color * data, unsigned int width, unsigned int height) const;
};

#endif /* SUPERSAMPLING_H_ */
