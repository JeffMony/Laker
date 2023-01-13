
#ifndef OPENGL_GL_BACKGROUND_H_
#define OPENGL_GL_BACKGROUND_H_

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "gaussian_blur.h"
#include "OpenGL/sub_effect/image_result.h"

#define MIN_BLUR_VALUE 5
#define MAX_BLUR_VALUE 25

namespace effect {

enum BackgroundType {
  kNone = -1,
  /// 原始比例显示
  kBackgroundOriginalScaleDisplay = 0,
  /// color类型
  kBackgroundColor,
  /// 图片类型
  kBackgroundImage,
  /// 高斯模糊
  kBackgroundBlur
};

enum TextureWrapType {
  kClampToEdge,
  kRepeat
};

/// 背景设置
class Background {
 public:
  Background();
  ~Background();

  /**
   * 设置原始比例显示
   */
  void SetOriginalScaleDisplay();

  /**
   * 设置显示裁剪类型, 0 和 1
   */
  void SetFrameType(int frame_type);

  /**
   * 设置背景颜色
   * @param red 红色 0 ~ 255
   * @param green 绿色 0 ~ 255
   * @param blue 蓝色 0 ~ 255
   * @param alpha 暂时忽略,没用到
   */
  void SetColor(int red, int green, int blue, int alpha);

  /**
   * 设置背景图片
   * @path 背景图片地址, 只支持jpeg png
   */
  void SetImage(unsigned char* buffer, int width, int height);

  /**
   * 设置模糊大小
   * @blur 模糊程度 0 ~ 25
   */
  void SetBlur(int blur);

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
   * 获取与FrameBuffer绑定的纹理
   */
  GLuint GetTextureId() {
    return texture_id_;
  }

  /**
   * 设置背景纹理环绕
   * @param wrap
   */
  void SetTextureWrap(int wrap);

  GLuint ProcessImage(GLuint origin_texture_id, GLuint texture_id, int frame_width,
                      int frame_height, int target_width, int target_height, ImageResult* result);

 private:
  void CropVertexCoordinate(int source_width, int source_height,
                            int target_width, int target_height,
                            int frame_type, float* vertex_coordinate);

  /**
   * 删除图片加载的纹理
   */
  void DeleteBackgroundImageTexture();

  /**
   * 背景颜色重置为0
   */
  void ResetBackgroundColor();

  /**
   * 获取缩小后的纹理
   * @param texture_id 原始纹理
   * @param frame_width 纹理宽
   * @param frame_height 纹理高
   * @return 缩小后的纹理
   */
  GLuint GetScaleTexture(GLuint texture_id, int frame_width, int frame_height);

  void ProcessBackground(GLuint texture_id, int frame_width, int frame_height);

  GLuint GetBackgroundTextureId(GLuint texture_id, int frame_width, int frame_height);

  GLuint CreateFrameBuffer(int width, int height);
  void DeleteFrameBuffer();

 private:
  GLuint program_;
  GLuint second_program_;
  /// 裁剪模式 0 和 1
  int frame_type_;
  /// FrameBuffer的id
  GLuint frame_buffer_id_;
  /// 与FrameBuffer绑定的纹理id
  GLuint texture_id_;
  /// 纹理的宽, 用来判断是否需要重新设置FrameBuffer
  int source_width_;
  /// 纹理的高, 用来判断是否需要重新设置FrameBuffer
  int source_height_;
  /// 纹理的宽
  int frame_width_;
  /// 纹理的高
  int frame_height_;
  /// 目标的宽
  int target_width_;
  /// 目标的高
  int target_height_;
  /// 红色值 0 ~ 255
  int red_;
  /// 绿色值 0 ~ 255
  int green_;
  /// 蓝色值 0 ~ 255
  int blue_;
  /// 透明度
  int alpha_;
  /// 高斯模糊的程度 0 ~ 25
  int blur_;
  /// 旋转的角度
  float rotate_;
  /// 横向位移位置
  float translation_x_;
  /// 竖向位移位置
  float translation_y_;
  /// 缩放倍数
  float scale_;
  /// 高斯模糊执行
  GaussianBlur* gaussian_blur_;
  /// 缩小纹理使用的FrameBuffer
  FrameBuffer* scale_frame_buffer_;
  /// 图片背景时使用的纹理
  GLuint background_image_texture_;
  /// 图片的宽
  int image_width_;
  /// 图片的高
  int image_height_;
  /// 背景类型
  BackgroundType background_type_;
  /// 视频内容的顶点坐标
  GLfloat* vertex_coordinate_;
  /// 背景的顶点坐标
  GLfloat* background_vertex_coordinate_;
  /// 背景纹理环绕
  GLuint texture_wrap_;
};

}  // namespace effect

#endif  // OPENGL_GL_BACKGROUND_H_