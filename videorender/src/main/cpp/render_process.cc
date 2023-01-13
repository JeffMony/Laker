//
// Created by jeffli on 2022/7/28.
//

#include "render_process.h"
#include "common.h"
#include "log.h"
#include "gl.h"
#include "OpenGL/gl/process_buffer.h"
#include <android/bitmap.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

extern "C" {
#include "media_env.h"
}

namespace media {

RenderProcess::RenderProcess(jobject object)
  : surface_width_(0)
  , surface_height_(0)
  , video_width_(0)
  , video_height_(0)
  , mirror_type_(MirrorType::NONE_MIRROR)
  , native_window_(nullptr)
  , render_screen_(nullptr)
  , egl_core_(nullptr)
  , render_surface_(EGL_NO_SURFACE)
  , image_process_(nullptr)
  , oes_frame_buffer_(nullptr)
  , oes_texture_(0)
  , texture_matrix_(nullptr)
  , draw_screen_texture_(0)
  , listener_num_(0) {
  JNIEnv *env = nullptr;
  int ret = get_env(&env);
  if (env != nullptr) {
    render_process_object_ = env->NewGlobalRef(object);
  }
  if (ret == JNI_EDETACHED) {
    detach_env();
  }
  texture_matrix_ = new float[16];
  pthread_mutex_init(&listener_mutex_, nullptr);
  std::string name("Render Message Queue");
  handler_thread_ = thread::HandlerThread::Create(name);
  handler_ = new thread::Handler(handler_thread_->GetLooper(), this);
  thread::Message *msg = new thread::Message();
  msg->what = EGLMessage::kCreateEGL;
  handler_->SendMessage(msg);
  image_process_ = new effect::ImageProcess();
}

RenderProcess::~RenderProcess() {
  SAFE_DELETE(handler_thread_)
  SAFE_DELETE(handler_)
  JNIEnv *env = nullptr;
  int ret = get_env(&env);
  if (env != nullptr) {
    if (render_process_object_ != nullptr) {
      env->DeleteGlobalRef(render_process_object_);
      render_process_object_ = nullptr;
    }
  }
  if (ret == JNI_EDETACHED) {
    detach_env();
  }
  SAFE_DELETE_ARRAY(texture_matrix_)
  pthread_mutex_destroy(&listener_mutex_);
}

void RenderProcess::OnSurfaceCreate(jobject j_surface) {
  LOGI("Enter: %s %s %d", __FILE_NAME__, __func__ , __LINE__);
  JNIEnv *env = nullptr;
  int ret = get_env(&env);
  if (env != nullptr) {
    native_window_ = ANativeWindow_fromSurface(env, j_surface);
    if (native_window_ != nullptr) {
      thread::Message *msg = new thread::Message();
      msg->what = EGLMessage::kCreateEGLSurface;
      handler_->SendMessage(msg);
    }
  }
  if (ret == JNI_EDETACHED) {
    detach_env();
  }
  LOGI("Leave: %s %s %d", __FILE_NAME__, __func__ , __LINE__);
}

void RenderProcess::OnSurfaceChanged(int width, int height) {
  if (surface_width_ == 0 || surface_height_ == 0) {
    thread::Message *msg = new thread::Message();
    msg->what = EGLMessage::kSurfaceChanged;
    msg->arg1 = width;
    msg->arg2 = height;
    handler_->SendMessage(msg);
  } else if (surface_width_ != width || surface_height_ != height) {
    thread::Message *msg = new thread::Message();
    msg->what = EGLMessage::kSurfaceChanged;
    msg->arg1 = width;
    msg->arg2 = height;
    handler_->SendMessage(msg);
  }
}

void RenderProcess::SetFrameSize(int width, int height) {
  if (surface_width_ == 0 || surface_width_ == 0) {
    thread::Message *msg = new thread::Message();
    msg->what = EGLMessage::kSetSurfaceSize;
    msg->arg1 = width;
    msg->arg2 = height;
    handler_->SendMessage(msg);
  } else if (surface_width_ != width || surface_height_ != height) {
    thread::Message *msg = new thread::Message();
    msg->what = EGLMessage::kSetSurfaceSize;
    msg->arg1 = width;
    msg->arg2 = height;
    handler_->SendMessage(msg);
  }
}

void RenderProcess::OnSurfaceDestroyed() {
  LOGI("Enter: %s %s %d message size: %d", __FILE_NAME__, __func__ , __LINE__, handler_->Size());
  if (native_window_ != nullptr) {
    ANativeWindow_release(native_window_);
    native_window_ = nullptr;
  }
  thread::Message *msg = new thread::Message();
  msg->what = EGLMessage::kDestroyEGLSurface;
  handler_->SendMessage(msg);
  LOGI("Leave: %s %s %d", __FILE_NAME__, __func__ , __LINE__);
}

void RenderProcess::SetVideoSize(int width, int height) {
  thread::Message *msg = new thread::Message();
  msg->what = PlayerMessage::kSetVideoSize;
  msg->arg1 = width;
  msg->arg2 = height;
  handler_->SendMessage(msg);
}

void RenderProcess::UpdateFrame() {
  handler_->RemoveMessage(PlayerMessage::kRenderFrame);
  thread::Message *msg = new thread::Message();
  msg->what = PlayerMessage::kRenderFrame;
  handler_->SendMessage(msg);
}

int RenderProcess::AddEffect(const char *effect_config) {
  if (render_surface_ == EGL_NO_SURFACE) {
    return -1;
  }
  auto length = strlen(effect_config) + 1;
  auto config = new char[length];
  snprintf(config, length, "%s%c", effect_config, 0);
  auto id = image_process_->GenerateEffectId();
  thread::Message *msg = new thread::Message();
  msg->what = PlayerMessage::kAddEffect;
  msg->arg1 = id;
  msg->obj1 = config;
  handler_->SendMessage(msg);
  return id;
}

void RenderProcess::UpdateEffect(int id, const char *effect_config) {
  if (render_surface_ == EGL_NO_SURFACE) {
    return;
  }
  auto length = strlen(effect_config) + 1;
  auto config = new char[length];
  snprintf(config, length, "%s%c", effect_config, 0);
  thread::Message *msg = new thread::Message();
  msg->what = PlayerMessage::kUpdateEffect;
  msg->arg1 = id;
  msg->obj1 = config;
  handler_->SendMessage(msg);
}

void RenderProcess::DeleteEffect(int id) {
  thread::Message *msg = new thread::Message();
  msg->what = PlayerMessage::kDeleteEffect;
  msg->arg1 = id;
  handler_->SendMessage(msg);
}

int RenderProcess::AddFilter(const char *filter_config) {
  auto length = strlen(filter_config) + 1;
  auto config = new char[length];
  snprintf(config, length, "%s%c", filter_config, 0);
  auto effect_id = image_process_->GenerateEffectId();
  thread::Message *msg = new thread::Message();
  msg->what = PlayerMessage::kAddFilter;
  msg->arg1 = effect_id;
  msg->obj1 = config;
  handler_->SendMessage(msg);
  return effect_id;
}

void RenderProcess::UpdateFilter(int id, const char *filter_config) {
  auto length = strlen(filter_config) + 1;
  auto config = new char[length];
  snprintf(config, length, "%s%c", filter_config, 0);
  thread::Message *msg = new thread::Message();
  msg->what = PlayerMessage::kUpdateFilter;
  msg->arg1 = id;
  msg->obj1 = config;
  handler_->SendMessage(msg);
}

void RenderProcess::UpdateFilterIntensity(int id, int intensity) {
  thread::Message *msg = new thread::Message();
  msg->what = PlayerMessage::kUpdateFilterIntensity;
  msg->arg1 = id;
  msg->arg2 = intensity;
  handler_->SendMessage(msg);
}

void RenderProcess::DeleteFilter(int id) {
  thread::Message *msg = new thread::Message();
  msg->what = PlayerMessage::kDeleteFilter;
  msg->arg1 = id;
  handler_->SendMessage(msg);
}

void RenderProcess::CaptureFrame(jobject listener) {
  int key = ++listener_num_;
  UpdateListenerMap(key, listener);
  thread::Message *msg = new thread::Message();
  msg->what = PlayerMessage::kCaptureFrame;
  msg->arg1 = key;
  handler_->SendMessage(msg);
}

void RenderProcess::SetMirrorType(int type) {
  if (type == 0) {
    mirror_type_ = MirrorType::NONE_MIRROR;
  } else if (type == 1) {
    mirror_type_ = MirrorType::VERTICAL_MIRROR;
  } else if (type == 2) {
    mirror_type_ = MirrorType::HORIZONTAL_MIRROR;
  }
}

void RenderProcess::Destroy() {
  thread::Message *msg = new thread::Message();
  msg->what = EGLMessage::kDestroyEGL;
  handler_->SendMessage(msg);
  handler_thread_->QuitSafely();
}

int RenderProcess::CreateEGLInternal() {
  LOGI("Enter: %s %s %d", __FILE_NAME__, __func__ , __LINE__);
  egl_core_ = new EGLCore();
  int result = egl_core_->Init();
  if (result != 0) {
    egl_core_->Release();
    delete egl_core_;
    egl_core_ = nullptr;
    return result;
  }
  LOGI("Leave: %s %s %d", __FILE_NAME__, __func__ , __LINE__);
  return 0;
}

int RenderProcess::CreateEGLSurfaceInternal() {
  if (egl_core_ == nullptr) {
    LOGE("%s %s %d EGLCore is null.", __FILE_NAME__, __func__ , __LINE__);
    int ret = CreateEGLInternal();
    if (ret != 0) {
      return ret;
    }
  }
  if (native_window_ == nullptr) {
    LOGE("%s %s %d window is null.", __FILE_NAME__, __func__ , __LINE__);
    return Error::NATIVE_WINDOW_NULL;
  }
  LOGI("Enter: %s %s %d", __FILE_NAME__, __func__ , __LINE__);
  render_surface_ = egl_core_->CreateWindowSurface(native_window_);
  if (render_surface_ == EGL_NO_SURFACE) {
    return Error::OPENGL_SURFACE_INVALID;
  }
  int ret = egl_core_->MakeCurrent(render_surface_);
  LOGI("Leave: %s %s %d", __FILE_NAME__, __func__ , __LINE__);
  return ret ? 0 : -1;
}

void RenderProcess::DestroyEGLSurfaceInternal() {
  LOGI("Enter: %s %s %d", __FILE_NAME__, __func__ , __LINE__);
  if (egl_core_ != nullptr) {
    if (render_surface_ != EGL_NO_SURFACE) {
      egl_core_->MakeCurrent(render_surface_);
      egl_core_->ReleaseSurface(render_surface_);
      render_surface_ = EGL_NO_SURFACE;
    }
  }
  if (oes_texture_ > 0) {
    glDeleteTextures(1, &oes_texture_);
    oes_texture_ = 0;
  }
  SAFE_DELETE(oes_frame_buffer_)
  SAFE_DELETE(render_screen_)
  CallJavaOnSurfaceDestroyed();
  LOGI("Leave: %s %s %d", __FILE_NAME__, __func__ , __LINE__);
}

void RenderProcess::DestroyEGLInternal() {
  if (egl_core_ != nullptr) {
    egl_core_->Release();
    SAFE_DELETE(egl_core_)
  }
  SAFE_DELETE(image_process_)
}

void RenderProcess::RenderFrameInternal() {
  if (render_surface_ == EGL_NO_SURFACE || native_window_ == nullptr) {
    return;
  }
  if (video_width_ == 0 || video_height_ == 0 || surface_width_ == 0 || surface_height_ == 0) {
    return;
  }
  if (render_surface_ != EGL_NO_SURFACE) {
    egl_core_->MakeCurrent(render_surface_);
  }
  UpdateTexImage();
  UpdateTextureMatrix();
  if (oes_frame_buffer_ == nullptr) {
    oes_frame_buffer_ = new effect::FrameBuffer(DEFAULT_VERTEX_MATRIX_SHADER, DEFAULT_OES_FRAGMENT_SHADER);
    oes_frame_buffer_->SetTextureType(effect::TextureType::TEXTURE_OES);
  }
  oes_frame_buffer_->SetOutput(video_width_, video_height_);
  float *vertex_coordinates = DEFAULT_VERTEX_COORDINATE;
  float *texture_coordinates = DEFAULT_TEXTURE_COORDINATE;
  if (mirror_type_ == MirrorType::VERTICAL_MIRROR) {
    texture_coordinates = TEXTURE_COORDINATE_VERTICAL_FLIP;
  } else if (mirror_type_ == MirrorType::HORIZONTAL_MIRROR) {
    texture_coordinates = TEXTURE_COORDINATE_HORIZONTAL_FLIP;
  }
  auto process_texture = oes_frame_buffer_->ProcessImage(oes_texture_, video_width_, video_height_,
                                                         vertex_coordinates, texture_coordinates, texture_matrix_);
  process_texture = image_process_->ProcessImage(process_texture, video_width_, video_height_,
                                                 surface_width_, surface_height_, 0);
  draw_screen_texture_ = process_texture;
  if (render_screen_ == nullptr) {
    render_screen_ = new effect::OpenGL(surface_width_, surface_height_);
  }
  render_screen_->SetOutput(surface_width_, surface_height_);
  render_screen_->ProcessImage(process_texture);
  if (!egl_core_->SwapBuffers(render_surface_)) {
    LOGE("%s %s %d eglCore SwapBuffers failed, surface=%d, %d", __FILE_NAME__, __func__ ,
         __LINE__, (render_surface_ == EGL_NO_SURFACE), eglGetError());
  }
}

void RenderProcess::AddEffectInternal(int id, const char *config) {
  int ret = image_process_->OnAddEffect(id, config, nullptr, effect::EffectInputType::kEffectInputBuffer, false);
  if (ret != 0) {
    OnError(ret);
  }
  SAFE_DELETE_ARRAY(config)
}

void RenderProcess::UpdateEffectInternal(int id, const char *config) {
  int ret = image_process_->OnUpdateEffect(id, config, effect::EffectInputType::kEffectInputBuffer);
  if (ret != 0) {
    OnError(ret);
  }
  SAFE_DELETE_ARRAY(config)
}

void RenderProcess::DeleteEffectInternal(int id) {
  image_process_->OnDeleteEffect(id);
}

void RenderProcess::AddFilterInternal(int id, const char *config) {
  int ret = image_process_->OnAddFilter(id, config, 0, INT32_MAX, false);
  if (ret != 0) {
    OnError(ret);
  }
  SAFE_DELETE_ARRAY(config)
}

void RenderProcess::UpdateFilterInternal(int id, const char *config) {
  int ret = image_process_->OnAddFilter(id, config, false);
  if (ret != 0) {
    OnError(ret);
  }
  SAFE_DELETE_ARRAY(config)
}

void RenderProcess::UpdateFilterIntensityInternal(int id, int intensity) {
  image_process_->OnUpdateFilterIntensity(id, intensity);
}

void RenderProcess::DeleteFilterInternal(int id) {
  image_process_->OnDeleteFilter(id);
}

void RenderProcess::CreateOesTexture() {
  if (oes_texture_ == 0) {
    glGenTextures(1, &oes_texture_);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, oes_texture_);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
  OnGenerateOesTexture(oes_texture_);
}

void RenderProcess::CaptureFrameInternal(jobject listener) {
  int width = surface_width_;
  int height = surface_height_;
  auto process_buffer = new effect::ProcessBuffer();
  process_buffer->Init(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
  process_buffer->CreateFrameBuffer(width, height);
  glBindFramebuffer(GL_FRAMEBUFFER, process_buffer->GetFrameBufferId());
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  process_buffer->ActiveProgram();
  process_buffer->ActiveAttribute(DEFAULT_VERTEX_COORDINATE, TEXTURE_COORDINATE_VERTICAL_FLIP);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, draw_screen_texture_);
  process_buffer->DrawArrays();
  GenerateBitmap(width, height, listener);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  SAFE_DELETE(process_buffer)
}

void RenderProcess::UpdateListenerMap(int key, jobject listener) {
  pthread_mutex_lock(&listener_mutex_);
  if (listener_map_.find(key) != listener_map_.end()) {
    listener_map_[key] = listener;
  } else {
    listener_map_.insert(std::pair<int, jobject>(key, listener));
  }
  pthread_mutex_unlock(&listener_mutex_);
}

jobject RenderProcess::GetListener(int key) {
  jobject ret = nullptr;
  pthread_mutex_lock(&listener_mutex_);
  if (listener_map_.find(key) != listener_map_.end()) {
    ret = listener_map_.at(key);
  }
  pthread_mutex_unlock(&listener_mutex_);
  return ret;
}

void RenderProcess::RemoveListener(int key) {
  pthread_mutex_lock(&listener_mutex_);
  if (listener_map_.find(key) != listener_map_.end()) {
    listener_map_.erase(key);
  }
  pthread_mutex_unlock(&listener_mutex_);
}

void RenderProcess::OnGenerateOesTexture(GLuint oes_texture) {
  if (render_process_object_ == nullptr) {
    return;
  }
  JNIEnv *env = nullptr;
  int ret = get_env(&env);
  if (env != nullptr) {
    auto clazz = env->GetObjectClass(render_process_object_);
    jmethodID generate_os_texture_method = env->GetMethodID(clazz, "onGenerateOesTexture", "(I)V");
    if (generate_os_texture_method != nullptr) {
      env->CallVoidMethod(render_process_object_, generate_os_texture_method, oes_texture);
    }
    env->DeleteLocalRef(clazz);
  }
  if (ret == JNI_EDETACHED) {
    detach_env();
  }
}

void RenderProcess::CallJavaOnSurfaceChanged(int width, int height) {
  if (render_process_object_ == nullptr) {
    return;
  }
  JNIEnv *env = nullptr;
  int ret = get_env(&env);
  if (env != nullptr) {
    auto clazz = env->GetObjectClass(render_process_object_);
    jmethodID surface_changed_method = env->GetMethodID(clazz, "onSurfaceChanged", "(II)V");
    if (surface_changed_method != nullptr) {
      env->CallVoidMethod(render_process_object_, surface_changed_method, width, height);
    }
    env->DeleteLocalRef(clazz);
  }
  if (ret == JNI_EDETACHED) {
    detach_env();
  }
}

void RenderProcess::CallJavaOnSurfaceDestroyed() {
  if (render_process_object_ == nullptr) {
    return;
  }
  JNIEnv *env = nullptr;
  int ret = get_env(&env);
  if (env != nullptr) {
    auto clazz = env->GetObjectClass(render_process_object_);
    jmethodID surface_destroyed_method = env->GetMethodID(clazz, "onSurfaceDestroy", "()V");
    if (surface_destroyed_method != nullptr) {
      env->CallVoidMethod(render_process_object_, surface_destroyed_method);
    }
    env->DeleteLocalRef(clazz);
  }
  if (ret == JNI_EDETACHED) {
    detach_env();
  }
}

void RenderProcess::UpdateTexImage() {
  if (render_process_object_ == nullptr) {
    return;
  }
  JNIEnv *env = nullptr;
  int ret = get_env(&env);
  if (env != nullptr) {
    auto clazz = env->GetObjectClass(render_process_object_);
    jmethodID update_tex_method = env->GetMethodID(clazz, "updateTexImage", "()V");
    if (update_tex_method != nullptr) {
      env->CallVoidMethod(render_process_object_, update_tex_method);
    }
    env->DeleteLocalRef(clazz);
  }
  if (ret == JNI_EDETACHED) {
    detach_env();
  }
}

void RenderProcess::UpdateTextureMatrix() {
  if (render_process_object_ == nullptr) {
    return;
  }
  JNIEnv *env = nullptr;
  int ret = get_env(&env);
  if (env != nullptr) {
    auto clazz = env->GetObjectClass(render_process_object_);
    auto matrix_object = env->CallObjectMethod(render_process_object_, env->GetMethodID(clazz, "getTextureMatrix", "()[F"));
    if (matrix_object != nullptr) {
      auto matrix_array = reinterpret_cast<jfloatArray>(matrix_object);
      float *matrix = env->GetFloatArrayElements(matrix_array, JNI_FALSE);
      memcpy(texture_matrix_, matrix, sizeof(float) * 16);
      env->ReleaseFloatArrayElements(matrix_array, matrix, 0);
      env->DeleteLocalRef(matrix_object);
    }
    env->DeleteLocalRef(clazz);
  }
  if (ret == JNI_EDETACHED) {
    detach_env();
  }
}

void RenderProcess::GenerateBitmap(int width, int height, jobject listener) {
  uint8_t *img_buffer = new uint8_t[4 * width * height];
  memset(img_buffer, 0, 4 * width * height);
  glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, img_buffer);
  JNIEnv *env = nullptr;
  int ret = get_env(&env);
  if (env != nullptr) {
    jclass bitmap_class = env->FindClass("android/graphics/Bitmap");
    const char *create_bitmap_method_signature = "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;";
    jmethodID create_bitmap_method_id = env->GetStaticMethodID(bitmap_class, "createBitmap",
                                                               create_bitmap_method_signature);
    jstring config_name = env->NewStringUTF("ARGB_8888");
    jclass bitmap_config_class = env->FindClass("android/graphics/Bitmap$Config");
    const char *bitmap_config_signature = "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;";
    jmethodID value_of_bitmap_config_method_id = env->GetStaticMethodID(bitmap_config_class, "valueOf",
                                                                        bitmap_config_signature);
    jobject bitmap_config = env->CallStaticObjectMethod(bitmap_config_class, value_of_bitmap_config_method_id,
                                                        config_name);
    jobject new_bitmap = env->CallStaticObjectMethod(bitmap_class, create_bitmap_method_id,
                                                     width, height, bitmap_config);

    void *resultBitmapPixels;
    int ret = AndroidBitmap_lockPixels(env, new_bitmap, &resultBitmapPixels);
    if (ret < 0) {
      LOGE("%s %s %d AndroidBitmap_lockPixels() failed !", __FILE_NAME__, __func__ , __LINE__);
      if (listener != nullptr) {
        auto clazz = env->GetObjectClass(listener);
        jmethodID error_method = env->GetMethodID(clazz, "onError", "(I)V");
        if (error_method != nullptr) {
          env->CallVoidMethod(listener, error_method, ret);
        }
        env->DeleteLocalRef(clazz);
        env->DeleteGlobalRef(listener);
      }
      if (ret == JNI_EDETACHED) {
        detach_env();
      }
      return;
    }
    memcpy((uint8_t *) resultBitmapPixels, img_buffer, width * height * 4);
    ret = AndroidBitmap_unlockPixels(env, new_bitmap);
    if (ret < 0) {
      LOGE("%s %s %d AndroidBitmap_unlockPixels() failed !", __FILE_NAME__, __func__ , __LINE__);
      if (listener != nullptr) {
        auto clazz = env->GetObjectClass(listener);
        jmethodID error_method = env->GetMethodID(clazz, "onError", "(I)V");
        if (error_method != nullptr) {
          env->CallVoidMethod(listener, error_method, ret);
        }
        env->DeleteLocalRef(clazz);
        env->DeleteGlobalRef(listener);
      }
      if (ret == JNI_EDETACHED) {
        detach_env();
      }
      SAFE_DELETE_ARRAY(img_buffer)
      return;
    }

    memset(img_buffer, 0, 4 * width * height);
    SAFE_DELETE_ARRAY(img_buffer)

    if (listener != nullptr) {
      auto clazz = env->GetObjectClass(listener);
      jmethodID bitmap_complete_method = env->GetMethodID(clazz, "onSuccess", "(Landroid/graphics/Bitmap;)V");
      if (bitmap_complete_method != nullptr) {
        env->CallVoidMethod(listener, bitmap_complete_method, new_bitmap);
      }
      env->DeleteLocalRef(clazz);
      env->DeleteGlobalRef(listener);
    }
  }
  if (ret == JNI_EDETACHED) {
    detach_env();
  }
}

void RenderProcess::OnError(int code) {
  if (render_process_object_ == nullptr) {
    return;
  }
  JNIEnv *env = nullptr;
  int ret = get_env(&env);
  if (env != nullptr) {
    auto clazz = env->GetObjectClass(render_process_object_);
    jmethodID on_error_method = env->GetMethodID(clazz, "onError", "(I)V");
    if (on_error_method != nullptr) {
      env->CallVoidMethod(render_process_object_, on_error_method, code);
    }
    env->DeleteLocalRef(clazz);
  }
  if (ret == JNI_EDETACHED) {
    detach_env();
  }
}

void RenderProcess::HandleMessage(thread::Message *msg) {
  int what = msg->what;
  switch (what) {
    case EGLMessage::kCreateEGL: {
      int ret = CreateEGLInternal();
      if (ret != 0) {
        OnError(ret);
      }
      break;
    }

    case EGLMessage::kCreateEGLSurface: {
      int ret = CreateEGLSurfaceInternal();
      if (ret != 0) {
        OnError(ret);
      } else {
        CreateOesTexture();
      }
      break;
    }

    case EGLMessage::kSurfaceChanged: {
      surface_width_ = msg->arg1;
      surface_height_ = msg->arg2;
      CallJavaOnSurfaceChanged(surface_width_, surface_height_);
      break;
    }

    case EGLMessage::kSetSurfaceSize: {
      surface_width_ = msg->arg1;
      surface_height_ = msg->arg2;
      CallJavaOnSurfaceChanged(surface_width_, surface_height_);
      break;
    }

    case EGLMessage::kDestroyEGLSurface: {
      DestroyEGLSurfaceInternal();
      break;
    }

    case EGLMessage::kDestroyEGL: {
      DestroyEGLInternal();
      break;
    }

    case PlayerMessage::kSetVideoSize: {
      video_width_ = msg->arg1;
      video_height_ = msg->arg2;
      break;
    }

    case PlayerMessage::kRenderFrame: {
      RenderFrameInternal();
      break;
    }

    case PlayerMessage::kAddEffect: {
      auto id = msg->arg1;
      auto config = reinterpret_cast<const char*>(msg->obj1);
      AddEffectInternal(id, config);
      break;
    }

    case PlayerMessage::kUpdateEffect: {
      auto id = msg->arg1;
      auto config = reinterpret_cast<const char*>(msg->obj1);
      UpdateEffectInternal(id, config);
      break;
    }

    case PlayerMessage::kDeleteEffect: {
      auto id = msg->arg1;
      DeleteEffectInternal(id);
      break;
    }

    case PlayerMessage::kAddFilter: {
      auto id = msg->arg1;
      auto config = reinterpret_cast<const char*>(msg->obj1);
      AddFilterInternal(id, config);
      break;
    }

    case PlayerMessage::kUpdateFilter: {
      auto id = msg->arg1;
      auto config = reinterpret_cast<const char*>(msg->obj1);
      UpdateFilterInternal(id, config);
      break;
    }

    case PlayerMessage::kUpdateFilterIntensity: {
      auto id = msg->arg1;
      auto intensity = msg->arg2;
      UpdateFilterIntensityInternal(id, intensity);
      break;
    }

    case PlayerMessage::kDeleteFilter: {
      auto id = msg->arg1;
      DeleteFilterInternal(id);
      break;
    }

    case PlayerMessage::kCaptureFrame: {
      auto key = msg->arg1;
      auto listener = GetListener(key);
      if (listener != nullptr) {
        RemoveListener(key);
        CaptureFrameInternal(listener);
      }
      break;
    }
  }
}

}