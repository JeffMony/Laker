
#ifndef OPENGL_SUB_EFFECT_BACKGROUND_SUB_EFFECT_H_
#define OPENGL_SUB_EFFECT_BACKGROUND_SUB_EFFECT_H_

#include <string>
#include <list>

#include "sub_effect.h"
#include "OpenGL/gl/background.h"

namespace effect {

class BackgroundSubEffect : public SubEffect {
 public:
  BackgroundSubEffect();
  virtual ~BackgroundSubEffect();

  /**
   * 设置原始比例显示
   */
  void SetOriginalScaleDisplay();

  /**
   * 设置显示裁剪类型, 0 和 1
   */
  void SetFrameType(int frame_type);

  /**
   * 设置模糊程度
   */
  void SetBlur(int blur);

  /**
   * 设置颜色背景
   */
  void SetColor(int red, int green, int blue, int alpha);

  /**
   * 设置视频旋转角度
   * @param rotate 0 ~ 360度
   */
  void SetRotate(float rotate);

  /**
   * 设置位移
   * @param x 横向位移位置
   * @param y 竖向位移位置
   */
  void SetTranslation(float x, float y);

  /**
   * 设置缩放
   * @param scale 缩放倍数
   */
  void SetScale(float scale);

  /**
   * 设置图片背景
   */
  int SetImagePath(const std::string& path);

  /**
   * 设置纹理环绕
   */
  void SetTextureWrap(int wrap);

  /**
   * 定义子特效处理方法, 这里由下面子特效重写
   * @param face_detection 人脸检测接口
   * @param sub_effects 当前这个特效中的所有子特效
   * @param origin_texture_id 原始的纹理id
   * @param texture_id 子特效处理过的纹理id
   * @param frame_width 纹理宽
   * @param frame_height 纹理高
   * @param target_width 目标宽
   * @param target_height 目前高
   * @param current_time 当前时间
   */
  GLuint ProcessImage(const std::list<SubEffect*>& sub_effects,
                      int origin_texture_id, int texture_id, int frame_width, int frame_height,
                      int target_width, int target_height,
                      int64_t current_time, ImageResult* result) override;

 private:
  Background* background_;
  int target_width_;
  int target_height_;
  int source_width_;
  int source_height_;
  int image_width_;
  int image_height_;
};

}  // namespace effect


#endif  // OPENGL_SUB_EFFECT_BACKGROUND_SUB_EFFECT_H_