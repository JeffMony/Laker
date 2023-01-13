//
// Created by jeffli on 2022/7/28.
//

#include <jni.h>
#include "log.h"
#include "render_process.h"
extern "C" {
#include "media_env.h"
}

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NELEM
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif

#define RENDER_PROCESS "com/jeffmony/videorender/RenderProcessImpl"

static jlong RENDERSDK_render_process_create(JNIEnv *env, jobject object) {
  auto render_process = new media::RenderProcess(object);
  return reinterpret_cast<jlong>(render_process);
}

static void RENDERSDK_render_process_surface_create(JNIEnv *env, jobject object, jlong id, jobject j_surface) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  render_process->OnSurfaceCreate(j_surface);
}

static void RENDERSDK_render_process_surface_changed(JNIEnv *env, jobject object, jlong id, jint width, jint height) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  render_process->OnSurfaceChanged(width, height);
}

static void RENDERSDK_render_process_set_frame_size(JNIEnv *env, jobject object, jlong id, jint width, jint height) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  render_process->SetFrameSize(width, height);
}

static void RENDERSDK_render_process_surface_destroyed(JNIEnv *env, jobject object, jlong id) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  render_process->OnSurfaceDestroyed();
}

static void RENDERSDK_render_process_set_video_size(JNIEnv *env, jobject object, jlong id, jint width, jint height) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  render_process->SetVideoSize(width, height);
}

static void RENDERSDK_render_process_update_frame(JNIEnv *env, jobject object, jlong id) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  render_process->UpdateFrame();
}

static jint RENDERSDK_render_process_add_effect(JNIEnv *env, jobject object, jlong id, jstring j_config) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  auto config = env->GetStringUTFChars(j_config, JNI_FALSE);
  int ret = render_process->AddEffect(config);
  env->ReleaseStringUTFChars(j_config, config);
  return ret;
}

static void RENDERSDK_render_process_update_effect(JNIEnv *env, jobject object, jlong id, jint effect_id, jstring j_config) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  auto config = env->GetStringUTFChars(j_config, JNI_FALSE);
  render_process->UpdateEffect(effect_id, config);
  env->ReleaseStringUTFChars(j_config, config);
}

static void RENDERSDK_render_process_delete_effect(JNIEnv *env, jobject object, jlong id, jint effect_id) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  render_process->DeleteEffect(effect_id);
}

static jint RENDERSDK_render_process_add_filter(JNIEnv *env, jobject object, jlong id, jstring j_config) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  auto config = env->GetStringUTFChars(j_config, JNI_FALSE);
  int ret = render_process->AddFilter(config);
  env->ReleaseStringUTFChars(j_config, config);
  return ret;
}

static void RENDERSDK_render_process_update_filter(JNIEnv *env, jobject object, jlong id, jint filter_id, jstring j_config) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  auto config = env->GetStringUTFChars(j_config, JNI_FALSE);
  render_process->UpdateFilter(filter_id, config);
  env->ReleaseStringUTFChars(j_config, config);
}

static void RENDERSDK_render_process_update_filter_intensity(JNIEnv *env, jobject object, jlong id, jint filter_id, jint intensity) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  render_process->UpdateFilterIntensity(filter_id, intensity);
}

static void RENDERSDK_render_process_delete_filter(JNIEnv *env, jobject object, jlong id, jint filter_id) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  render_process->DeleteFilter(filter_id);
}

static void RENDERSDK_render_process_capture_frame(JNIEnv *env, jobject object, jlong id, jobject j_listener) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  auto listener = env->NewGlobalRef(j_listener);
  render_process->CaptureFrame(listener);
}

static void RENDERSDK_render_process_set_mirror_type(JNIEnv *env, jobject object, jlong id, jint type) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  render_process->SetMirrorType(type);
}

static void RENDERSDK_render_process_destroy(JNIEnv *env, jobject object, jlong id) {
  auto render_process = reinterpret_cast<media::RenderProcess *>(id);
  render_process->Destroy();
  delete render_process;
}

static JNINativeMethod renderProcessMethods[] = {
    {"create", "()J", (void **) RENDERSDK_render_process_create },
    {"surfaceCreate", "(JLandroid/view/Surface;)V", (void **) RENDERSDK_render_process_surface_create },
    {"surfaceChanged", "(JII)V", (void **) RENDERSDK_render_process_surface_changed },
    {"setFrameSize", "(JII)V", (void **) RENDERSDK_render_process_set_frame_size },
    {"surfaceDestroyed", "(J)V", (void **) RENDERSDK_render_process_surface_destroyed },
    {"setVideoSize", "(JII)V", (void **) RENDERSDK_render_process_set_video_size },
    {"updateFrame", "(J)V", (void **) RENDERSDK_render_process_update_frame },
    {"addEffect", "(JLjava/lang/String;)I", (void **) RENDERSDK_render_process_add_effect },
    {"updateEffect", "(JILjava/lang/String;)V", (void **) RENDERSDK_render_process_update_effect },
    {"deleteEffect", "(JI)V", (void **) RENDERSDK_render_process_delete_effect },
    {"addFilter", "(JLjava/lang/String;)I", (void **) RENDERSDK_render_process_add_filter },
    {"updateFilter", "(JILjava/lang/String;)V", (void **) RENDERSDK_render_process_update_filter },
    {"updateFilterIntensity", "(JII)V", (void **) RENDERSDK_render_process_update_filter_intensity },
    {"deleteFilter", "(JI)V", (void **) RENDERSDK_render_process_delete_filter },
    {"captureFrame", "(JLcom/shizhuang/videorender/listener/OnCaptureListener;)V", (void **) RENDERSDK_render_process_capture_frame },
    {"setMirror", "(JI)V", (void **) RENDERSDK_render_process_set_mirror_type },
    {"destroy", "(J)V", (void **) RENDERSDK_render_process_destroy },
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
  JNIEnv *env = nullptr;
  if ((vm)->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
    return JNI_ERR;
  }
  set_java_vm(vm);
  auto render_process = env->FindClass(RENDER_PROCESS);
  jint render_process_result = env->RegisterNatives(render_process, renderProcessMethods, NELEM(renderProcessMethods));
  LOGI("render_process_result=%d", (render_process_result == JNI_OK));
  env->DeleteLocalRef(render_process);
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnLoad(JavaVM *vm, void *reserved) {
  JNIEnv *env = nullptr;
  if ((vm)->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
    return;
  }
}

#ifdef __cplusplus
}
#endif
