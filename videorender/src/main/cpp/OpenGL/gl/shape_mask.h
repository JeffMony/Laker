
#ifndef OPENGL_GL_SHAPE_MASK_H_
#define OPENGL_GL_SHAPE_MASK_H_

#include "frame_buffer.h"

namespace effect {

/// 视频蒙版的形状
enum ShapeType {
  NONE_SHAPE = 0,
  RECTANGLE,  /// 矩形
  STAR,       /// 五角星
  ROUND,      /// 圆形
  HEART,      /// 心形
};

class ShapeMaskBuffer : public FrameBuffer {
 public:
  ShapeMaskBuffer(int width, int height, const char *vertex_shader, const char *fragment_shader);

  ~ShapeMaskBuffer();

  void SetShapeType(ShapeType shape_type);

  void SetRoundArgument(float center_x, float center_y, float radius);

  void SetStarArgument(float center_x, float center_y, float r_min, float r_max);

  void SetHeartArgument(float center_x, float center_y, float scale_x, float scale_y);

  void SetRectArgument(float x, float y, float w, float h);

 protected:
  void RunOnDrawTasks() override;

 private:
  ShapeType shape_type_;
  int width_;
  int height_;
  float center_x_;
  float center_y_;
  float radius_;
  float r_min_;
  float r_max_;
  float scale_x_;
  float scale_y_;
  float rect_w_;
  float rect_h_;
};

class ShapeMask {
 public:
  ShapeMask();
  ~ShapeMask();

  void SetShapeType(ShapeType shape_type);

  void SetRoundArgument(float center_x, float center_y, float radius);

  void SetStarArgument(float center_x, float center_y, float r_min, float r_max);

  void SetHeartArgument(float center_x, float center_y, float scale_x, float scale_y);

  void SetRectArgument(float x, float y, float w, float h);

  void SetBackgroundImage(unsigned char *buffer, int width, int height);

  int GetWidth();

  int GetHeight();

  GLuint ProcessImage(GLuint texture_id, int width, int height);

 private:
  ShapeType shape_type_;
  GLuint bg_image_texture_id_;
  int bg_image_width_;
  int bg_image_height_;
  float center_x_;
  float center_y_;
  float radius_;
  float r_min_;
  float r_max_;
  float scale_x_;
  float scale_y_;
  float rect_w_;
  float rect_h_;
  ShapeMaskBuffer *shape_mask_buffer_;
};

}

#endif  // OPENGL_GL_SHAPE_MASK_H_