#include <stdio.h>
#include "kinect.h"

int main() {
  if (!kinect_initialize()) {
    return 1;
  }

  while (kinect_process_events()) {
#ifdef DEPTH
    cv::Mat mat = getDepth();
#else
    cv::Mat mat = getRGB();
#endif
    if (mat.empty())
    {
      continue;
    }
    std::vector<uchar> buff;
    cv::imencode(".png", mat, buff);
    fprintf(stdout, "%zu:", buff.size());
    for (auto i = buff.begin(); i != buff.end(); ++i)
      fputc(*i, stdout);
    fputc(',', stdout);
  }

  kinect_shutdown();
  return 0;
}
