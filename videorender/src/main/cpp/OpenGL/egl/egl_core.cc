
#include "egl_core.h"

#include "log.h"

namespace media {

EGLCore::EGLCore()
    : display_(EGL_NO_DISPLAY)
    , config_(nullptr)
    , context_(EGL_NO_CONTEXT)
    , presentation_time_(nullptr) {}

EGLCore::~EGLCore() = default;

int EGLCore::Init(int egl_version) {
  if (egl_version == 0) {
    egl_version = 2;
  }
  return Init(EGL_NO_CONTEXT, egl_version);
}

int EGLCore::Init(EGLContext shared_context, int egl_version) {
  if (egl_version == 0) {
    egl_version = 2;
  }
  EGLint num_configs;
  const EGLint attributes[] = {
      EGL_BUFFER_SIZE,      32,
      EGL_ALPHA_SIZE,       8,
      EGL_BLUE_SIZE,        8,
      EGL_GREEN_SIZE,       8,
      EGL_RED_SIZE,         8,
      EGL_RENDERABLE_TYPE,
      EGL_OPENGL_ES2_BIT,
      EGL_SURFACE_TYPE,
      EGL_WINDOW_BIT,
      EGL_NONE
  };

  if ((display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
    LOGE("%s %s %d eglGetDisplay() returned error %d", __FILE_NAME__, __func__ , __LINE__, eglGetError());
    return Error::OPENGL_DISPLAY_ERROR;
  }
  if (!eglInitialize(display_, 0, 0)) {
    LOGE("%s %s %d eglInitialize() returned error %d", __FILE_NAME__, __func__ , __LINE__, eglGetError());
    return Error::OPENGL_INITIALIZE_ERROR;
  }

  if (!eglChooseConfig(display_, attributes, &config_, 1, &num_configs)) {
    LOGE("%s %s %d eglChooseConfig() returned error %d", __FILE_NAME__, __func__ , __LINE__, eglGetError());
    Release();
    return Error::OPENGL_CONFIG_ERROR;
  }
  EGLint eglContextAttributes[] = { EGL_CONTEXT_CLIENT_VERSION, static_cast<EGLint>(egl_version), EGL_NONE };
  context_ = eglCreateContext(display_, config_, shared_context, eglContextAttributes);
  if (context_ == nullptr) {
    LOGE("%s %s %d eglCreateContext() returned error %d", __FILE_NAME__, __func__ , __LINE__, eglGetError());
    Release();
    return Error::OPENGL_CREATE_CONTEXT_ERROR;
  }

  presentation_time_ = (PFNEGLPRESENTATIONTIMEANDROIDPROC) eglGetProcAddress("eglPresentationTimeANDROID");
  if (!presentation_time_) {
    LOGE("%s %s %d eglPresentationTimeANDROID is not available!", __FILE_NAME__, __func__ , __LINE__);
    return Error::OPENGL_GET_TIME_ERROR;
  }
  return 0;
}

EGLSurface EGLCore::CreateWindowSurface(ANativeWindow *window) {
  EGLSurface surface = EGL_NO_SURFACE;
  EGLint format;

  if (window == nullptr) {
    LOGE("%s %s %d window is null.", __FILE_NAME__, __func__ , __LINE__);
    return surface;
  }

  if (!eglGetConfigAttrib(display_, config_, EGL_NATIVE_VISUAL_ID, &format)) {
    LOGE("%s %s %d eglGetConfigAttrib() returned error %d", __FILE_NAME__, __func__ , __LINE__, eglGetError());
    Release();
    return surface;
  }
  ANativeWindow_setBuffersGeometry(window, 0, 0, format);
  if (!(surface = eglCreateWindowSurface(display_, config_, window, nullptr))) {
    LOGE("%s %s %d eglCreateWindowSurface() returned error %d", __FILE_NAME__, __func__ , __LINE__, eglGetError());
  }
  return surface;
}

EGLSurface EGLCore::CreateOffscreenSurface(int width, int height) {
  EGLSurface surface = EGL_NO_SURFACE;
  EGLint attributes[] = {EGL_WIDTH, width, EGL_HEIGHT, height, EGL_NONE, EGL_NONE};
  if (!(surface = eglCreatePbufferSurface(display_, config_, attributes))) {
    LOGE("%s %s %d eglCreatePbufferSurface() returned error %d", __FILE_NAME__, __func__ , __LINE__, eglGetError());
  }
  return surface;
}

int EGLCore::MakeCurrent(EGLSurface surface) {
  return eglMakeCurrent(display_, surface, surface, context_);
}

int EGLCore::SwapBuffers(EGLSurface surface) {
  return eglSwapBuffers(display_, surface);
}

int EGLCore::SetPresentationTime(EGLSurface surface, khronos_stime_nanoseconds_t nsecs) {
  presentation_time_(display_, surface, nsecs);
  return 0;
}

void EGLCore::ReleaseSurface(EGLSurface surface) {
  eglDestroySurface(display_, surface);
}

void EGLCore::Release() {
  if (EGL_NO_DISPLAY != display_ && EGL_NO_CONTEXT != context_) {
    eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(display_, context_);
    eglTerminate(display_);
    eglReleaseThread();
  }
  display_ = EGL_NO_DISPLAY;
  context_ = EGL_NO_CONTEXT;
}

EGLContext EGLCore::GetContext() {
  return context_;
}

EGLDisplay EGLCore::GetDisplay() {
  return display_;
}

EGLConfig EGLCore::GetConfig() {
  return config_;
}

}  // namespace media