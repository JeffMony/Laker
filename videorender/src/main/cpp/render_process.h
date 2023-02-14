//
// Created by jeffli on 2022/7/28.
//

#ifndef YUANPLAYER_RENDER_PROCESS_H
#define YUANPLAYER_RENDER_PROCESS_H

#include <jni.h>
#include <map>
#include <pthread.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "OpenGL/egl/egl_core.h"
#include "thread/handler_thread.h"
#include "thread/handler.h"
#include "image_process.h"
#include "render_message.h"

namespace media {

class RenderProcess : public thread::HandlerCallback {
 public:
  RenderProcess(jobject object);

  ~RenderProcess();

  void OnSurfaceCreate(jobject j_surface);

  void OnSurfaceChanged(int width, int height);

  void SetFrameSize(int width, int height);

  void OnSurfaceDestroyed();

  void SetVideoSize(int width, int height);

  void UpdateFrame();

  int AddEffect(const char *config);

  void UpdateEffect(int id, const char *config);

  void DeleteEffect(int id);

  int AddFilter(const char *config);

  void UpdateFilter(int id, const char *config);

  void UpdateFilterIntensity(int id, int intensity);

  void DeleteFilter(int id);

  void CaptureFrame(jobject listener);

  void SetMirrorType(int type);

  void Destroy();

 protected:
  void HandleMessage(thread::Message *msg) override;

 private:
  int CreateEGLInternal();

  int CreateEGLSurfaceInternal();

  void DestroyEGLSurfaceInternal();

  void DestroyEGLInternal();

  void RenderFrameInternal();

  void AddEffectInternal(int id, const char *config);

  void UpdateEffectInternal(int id, const char *config);

  void DeleteEffectInternal(int id);

  void AddFilterInternal(int id, const char *config);

  void UpdateFilterInternal(int id, const char *config);

  void UpdateFilterIntensityInternal(int id, int intensity);

  void DeleteFilterInternal(int id);

  void CreateOesTexture();

  void CaptureFrameInternal(jobject listener);

  void UpdateListenerMap(int key, jobject listener);

  jobject GetListener(int key);

  void RemoveListener(int key);

  /**
   * 回调到java层
   */
  void OnGenerateOesTexture(GLuint oes_texture);

  void CallJavaOnSurfaceChanged(int width, int height);

  void CallJavaOnSurfaceDestroyed();

  void UpdateTexImage();

  void UpdateTextureMatrix();

  void GenerateBitmap(int width, int height, jobject listener);

  void OnError(int code);

  void OnError(JNIEnv *env, int code, jobject listener);

 private:
  jobject render_process_object_;
  int surface_width_;
  int surface_height_;
  int video_width_;
  int video_height_;
  MirrorType mirror_type_;
  /// OpenGL 环境
  EGLCore *egl_core_;
  /// OpenGL 渲染surface
  EGLSurface render_surface_;
  ANativeWindow *native_window_;
  effect::OpenGL *render_screen_;
  thread::HandlerThread *handler_thread_;
  thread::Handler *handler_;
  effect::ImageProcess *image_process_;
  effect::FrameBuffer *oes_frame_buffer_;
  GLuint oes_texture_;
  float *texture_matrix_;
  GLuint draw_screen_texture_;
  std::map<int, jobject> listener_map_;
  int listener_num_;
  pthread_mutex_t listener_mutex_;
};

}

#endif //YUANPLAYER_RENDER_PROCESS_H
