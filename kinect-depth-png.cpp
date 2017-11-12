#include <stdio.h>
#include "base64.h"
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
    auto basedStr = base64_encode(buff.data(), buff.size());
    fprintf(stdout, "%zu:%s,", basedStr.size(), basedStr.c_str());
  }

  kinect_shutdown();
  return 0;
}
