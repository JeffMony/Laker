

#ifndef OPENGL_GL_FILTER_H_
#define OPENGL_GL_FILTER_H_

#include "frame_buffer.h"

namespace effect {

class Filter : public FrameBuffer {
 public:
  Filter(uint8_t* lut, int lut_width, int lut_height);
  ~Filter();

  /**
   * 设置滤镜透明度
   * @param intensity 透明度的值 0 ~ 100
   */
  void SetIntensity(int intensity = 100);

  /**
   * 更新一个滤镜
   * @param lut 滤镜lut的图片buffer
   * @param lut_width 滤镜lut的宽
   * @param lut_height 滤镜lut的高
   */
  void UpdateLut(uint8_t* lut, int lut_width, int lut_height);

 protected:
  void RunOnDrawTasks() override;

 private:
  /// 滤镜的纹理
  GLuint texture_id_;
  /// 滤镜的透明度
  int intensity_;
};

}  // namespace effect

#endif  // OPENGL_GL_FILTER_H_