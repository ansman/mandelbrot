#include "global.h"

#include "hdr.h"

PreTexture * preTextureProcessing[] = {
    new HDR(),
    NULL
};

#include "supersampling.h"

PostTexture * postTextureProcessing[] = {
    //new Supersampling(),
    NULL
};

