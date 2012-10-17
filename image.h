#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"

struct Image_t {
  unsigned int width;
  unsigned int height;
  unsigned char *data;
};

typedef struct Image_t Image;

Image *Image_create(unsigned int, unsigned int);

void Image_destroy(Image *);

extern inline void Image_set_pixel(Image *, unsigned int, unsigned int, unsigned char);

extern inline unsigned char Image_get_pixel(Image *, unsigned int, unsigned int);

char Image_write_png(Image *, FILE *);

char Image_get_png(Image *, Buffer *);

char Image_downsample(Image *, Image *);

#endif
