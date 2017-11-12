#include <signal.h>
#include <stdio.h>
#include <mutex>
#include "kinect.h"

unsigned int kinect_running;
static int last_frame = 0;
static int frame = 0;
cv::Mat rgbMat;
cv::Mat depthMat;

std::mutex rgbMat_mutex;  // protects rgbMat
std::mutex depthMat_mutex;  // protects depthMat

cv::Mat getRGB()
{
  return rgbMat;
}

cv::Mat getDepth()
{
  return depthMat;
}

void kinect_handle_interrupt(int signal) {
  kinect_running = 0;
}

void kinect_trap_signals() {
  if (signal(SIGINT, kinect_handle_interrupt) == SIG_IGN) {
    signal(SIGINT, SIG_IGN);
  }

  if (signal(SIGTERM, kinect_handle_interrupt) == SIG_IGN) {
    signal(SIGTERM, SIG_IGN);
  }
}

void kinect_capture_video_image(freenect_device *dev, void *v_video, uint32_t timestamp) {
  std::lock_guard<std::mutex> lock(rgbMat_mutex);
  uint8_t* rgb = static_cast<uint8_t*>(v_video);
  rgbMat.data = rgb;
  cv::cvtColor(rgbMat, rgbMat, cv::COLOR_RGB2BGR);
  frame++;
}

void kinect_capture_depth_image(freenect_device *dev, void *v_depth, uint32_t timestamp) {
  std::lock_guard<std::mutex> lock(depthMat_mutex);
  uint16_t* depth = static_cast<uint16_t*>(v_depth);
  for(int y = 0; y < depthMat.rows; ++y)
  {
    for (int x = 0; x < depthMat.cols; ++x)
    {
        uint16_t val = depth[y * 640 + x];
        val = std::max(0, val - 0x1FF);
        if (val > 0x1FF)
            val = 0x1FF;
        depthMat.at<uint8_t>(y,x) = 0xFF - (uint8_t)(val >> 1);
    }
  }
  frame++;
}


int kinect_initialize() {
  if (kinect_initialized) {
    return 0;
  }

  rgbMat = cv::Mat(cv::Size(640, 480), CV_8UC3, cv::Scalar(0));
  depthMat = cv::Mat(cv::Size(640,480), CV_8UC1, cv::Scalar(0));
  kinect_running = 1;
  kinect_trap_signals();

  if (freenect_init(&kinect_context, NULL) < 0) {
    fprintf(stderr, "freenect_init() failed\n");
    return 0;
  }

  freenect_select_subdevices(kinect_context, (freenect_device_flags)(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));

  if (freenect_num_devices(kinect_context) < 1) {
    fprintf(stderr, "no devices found\n");
    freenect_shutdown(kinect_context);
    return 0;
  }

  if (freenect_open_device(kinect_context, &kinect_device, 0) < 0) {
    fprintf(stderr, "can't open device\n");
    freenect_shutdown(kinect_context);
    return 0;
  }

  freenect_set_led(kinect_device, LED_GREEN);
#ifdef DEPTH
  freenect_set_ir_brightness(kinect_device, 50);
  freenect_set_depth_callback(kinect_device, kinect_capture_depth_image);
  freenect_set_depth_mode(kinect_device, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_11BIT));
  freenect_start_depth(kinect_device);
#else
  freenect_set_ir_brightness(kinect_device, 1);
#endif
  freenect_set_flag(kinect_device, FREENECT_AUTO_EXPOSURE, FREENECT_ON);
  freenect_set_flag(kinect_device, FREENECT_AUTO_WHITE_BALANCE, FREENECT_ON);
  freenect_set_video_callback(kinect_device, kinect_capture_video_image);
  freenect_set_video_mode(kinect_device, freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB));
  freenect_start_video(kinect_device);

  kinect_initialized = 1;
  return 1;
}

int kinect_process_events() {
  if (!kinect_initialized) {
    return 0;
  }

  for (;;) {
    if (!kinect_running) {
      return 0;
    }

    int result = freenect_process_events(kinect_context);
    if (result < 0) {
      return result;
    }
    if (last_frame != frame) {
        last_frame = frame;
        return 1;
    }

  }

  return 0;
}

void kinect_shutdown() {
  if (!kinect_initialized) {
    return;
  }

#ifdef DEPTH
  freenect_stop_depth(kinect_device);
#endif
  freenect_stop_video(kinect_device);
  freenect_set_led(kinect_device, LED_OFF);
  freenect_close_device(kinect_device);
  freenect_shutdown(kinect_context);
}
