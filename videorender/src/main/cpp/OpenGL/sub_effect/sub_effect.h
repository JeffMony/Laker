
#ifndef OPENGL_SUB_EFFECT_SUB_EFFECT_H_
#define OPENGL_SUB_EFFECT_SUB_EFFECT_H_

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <list>
#include "image_result.h"
#include "effect_param_callback.h"

namespace effect {

class SubEffect {
 public:
  SubEffect();
  ~SubEffect();
  /// 特效类型
  std::string type;
  /// 特效名字
  std::string name;
  /// 特效层级
  int z_order;
  /// 特效开始时间
  int start_time;
  /// 特效结束时间
  int end_time;
  /// 是否打开此特效
  bool enable;
  /// 特效时长
  int duration;
  /// 特效id
  int effect_id;

  /// 特效参数回调
  EffectParamCallback* effect_param_callback_;

  virtual void SetEffectParamCallback(EffectParamCallback* callback) {
    effect_param_callback_ = callback;
  }

  virtual GLuint ProcessImage(const std::list<SubEffect*>& sub_effects,
                              int origin_texture, int texture,
                              int frame_width, int frame_height,
                              int target_width, int target_height,
                              int64_t current_time, ImageResult *result) {
    return texture;
  }
};

}

#endif  // OPENGL_SUB_EFFECT_SUB_EFFECT_H_