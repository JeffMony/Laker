

#ifndef OPENGL_GL_COLOR_ADJUST_H_
#define OPENGL_GL_COLOR_ADJUST_H_

#include "frame_buffer.h"
#include <string>

namespace effect {

enum ColorType {
  COLOR_NONE = 0,
  COLOR_BRIGHTNESS,      /// 亮度
  COLOR_CONTRAST,        /// 对比度
  COLOR_TEMPERATURE,     /// 色温
  COLOR_SATURATION,      /// 饱和度
  COLOR_SHARPNESS,       /// 锐度
};

class ColorAdjustBuffer : public FrameBuffer {
 public:
  ColorAdjustBuffer(int width, int height, const char *vertex_shader, const char *fragment_shader);

  virtual ~ColorAdjustBuffer();

  void SetBrightnessLevel(float brightness_level);

  void SetContrastLevel(float contrast_level);

  void SetTemperatureLevel(float temperature_level);

  void SetSaturationLevel(float saturation_level);

  void SetGrainLevel(float grain_level);

  void SetSharpnessLevel(float sharpness_level);

  void SetMethodBit(int method_bit);

 protected:
  void RunOnDrawTasks() override;

 private:
  int width_;
  int height_;
  float brightness_level_;
  float contrast_level_;
  float temperature_level_;
  float saturation_level_;
  float grain_level_;
  float sharpness_level_;
  int method_bit_;
  bool adjust_bright_;
  bool adjust_contrast_;
  bool adjust_temperature_;
  bool adjust_saturation_;
  bool adjust_grain_;
};


class ColorAdjust {

 public:
  ColorAdjust();
  virtual ~ColorAdjust();

  /**
   * 调节范围都是参考竞品得到的，如果是负的，说明是可以负向操作的，都是非负，说明只能单向操作
   */

  /**
   * 调节范围是 -100 ~ 100
   * @param brightness_level
   */
  void SetBrightnessLevel(float brightness_level);

  /**
   * 调节范围是 -100 ~ 100
   * @param contrast_level
   */
  void SetContrastLevel(float contrast_level);

  /**
   * 调节范围是 -100 ~ 100
   * @param temperature_level
   */
  void SetTemperatureLevel(float temperature_level);

  /**
   * 调节范围是 -100 ~ 100
   * @param saturation_level
   */
  void SetSaturationLevel(float saturation_level);

  /**
   * 调节范围是 0 ~ 100
   * @param grain_level
   */
  void SetGrainLevel(float grain_level);

  /**
   * 调节范围是 0 ~ 100
   * @param sharpness_level
   */
  void SetSharpnessLevel(float sharpness_level);

  void SetMethodBit(int method_bit);

  /**
   * 设置要处理的图片
   * @param buffer
   * @param width
   * @param height
   */
  void SetBackgroundImage(unsigned char *buffer, int width, int height);

  int GetWidth();

  int GetHeight();

  GLuint ProcessImage(GLuint texture_id, int width, int height);

 private:
  float brightness_level_;
  float contrast_level_;
  float temperature_level_;
  float saturation_level_;
  float grain_level_;
  float sharpness_level_;
  int method_bit_;
  GLuint bg_image_texture_id_;
  int bg_image_width_;
  int bg_image_height_;
  ColorAdjustBuffer *normal_color_adjust_buffer_;
  ColorAdjustBuffer *sharp_color_adjust_buffer_;

};

}  // namespace effect

#endif  // OPENGL_GL_COLOR_ADJUST_H_