
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace effect {

class ProcessBuffer {
 public:
  ProcessBuffer();
  ~ProcessBuffer();

  /**
   * 初始化, 创建OpenGL program
   * @param vertex_shader 顶点shader内容
   * @param fragment_shader fragment shader内容
   * @return 成功返回0, 其它为失败
   */
  int Init(const char* vertex_shader, const char* fragment_shader);

  /**
   * 传到shader中int类型参数和值
   * @param name shader中参数的名字
   * @param value 传入的值
   */
  void SetInt(const char* name, int value);

  /**
   * 传到shader中float类型的参数和值
   * @param name shader中参数的名字
   * @param value 传入的值
   */
  void SetFloat(const char* name, float value);

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
  void SetFloatVec1(const char* name, int size, GLfloat* value);

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

  int CreateFrameBuffer(int width, int height);

  /**
   * 传到shader中4x4矩阵的参数和值
   * @param name 4x4矩阵的名字
   * @param size 4x4矩阵的个数
   * @param matrix 4x4矩阵的值
   */
  void SetUniformMatrix4f(const char* name, int size, GLfloat* matrix);

  /**
   * 创建一个FrameBuffer, 同时激活一个program
   * @param width frame buffer宽
   * @param height frame buffer高
   */
  void ActiveProgram(int width = 0, int height = 0);

  /**
   * 设置Viewport的x, y
   */
  void SetViewportPosition(int x, int y);

  /**
   * 设置Viewport的宽和高
   */
  void SetViewportSize(int width, int height);

  /**
   * 清除OpenGL的颜色Buffer信息
   */
  void Clear(bool clear_color_buffer_bit = true);

  /**
   * 设置背景颜色
   */
  void SetClearColor(float red = 0, float green = 0, float blue = 0, float alpha = 0);

  /**
   * VBO时开启position
   */
  void ActiveBufferPosition(GLint size = 2,
                            GLenum type = GL_FLOAT,
                            GLboolean normalized = GL_FALSE,
                            GLsizei stride = 8,
                            GLvoid *pointer = nullptr);

  /**
   * 设置纹理坐标
   */
  void ActiveTextureAttribute(GLint size = 2,
                              GLenum type = GL_FLOAT,
                              GLboolean normalized = GL_FALSE,
                              GLsizei stride = 8,
                              GLvoid *pointer = nullptr);

  void ActiveTextureAttribute(const char *name, GLfloat *texture_coordinate, int texture_coordinate_size = 2);
  /**
   * 开启顶点和纹理坐标的传递
   * @param vertex_coordinate 顶点坐标, 如果为nullptr使用默认顶点坐标
   * @param texture_coordinate 纹理坐标, 如果为nullptr使用默认纹理坐标
   */
  void ActiveAttribute(GLfloat* vertex_coordinate = nullptr,
                       GLfloat* texture_coordinate = nullptr,
                       int vertex_coordinate_size = 2,
                       int texture_coordinate_size = 2);

  /**
 * 开启Vap素材顶点和纹理坐标的传递
 * @param vertex_coordinate 顶点坐标, 如果为nullptr使用默认顶点坐标
 * @param texture_coordinate 纹理坐标, 如果为nullptr使用默认纹理坐标
 */
  void ActiveVapAttribute(GLfloat* vertex_coordinate = nullptr,
                          GLfloat* texture_coordinate = nullptr,
                          GLfloat* rgb_texture_coordinate = nullptr,
                          GLfloat* alpha_texture_coordinate = nullptr,
                          int vertex_coordinate_size = 2,
                          int texture_coordinate_size = 2);

  void ActiveAttributeName(const char* name,
                           GLint size,
                           GLenum type,
                           GLboolean normalized,
                           GLsizei stride,
                           GLvoid *pointer);

  /**
   * 上传纹理
   * @param texture_id 需要显示的纹理
   * @param index glActiveTexture 的位置
   * @param name shader中纹理的名字
   */
  void ActiveTexture(GLuint texture_id, int index, const char* name);

  /**
   * 执行glDrawArray
   */
  void DrawArrays();

  /**
   * 关闭开启的资源
   */
  void Disable();

  /**
   * 销毁资源
   */
  void Destroy();

  /**
   * 绑定当前的FrameBuffer
   */
  void BindFrameBuffer();

  /**
   * 解绑当前的FrameBuffer
   */
  void UnBindFrameBuffer();

  /**
   * 获取当前的Frame Buffer id
   */
  GLuint GetFrameBufferId() {
    return frame_buffer_id_;
  }

  /**
   * 获取与当前FrameBuffer绑定的纹理id
   */
  GLuint GetTextureId() {
    return texture_id_;
  }

 private:
  /// 纹理id
  GLuint texture_id_;
  /// framebuffer id
  GLuint frame_buffer_id_;
  /// OpenGL创建的program
  GLuint program_;
  /// vao id
  GLuint vao_id_;
  /// 顶点坐标
  GLfloat* vertex_coordinate_;
  /// 纹理坐标
  GLfloat* texture_coordinate_;
  /// 背景颜色
  float red_;
  float green_;
  float blue_;
  float alpha_;
  /// 纹理宽
  int width_;
  /// 纹理高
  int height_;
  /// view port的宽和高
  int view_width_;
  int view_height_;
  int x_;
  int y_;
};

}
