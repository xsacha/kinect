#include <signal.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
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

void clear_screen(FILE *file) {
  fprintf(file, "\x1B[2J");
}

struct winsize reset_frame(FILE *file) {
  struct winsize w;
  ioctl(fileno(file), TIOCGWINSZ, &w);
  fprintf(file, "\x1B[1;1H");
  return w;
}

void draw_depth_image(FILE *file, int width, int height) {
  image *img = image_create(width, height);
  image_downsample(kinect_depth_image, img);

  int x, y;
  unsigned char pixel, c;

  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      pixel = image_get_pixel(img, x, y);

      if (pixel < 0x40) {
        c = '%';
      } else if (pixel < 0x80) {
        c = '+';
      } else if (pixel < 0xC0) {
        c = '.';
      } else {
        c = ' ';
      }

      fputc(c, file);
    }
    fputc('\n', file);
  }

  image_destroy(img);
}

int main() {
  if (!kinect_initialize()) {
    return 1;
  }

  trap_signals();
  clear_screen(stdout);

  while (running && kinect_process_events()) {
    struct winsize w = reset_frame(stdout);
    draw_depth_image(stdout, w.ws_col, w.ws_row - 1);
  }

  kinect_shutdown();
  return 0;
}
