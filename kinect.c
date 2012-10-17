#include <stdio.h>
#include "image.h"
#include "kinect.h"

void kinect_capture_depth_image(freenect_device *dev, void *v_depth, uint32_t timestamp) {
  int x, y;
  uint16_t *depth = (uint16_t*)v_depth;

  for (y = 0; y < 480; y++) {
    for (x = 0; x < 640; x++) {
      image_set_pixel(kinect_depth_image, x, y, depth[y * 640 + x]);
    }
  }
}

int kinect_initialize() {
  if (kinect_initialized) {
    return 0;
  }

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

  kinect_depth_image = image_create(640, 480);

  freenect_set_led(kinect_device, LED_GREEN);
  freenect_set_depth_callback(kinect_device, kinect_capture_depth_image);
  freenect_set_depth_mode(kinect_device, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_11BIT));
  freenect_start_depth(kinect_device);

  kinect_initialized = 1;
  return 1;
}

int kinect_process_events() {
  if (!kinect_initialized) {
    return 0;
  }

  return freenect_process_events(kinect_context) >= 0;
}

void kinect_shutdown() {
  if (!kinect_initialized) {
    return;
  }

  freenect_stop_depth(kinect_device);
  freenect_set_led(kinect_device, LED_OFF);
  freenect_close_device(kinect_device);
  freenect_shutdown(kinect_context);
}
