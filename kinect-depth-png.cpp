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
    cv::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(85);
    std::vector<uchar> buff;
    cv::imencode(".jpg", mat, buff, compression_params);
    fprintf(stdout, "%zu:", buff.size());
    for (auto i = buff.begin(); i != buff.end(); ++i)
      fputc(*i, stdout);
    fputc(',', stdout);
  }

  kinect_shutdown();
  return 0;
}
