

#ifndef OPENGL_GL_OPENGL_H_
#define OPENGL_GL_OPENGL_H_

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace effect {

enum TextureType {
  /// 这个类型只有android平台会有,ios可以忽略
  TEXTURE_OES = 0,
  TEXTURE_2D,
};

enum FrameType {
  /// 按内容比例显示, 但是会上下或者左右留黑
  FIT = 0,
  /// 铺满窗口, 但是内容会被左右裁剪
  FILL
};

/**
 * OpenGL 基础处理类
 */
class OpenGL {
 public:
  /// 普通构造函数, shader为 DEFAULT_VERTEX_SHADER DEFAULT_FRAGMENT_SHADER
  OpenGL();
  /// 设置viewport宽高, shader为 DEFAULT_VERTEX_SHADER DEFAULT_FRAGMENT_SHADER
  OpenGL(int width, int height);
  /// 不设置viewport宽高, 自定义shader
  OpenGL(const char* vertex, const char* fragment);
  /// 设置viewport宽高, 自定义shader
  OpenGL(int width, int height, const char* vertex, const char* fragment);
  virtual ~OpenGL();

  /**
   * 设置输入纹理类型, 这个函数应该只有android平台会调用
   * 因为android平台默认是OES类型
   */
  void SetTextureType(TextureType type);

  /**
   * 设置显示方式
   * @param source_width 图像的宽
   * @param source_height 图像的高
   * @param target_width 目标的宽, 这里传进来的应该是窗口或者录制视频的宽
   * @param target_height 目标的高, 这里传进来的应该是窗口或者录制视频的高
   * @param frame_type 显示到窗口或者录制到视频的方式
   */
  void SetFrame(int source_width, int source_height, int target_width, int target_height, FrameType frame_type);

  /**
   * 设置viewport的宽和高
   * @param width glViewPort的宽
   * @param height glViewPort的高
   */
  void SetOutput(int width, int height);

  /**
   * 传到shader中int类型参数和值
   * @param name shader中参数的名字
   * @param value 传入的值
   */
  void SetInt(const char* name, int value);

  /**
   * 设置int数组
   * @param name
   * @param size
   * @param value
   */
  void SetIntArray(const char* name, int size, int* value);

  /**
   * 传到shader中float类型的参数和值
   * @param name shader中参数的名字
   * @param value 传入的值
   */
  void SetFloat(const char* name, float value);

  /**
   * 设置float数组
   * @param name
   * @param size
   * @param value
   */
  void SetFloatArray(const char* name, int size, GLfloat* value);

  /**
   * 传到shader中4个参数
   * @param name shader中参数的名字
   * @param v0 第一个参数
   * @param v1 第二个参数
   * @param v2 第三个参数
   * @param v3 第四个参数
   */
  void SetUniform4f(const char* name, float v0, float v1, float v2, float v3);

  /**
   * 传到shader中2维向量的参数和值
   * @param name 二维向量的名字
   * @param size 二维向量的个数
   * @param value 二维向量的值
   */
  void SetFloatVec2(const char* name, int size, GLfloat* value);

  /**
   * 传到shader中3维向量的参数和值
   * @param name 3维向量的名字
   * @param size 3维向量的个数
   * @param value 3维向量的值
   */
  void SetFloatVec3(const char* name, int size, GLfloat* value);

  /**
   * 传到shader中4维向量的参数和值
   * @param name 4维向量的名字
   * @param size 4维向量的个数
   * @param value 4维向量的值
   */
  void SetFloatVec4(const char* name, int size, GLfloat* value);

  /**
   * 传到shader中3x3矩阵的参数和值
   * @param name 3x3矩阵的名字
   * @param size 3x3矩阵的个数
   * @param matrix 3x3矩阵的值
   */
  void SetUniformMatrix3f(const char* name, int size, GLfloat* matrix);

  /**
   * 传到shader中4x4矩阵的参数和值
   * @param name 4x4矩阵的名字
   * @param size 4x4矩阵的个数
   * @param matrix 4x4矩阵的值
   */
  void SetUniformMatrix4f(const char* name, int size, GLfloat* matrix);

  /**
   * 处理一帧, 渲染OpenGL
   * @param texture_id 待处理的纹理
   */
  void ProcessImage(GLuint texture_id);

  /**
   * 处理一帧, 渲染到OpenGL
   * @param texture_id 待处理的纹理
   * @param matrix 传到顶点中的矩阵
   */
  void ProcessImage(GLuint texture_id, GLfloat* matrix);

  /**
   * 处理一帧, 渲染到OpenGL
   * @param texture_id 待处理的纹理
   * @param vertex_coordinate 顶点坐标
   * @param texture_coordinate 纹理坐标
   */
  void ProcessImage(GLuint texture_id, const GLfloat* vertex_coordinate, const GLfloat* texture_coordinate);

  /**
   * 处理一帧, 渲染到OpenGL
   * @param texture_id 待处理的纹理
   * @param vertex_coordinate 顶点坐标
   * @param texture_coordinate 纹理坐标
   * @param matrix 传到顶点中的矩阵
   */
  void ProcessImage(GLuint texture_id, const GLfloat* vertex_coordinate,
                    const GLfloat* texture_coordinate, GLfloat* matrix);

 protected:
  /**
   * 子类实现额外的OpenGL操作
   */
  virtual void RunOnDrawTasks();
  /**
   * 子类实现在glDrawArrays之后的操作,比如调用glReadPixels等
   */
  virtual void OnDrawArrays();

 private:
  /// 输入的纹理类型
  TextureType type_;
  /// OpenGL编译shader成功返回的program id
  GLuint program_;
  /// glViewPort的宽
  int width_;
  /// glViewPort的高
  int height_;
  /// 顶点坐标
  GLfloat* vertex_coordinate_;
};

}

#endif  // OPENGL_GL_OPENGL_H_