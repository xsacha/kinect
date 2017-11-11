#include <png.h>
#include <math.h>
#include "image.h"


char Image_write_png_internal(cv::Mat mat, void *write_io_ptr, png_rw_ptr write_data_fn, png_flush_ptr output_flush_fn) {
  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) {
    return 0;
  }

  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_write_struct(&png, (png_infopp)NULL);
    return 0;
  }

  if (setjmp(png_jmpbuf(png))) {
    png_destroy_info_struct(png, (png_infopp)&info);
    png_destroy_write_struct(&png, (png_infopp)&info);
    return 0;
  }

    png_set_write_fn(png, write_io_ptr, write_data_fn, output_flush_fn);

  png_set_IHDR(png, info, mat.cols, mat.rows, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_write_info(png, info);

  int x, y;

  cv::Mat rgb;
  if (mat.channels() == 1)
  {
    cv::cvtColor(mat, rgb, cv::COLOR_GRAY2BGR);
  }
  else
  {
    rgb = mat;
  }
  for (y = 0; y < mat.rows; y++) {
    png_write_row(png, (png_bytep)rgb.ptr(y);
  }

  free(row);
  png_write_end(png, NULL);
  png_destroy_info_struct(png, (png_infopp)&info);
  png_destroy_write_struct(&png, (png_infopp)&info);
  return 1;
}

void Image_get_png_write_data(png_structp png, png_bytep data, png_size_t length) {
  Buffer *buffer = (Buffer *)png_get_io_ptr(png);
  Buffer_append(buffer, length, data);
}

void Image_get_png_flush_data(png_structp png) {
}

char Image_get_png(cv::Mat img, Buffer *buffer) {
  return Image_write_png_internal(img, (void *)buffer, Image_get_png_write_data, Image_get_png_flush_data);
}

