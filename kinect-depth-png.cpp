#include <stdio.h>
#include "kinect.h"
#include "image.h"
#include "buffer.h"
//#include <opencv2/highgui/highgui.hpp>

void write_depth_png_netstring(FILE *file) {
#ifdef DEPTH
  cv::Mat mat = getDepth();
#else
  cv::Mat mat = getRGB();
#endif
  if (mat.empty())
  {
    return;
  }

  Buffer *buffer = Buffer_create();
  if (Image_get_png(getDepth(), buffer)) {
    fprintf(file, "%zu:", buffer->size);
    Buffer_write(buffer, file);
    fputc(',', file);
  }

  Buffer_destroy(buffer);
}

int main() {
  if (!kinect_initialize()) {
    return 1;
  }

  while (kinect_process_events()) {
    write_depth_png_netstring(stdout);
  }

  kinect_shutdown();
  return 0;
}
