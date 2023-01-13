
#include "effect.h"

#include <utility>
#include <memory>

#include "parse_config.h"
#include "log.h"

namespace effect {

Effect::Effect()
    : effect_param_callback_(nullptr)
    , start_time_(0)
    , end_time_(INT32_MAX) {
}

Effect::~Effect() {
  for (auto& sub_effect : sub_effects_) {
    delete sub_effect;
  }
  sub_effects_.clear();
}

void Effect::SetEffectParamCallback(EffectParamCallback *callback) {
  effect_param_callback_ = callback;
}

void Effect::CallbackEffectParam(int effect_id, EffectParamCallback *callback) {
  if (callback != nullptr) {
    int sub_effect_index = 0;
    for (auto& sub_effect : sub_effects_) {
      auto start_time_ms = sub_effect->start_time;
      auto end_time_ms = sub_effect->end_time;

    }
  }
}

static bool SortSubEffect(SubEffect* s1, SubEffect* s2) {
  return s1->z_order < s2->z_order;
}

int Effect::AddConfig(int effect_id, const char *config, const char *extra_config,
                      EffectInputType type, bool encrypt) {
  int ret = ParseConfig::ParseEffectConfig(effect_id, config, extra_config, type, sub_effects_, encrypt);
  sub_effects_.sort(SortSubEffect);
  for (auto sub_effect : sub_effects_) {
    sub_effect->effect_id = effect_id;
  }
  return ret;
}

int Effect::UpdateConfig(int effect_id, const char *config, EffectInputType type, bool encrypt) {
  return ParseConfig::UpdateEffectConfig(effect_id, config, nullptr, type, sub_effects_, encrypt);
}

void Effect::UpdateTime(int start_time, int end_time) {
  for (auto& sub_effect : sub_effects_) {
    sub_effect->start_time = start_time;
    sub_effect->end_time = end_time;
  }
  start_time_ = start_time;
  end_time_ = end_time;
}

void Effect::UpdateTimeByName(const char *name, int start_time, int end_time) {
  for (auto& sub_effect : sub_effects_) {
    if (strcasecmp(name, sub_effect->name.c_str()) == 0) {
      sub_effect->start_time = start_time;
      sub_effect->end_time = end_time;
    }
  }
}

void Effect::AddSubEffectTimeByName(const char *name, int start_time, int end_time) {
  for (auto& sub_effect : sub_effects_) {
    if (strcasecmp(name, sub_effect->name.c_str()) == 0) {
    }
  }
}

void Effect::UpdateParamFloat(const std::string& effect_name, const std::string& param_name, float value) {
  for (auto& sub_effect : sub_effects_) {
    if (effect_name == sub_effect->name) {
    }
  }
}

void Effect::UpdateParamFloat(const std::string& effect_name, const std::string& param_name, float *value, int length) {
  // NOLINT
}

void Effect::UpdateParamInt(const std::string& effect_name, const std::string& param_name, int value) {
  for (auto& sub_effect : sub_effects_) {
    if (effect_name == sub_effect->name) {
    }
  }
}

GLuint Effect::ProcessImage(GLuint texture_id, int frame_width, int frame_height,
                            int target_width, int target_height, int64_t current_time) {
  return ProcessImage(texture_id, frame_width, frame_height, target_width, target_height, current_time, nullptr);
}

GLuint Effect::ProcessImage(GLuint texture_id, int frame_width, int frame_height,
                            int target_width, int target_height, int64_t current_time, ImageResult* result) {
  auto origin_texture_id = texture_id;
  auto texture = texture_id;
  for (auto& sub_effect : sub_effects_) {
    if (!sub_effect->enable) {
      /// 这个特效关闭了,继续下一个
      continue;
    }

    auto start_time = -1;
    auto end_time = -1;
    start_time = sub_effect->start_time;
    end_time = sub_effect->end_time;

    if (start_time == -1 || end_time == -1) {
      continue;
    }

    if (current_time >= start_time && current_time <= end_time) {
      sub_effect->SetEffectParamCallback(effect_param_callback_);
      texture = sub_effect->ProcessImage(sub_effects_, origin_texture_id,
                                         texture,
                                         frame_width, frame_height, target_width,
                                         target_height, current_time, result);
      if (result != nullptr && result->width != 0 && result->height != 0) {
        frame_width = result->width;
        frame_height = result->height;
      }
    }
  }
  return texture;
}

}  // namespace effect