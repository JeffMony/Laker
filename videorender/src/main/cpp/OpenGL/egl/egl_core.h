
#ifndef OPENGL_EGL_EGL_CORE_H_
#define OPENGL_EGL_EGL_CORE_H_

#include <android/native_window.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <KHR/khrplatform.h>
#include "error.h"

namespace media {

/**
 * OpenGL环境控制
 */
class EGLCore {
 public:
  EGLCore();

  virtual ~EGLCore();

  /**
   * 初始化EGL环境
   * @return 0成功, 其它失败
   */
  int Init(int egl_version = 2);

  /**
   * 初始EGL环境
   * @param shared_context 共享context
   * @return 0成功, 其它失败
   */
  int Init(EGLContext shared_context, int egl_version = 2);

  /**
   * 创建显示到屏幕的Surface
   * @param window android的渲染view
   * @return OpenGL的渲染Surface
   */
  EGLSurface CreateWindowSurface(ANativeWindow *window);

  /**
   * 创建离屏渲染的Surface, 在导出时需要
   * @param width Surface的宽
   * @param height Surface的高
   * @return OpenGL的渲染Surface
   */
  EGLSurface CreateOffscreenSurface(int width = 64, int height = 64);

  /**
   * 切换OpenGL上下文
   * @param surface OpenGL的Surface
   * @return 0成功, 其它失败
   */
  int MakeCurrent(EGLSurface surface);

  /**
   * 交换Buffer缓冲区
   * @param surface OpenGL的Surface
   * @return 0成功, 其它失败
   */
  int SwapBuffers(EGLSurface surface);

  /**
   * 设置编码时需要的时间戳
   * @param surface OpenGL的Surface
   * @param nsecs 时间戳, 纳秒
   * @return 0成功,其它失败
   */
  int SetPresentationTime(EGLSurface surface, khronos_stime_nanoseconds_t nsecs);

  /**
   * 释放OpenGL的Surface
   * @param surface OpenGL的Surface
   */
  void ReleaseSurface(EGLSurface surface);

  /**
   * 释放OpenGL的资源
   */
  void Release();

  /**
   * 获取当前的OpenGL上下文
   * @return OpenGL当前的Context
   */
  EGLContext GetContext();

  /**
   * 获取当前的显示设备
   * @return OpenGL当前的显示设备
   */
  EGLDisplay GetDisplay();

  /**
   * 获取创建Context时的Config配置
   * @return config
   */
  EGLConfig GetConfig();

 private:
  /// 当前的显示设备
  EGLDisplay display_;
  /// 创建Context时的Config配置
  EGLConfig config_;
  /// OpenGL上下文环境
  EGLContext context_;
  /// 编码时设置的时间
  PFNEGLPRESENTATIONTIMEANDROIDPROC presentation_time_;
};

}  // namespace media

#endif  // OPENGL_EGL_EGL_CORE_H_