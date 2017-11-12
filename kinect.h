#pragma once

//#define DEPTH

#include <libfreenect.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

static freenect_context *kinect_context;
static freenect_device *kinect_device;
static int kinect_initialized = 0;

int kinect_initialize();
int kinect_process_events();
void kinect_shutdown();

cv::Mat getRGB();
cv::Mat getDepth();

