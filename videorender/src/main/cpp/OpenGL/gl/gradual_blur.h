

#ifndef OPENGL_GL_GRADUAL_BLUR_H_
#define OPENGL_GL_GRADUAL_BLUR_H_

#include <string>
#include "frame_buffer.h"
#include "process_buffer.h"

namespace effect {

enum GradualShape {
  NONE = 0,
  CIRCLE,
  RECT,
};

enum BlurType {
  GAUSSIAN,
  FROST_GLASS,
};

class VerticalBlur : public FrameBuffer {
 public:
  VerticalBlur(int width, int height, const char *vertex_shader, const char *fragment_shader);

  virtual ~VerticalBlur();

  void SetDrawLine(bool draw_line);

  void SetGradualShape(GradualShape shape);

  void SetBlurParams(int blur_level, float blur_offset, float weight, int gradual_blur_level,
                     float gradual_blur_offset, float gradual_weight);

  void SetShapeParams(float shape1, float shape2, float shape3, float shape4);

 protected:
  void RunOnDrawTasks() override;

 private:
  int width_;
  int height_;
  GradualShape shape_;
  bool draw_line_;
  int blur_level_;
  float blur_offset_;
  float weight_;
  int gradual_blur_level_;
  float gradual_blur_offset_;
  float gradual_weight_;
  float shape1_;
  float shape2_;
  float shape3_;
  float shape4_;
};

class HorizontalBlur : public FrameBuffer {
 public:
  HorizontalBlur(int width, int height, const char *vertex_shader, const char *fragment_shader);

  virtual ~HorizontalBlur();

  void SetDrawLine(bool draw_line);

  void SetGradualShape(GradualShape shape);

  void SetBlurParams(int blur_level, float blur_offset, float weight, int gradual_blur_level,
                     float gradual_blur_offset, float gradual_weight);

  void SetShapeParams(float shape1, float shape2, float shape3, float shape4);

 protected:
  void RunOnDrawTasks() override;

 private:
  int width_;
  int height_;
  GradualShape shape_;
  bool draw_line_;
  int blur_level_;
  float blur_offset_;
  float weight_;
  int gradual_blur_level_;
  float gradual_blur_offset_;
  float gradual_weight_;
  float shape1_;
  float shape2_;
  float shape3_;
  float shape4_;
};

class GradualBlur {
 public:
  GradualBlur();

  ~GradualBlur();

  void SetBlurType(BlurType type);

  void SetGradient(bool is_gradual);

  void SetGradualShape(GradualShape shape);

  void SetCircleParams(float center_x, float center_y, float in_radius, float out_radius);

  void SetRectParams(float out_start_y, float out_end_y, float in_start_y, float in_end_y);

  void SetDrawLine(bool draw_line);

  void SetLevel(int level, int gradual_level);

  void SetBackgroundImage(unsigned char *buffer, int width, int height);

  void SetForegroundImage(unsigned char *buffer, int width, int height);

  GLuint ProcessImage(GLuint texture_id, int width, int height);

  int GetWidth();

  int GetHeight();

 private:
  float GetWeight(int level);

 private:
  BlurType blur_type_;
  GradualShape gradual_shape_;
  bool draw_line_;
  bool is_gradual_;
  float center_x_;
  float center_y_;
  float in_radius_;
  float out_radius_;
  float out_start_y_;
  float out_end_y_;
  float in_start_y_;
  float in_end_y_;
  int level_;
  int gradual_level_;
  int bg_image_width_;
  int bg_image_height_;
  float weight_;
  float gradual_weight_;
  GLuint bg_image_texture_id_;
  GLuint fg_image_texture_id_;

  VerticalBlur *vertical_blur_;
  HorizontalBlur *horizontal_blur_;
  ProcessBuffer *mix_;
};

}  // namespace effect

#endif  // OPENGL_GL_GRADUAL_BLUR_H_