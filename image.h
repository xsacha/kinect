#pragma once

#include <stdio.h>
#include "buffer.h"
#include <opencv2/core/core.hpp>

typedef uint8_t Pixel;

char Image_get_png(cv::Mat img, Buffer *);

