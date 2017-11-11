#pragma once

#include <stdio.h>
#include "buffer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

char Image_get_png(cv::Mat img, Buffer *);

