
#ifndef OPENGL_GL_GAUSSIAN_BLUR_H_
#define OPENGL_GL_GAUSSIAN_BLUR_H_

#include <string>

#include "common.h"
#include "frame_buffer.h"

/// 高斯模糊

namespace effect {

class VerticalGaussianBlur : public FrameBuffer {
 public:
  VerticalGaussianBlur(int width, int height, const char *vertex_shader,
                       const char *fragment_shader);
  virtual ~VerticalGaussianBlur();

  void SetBlurParam(int radius, float offset, float weight);

 protected:
  void RunOnDrawTasks() override;

 private:
  int width_;
  int height_;
  int blur_radius_;
  float blur_offset_;
  float sum_weight_;
};

class HorizontalGaussianBlur : public FrameBuffer {
 public:
  HorizontalGaussianBlur(int width, int height, const char *vertex_shader,
                         const char *fragment_shader);
  virtual ~HorizontalGaussianBlur();

  void SetBlurParam(int radius, float offset, float weight);

 protected:
  void RunOnDrawTasks() override;

 private:
  int width_;
  int height_;
  int blur_radius_;
  float blur_offset_;
  float sum_weight_;
};

class GaussianBlur {
 public:
  explicit GaussianBlur(int width, int height);
  ~GaussianBlur();

  void SetBlurOffset(float offset);

  void SetBlurRadius(int radius);

  int ProcessImage(int texture_id);

 private:
  int width_;
  int height_;
  float blur_offset_;
  int blur_radius_;
  VerticalGaussianBlur *vertical_blur_;
  HorizontalGaussianBlur *horizontal_blur_;
};

}  // namespace effect

#endif  // OPENGL_GL_GAUSSIAN_BLUR_H_