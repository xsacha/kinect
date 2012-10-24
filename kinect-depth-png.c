#include <signal.h>
#include <stdio.h>
#include "image.h"
#include "kinect.h"

int running = 1;

void handle_interrupt(int signal) {
  running = 0;
}

void trap_signals() {
  if (signal(SIGINT, handle_interrupt) == SIG_IGN) {
    signal(SIGINT, SIG_IGN);
  }

  if (signal(SIGTERM, handle_interrupt) == SIG_IGN) {
    signal(SIGTERM, SIG_IGN);
  }
}

void write_depth_png_netstring(FILE *file) {
  Buffer *buffer = Buffer_create();
  Image *image = Image_create(320, 240);

  Image_downsample(kinect_depth_image, image);

  if (Image_get_png(image, buffer)) {
    fprintf(file, "%lu:", buffer->size);
    Buffer_write(buffer, file);
    fputc(',', file);
  }

  Image_destroy(image);
  Buffer_destroy(buffer);
}

int main() {
  if (!kinect_initialize()) {
    return 1;
  }

  trap_signals();

  while (running && kinect_process_events()) {
    write_depth_png_netstring(stdout);
  }

  kinect_shutdown();
  return 0;
}
