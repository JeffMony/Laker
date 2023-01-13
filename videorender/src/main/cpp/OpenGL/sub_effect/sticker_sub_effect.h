

#ifndef OPENGL_SUB_EFFECT_STICKER_SUB_EFFECT_H_
#define OPENGL_SUB_EFFECT_STICKER_SUB_EFFECT_H_

#include <string>
#include <vector>
#include <list>
#include "sub_effect.h"
#include "OpenGL/gl/image_buffer.h"
#include "OpenGL/gl/blend.h"

namespace effect {

class StickerSubEffect : public SubEffect {
 public:
  StickerSubEffect();

  virtual ~StickerSubEffect();

  /**
   * 设置贴纸的大小和地址
   * @param image 贴纸地址
   */
  int SetStickerInfo(const std::string& image);

  /**
   * 获取贴纸文件路径
   * @return 贴纸路径
   */
  std::string GetStickerInfo();

  /**
   * 设置贴纸中心点坐标
   * @param center_x
   * @param center_y
   */
  void SetCenter(float center_x, float center_y);

  /**
   * 设置贴纸旋转角度
   * @param rotate
   */
  void SetRotate(float rotate);

  /**
   * 设置贴纸放缩
   * @param scale
   */
  void SetScale(float scale);

  /**
   * 设置贴纸左右镜像
   * @param mirror true 左右镜像
   */
  void SetHorizontalMirror(bool mirror);

  /**
   * 获取贴纸是否左右镜像
   * @return bool  sticker is mirror
   */
  bool GetHorizontalMirror();

  /**
   * 设置贴纸上下镜像
   * @param mirror true 上下镜像
   */
  void SetVerticalMirror(bool mirror);

  /**
   * 获取贴纸是否上下镜像
   * @return bool  sticker is mirror
   */
  bool GetVerticalMirror();

  /**
   * 定义子特效处理方法, 这里由下面子特效重写
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

 protected:
  /// 大贴纸的宽
  float width_;
  /// 大贴纸的高
  float height_;
  /// 贴纸的中心点 center_x/center_y
  float center_x_;
  /// 贴纸显示的y位置
  float center_y_;
  /// 贴纸缩放的倍数
  float scale_;
  /// 旋转角度
  float rotate_;
  /// 图片buffer
  ImageBuffer* image_buffer_;
  /// 混合
  Blend* blend_;
  /// 左右镜像
  bool horizontal_mirror_;
  /// 上下镜像
  bool vertical_mirror_;
  /// sticker image path
  std::string image_path_;
};

}  // namespace effect

#endif  // OPENGL_SUB_EFFECT_STICKER_SUB_EFFECT_H_