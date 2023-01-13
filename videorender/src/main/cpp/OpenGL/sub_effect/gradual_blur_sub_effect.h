

#ifndef OPENGL_SUB_EFFECT_GRADUAL_BLUR_SUB_EFFECT_H_
#define OPENGL_SUB_EFFECT_GRADUAL_BLUR_SUB_EFFECT_H_

#include <string>
#include <list>
#include "sub_effect.h"
#include "OpenGL/gl/gradual_blur.h"

namespace effect {

class GradualBlurSubEffect : public SubEffect {
 public:
  GradualBlurSubEffect();

  virtual ~GradualBlurSubEffect();

  /**
   * 设置虚化类型
   * @param type
   */
  void SetBlurType(BlurType type);

  /**
   * 是否渐进式虚化
   * @param is_gradual
   */
  void SetGradient(bool is_gradual);

  /**
   * 渐进式虚化区域的形状
   * @param shape
   */
  void SetGradualShape(GradualShape shape);

  /**
   * 渐进式虚化区域为圆形的相关参数
   * @param center_x    圆心x坐标
   * @param center_y    圆心y坐标
   * @param in_radius   内圆的半径
   * @param out_radius  外圆的半径
   */
  void SetCircleParams(float center_x, float center_y, float in_radius, float out_radius);

  /**
   * 渐进式虚化区域为矩形的相关参数
   * @param out_start_y  外层y起始坐标
   * @param out_end_y    外层y结束坐标
   * @param in_start_y   内层y起始坐标
   * @param in_end_y     内层y结束坐标
   */
  void SetRectParams(float out_start_y, float out_end_y, float in_start_y, float in_end_y);

  /**
   * 是否将渐进区域画线
   * @param draw_line
   */
  void SetDrawLine(bool draw_line);

  /**
   * 设置虚化的层级 0 ~ 100
   * @param level
   * @param gradual_level
   */
  void SetLevel(int level, int gradual_level);

  /**
   * 设置虚化的背景
   * @param path
   */
  void SetBackgroundImagePath(const std::string &path);

  /**
   * 设置前景照片路径
   * @param path
   */
  void SetForegroundImagePath(const std::string &path);

  GLuint ProcessImage(const std::list<SubEffect *> &sub_effects,
                      int origin_texture_id,
                      int texture_id,
                      int frame_width, int frame_height,
                      int target_width, int target_height,
                      int64_t current_time, ImageResult *result) override;

 private:
  GradualBlur *gradual_blur_;

};
}  // namespace effect

#endif  // OPENGL_SUB_EFFECT_GRADUAL_BLUR_SUB_EFFECT_H_