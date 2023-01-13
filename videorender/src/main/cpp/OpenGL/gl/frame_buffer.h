

#ifndef OPENGL_GL_FRAME_BUFFER_H_
#define OPENGL_GL_FRAME_BUFFER_H_

#include "opengl.h"

namespace effect {

class FrameBuffer : public OpenGL {
 public:
  /// 普通构造函数, shader为 DEFAULT_VERTEX_SHADER DEFAULT_FRAGMENT_SHADER
  FrameBuffer();
  /// 不设置viewport宽高, 自定义shader
  explicit FrameBuffer(const char* vertex, const char* fragment);
  /// 设置viewport宽高, 自定义shader
  explicit FrameBuffer(int width, int height, const char* vertex, const char* fragment);
  // 析构
  virtual ~FrameBuffer();

  /**
   * 设置FrameBuffer的处理的开始时间,如果小于这个时间,会返回原始的纹理
   * @param time 开始时间
   */
  void SetStartTime(int time);

  /**
   * 设置FrameBuffer的处理的结束时间,如果大于这个时间,会返回原始的纹理
   * @param time 结束时间
   */
  void SetEndTime(int time);

  /**
   * 把texture_id经过FrameBuffer处理一遍
   * @param data rgba数据
   * @param width 图片的宽
   * @param height 图片的高
   * @param current_time 当前的处理时间
   * @return FrameBuffer处理过的纹理
   */
  GLuint ProcessImage(uint8_t* data, int width, int height, int64_t current_time = 0);

  GLuint ProcessImageWithMatrix(uint8_t* data, int width, int height, const GLfloat* vertex_coordinate,
                                const GLfloat* texture_coordinate, GLfloat* matrix, int64_t current_time = 0);

  /**
   * 把texture_id经过FrameBuffer处理一遍
   * @param texture_id 输入的纹理
   * @param width 纹理的宽
   * @param height 纹理的高
   * @param current_time 当前的处理时间
   * @return FrameBuffer处理过的纹理
   */
  GLuint ProcessImage(GLuint texture_id, int width, int height, int64_t current_time = 0);

  /**
   * 把texture_id经过FrameBuffer处理一遍
   * @param texture_id 输入的纹理
   * @param width 纹理的宽
   * @param height 纹理的高
   * @param matrix 传入到shader中的4x4的矩阵
   * @param current_time 当前的处理时间
   * @return FrameBuffer处理过的纹理
   */
  GLuint ProcessImage(GLuint texture_id, int width, int height, GLfloat* matrix, int64_t current_time = 0);

  /**
   * 把texture_id经过FrameBuffer处理一遍
   * @param texture_id 输入的纹理
   * @param width 纹理的宽
   * @param height 纹理的高
   * @param vertex_coordinate 自定义的顶点坐标
   * @param texture_coordinate 自定义的纹理坐标
   * @param current_time 当前的处理时间
   * @return FrameBuffer处理过的纹理
   */
  GLuint ProcessImage(GLuint texture_id, int width, int height, const GLfloat* vertex_coordinate,
                      const GLfloat* texture_coordinate, int64_t current_time = 0);

  /**
   * 把texture_id经过FrameBuffer处理一遍
   * @param texture_id 输入的纹理
   * @param width 纹理的宽
   * @param height 纹理的高
   * @param vertex_coordinate 自定义的顶点坐标
   * @param texture_coordinate 自定义的纹理坐标
   * @param matrix 传入到shader中的4x4的矩阵
   * @param current_time 当前的处理时间
   * @return FrameBuffer处理过的纹理
   */
  GLuint ProcessImage(GLuint texture_id, int width, int height, const GLfloat* vertex_coordinate,
                      const GLfloat* texture_coordinate, GLfloat* matrix, int64_t current_time = 0);

  /**
   * 获取与FrameBuffer绑定的纹理
   */
  GLuint GetTextureId() {
    return texture_id_;
  }

  /**
   * 获取FrameBuffer Id
   */
  GLuint GetFrameBufferId() {
    return frame_buffer_id_;
  }

 private:
  /**
   * 创建FrameBuffer
   */
  void CreateFrameBuffer(int width, int height);

  /**
   * 删除FrameBuffer和绑定的纹理
   */
  void DeleteFrameBuffer();

 private:
  /// 开始时间, 如果小于这个时间会返回原始纹理
  GLuint start_time_;
  /// 结束时间, 如果超过这个时间会返回原始纹理
  GLuint end_time_;
  /// FrameBuffer的id
  GLuint frame_buffer_id_;
  /// 与FrameBuffer绑定的纹理id
  GLuint texture_id_;
  /// FrameBuffer的宽
  int width_;
  /// FrameBuffer的高
  int height_;
  /// 图片的纹理
  GLuint image_texture_id_;
};

}
#endif  // OPENGL_GL_FRAME_BUFFER_H_