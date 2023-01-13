

#ifndef OPENGL_SUB_EFFECT_COLOR_ADJUST_SUB_EFFECT_H_
#define OPENGL_SUB_EFFECT_COLOR_ADJUST_SUB_EFFECT_H_

#include "sub_effect.h"
#include "OpenGL/gl/color_adjust.h"

namespace effect {

class ColorAdjustSubEffect : public SubEffect {

 public:
  ColorAdjustSubEffect();

  virtual ~ColorAdjustSubEffect();

  void SetBrightnessLevel(float brightness_level);

  void SetContrastLevel(float contrast_level);

  void SetTemperatureLevel(float temperature_level);

  void SetSaturationLevel(float saturation_level);

  void SetGrainLevel(float grain_level);

  void SetSharpnessLevel(float sharpness_level);

  void SetMethodBit(int method_bit);

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
  ColorAdjust *color_adjust_;
};

}  // namespace effect

#endif  // OPENGL_SUB_EFFECT_COLOR_ADJUST_SUB_EFFECT_H_