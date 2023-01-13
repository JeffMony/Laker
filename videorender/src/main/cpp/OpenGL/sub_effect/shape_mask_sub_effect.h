

#ifndef OPENGL_SUB_EFFECT_SHAPE_MASK_SUB_EFFECT_H_
#define OPENGL_SUB_EFFECT_SHAPE_MASK_SUB_EFFECT_H_

#include "sub_effect.h"
#include "OpenGL/gl/shape_mask.h"

namespace effect {

class ShapeMaskSubEffect : public SubEffect {
 public:
  ShapeMaskSubEffect();
  ~ShapeMaskSubEffect();

  /**
   * 设置蒙版形状
   * @param shape_type
   */
  void SetShapeType(ShapeType shape_type);

  /**
   * 设置圆参数
   * @param center_x
   * @param center_y
   * @param radius
   */
  void SetRoundArgument(float center_x, float center_y, float radius);

  /**
   * 设置五角星的参数
   * @param center_x
   * @param center_y
   * @param rmin
   * @param rmax
   */
  void SetStarArgument(float center_x, float center_y, float r_min, float r_max);

  /**
   * 设置心形的参数
   * @param center_x
   * @param center_y
   * @param scale_x
   * @param scale_y
   */
  void SetHeartArgument(float center_x, float center_y, float scale_x, float scale_y);

  /**
   * 设置矩形的参数
   * @param x
   * @param y
   * @param w
   * @param h
   */
  void SetRectArgument(float x, float y, float w, float h);

  /**
   * 设置处理的图片
   * @param path
   */
  void SetBackgroundImagePath(const std::string &path);

  GLuint ProcessImage(const std::list<SubEffect *> &sub_effects,
                      int origin_texture_id,
                      int texture_id,
                      int frame_width,
                      int frame_height,
                      int target_width,
                      int target_height,
                      int64_t current_time,
                      ImageResult *result) override;

 private:
  ShapeMask *shape_mask_;

};

}

#endif  // OPENGL_SUB_EFFECT_SHAPE_MASK_SUB_EFFECT_H_