
#include "image_process.h"

#include <utility>
#include <memory>
#include "common.h"
#include "tools.h"
#include "json/json.h"

namespace effect {

ImageProcess::ImageProcess()
    : effect_id_(0)
    , effect_param_callback_(nullptr) {}

ImageProcess::~ImageProcess() {
  ClearEffect();
  ClearFilter();
}

int ImageProcess::GenerateEffectId() {
  return ++effect_id_;
}

GLuint ImageProcess::ProcessImage(GLuint texture_id, int frame_width, int frame_height,
                                  int target_width, int target_height,
                                  int64_t current_time, ImageResult* result) {
  return OnProcess(texture_id, frame_width, frame_height, target_width, target_height, current_time, result);
}

GLuint ImageProcess::OnProcess(GLuint texture_id, int frame_width, int frame_height,
                               int target_width, int target_height,
                               int64_t current_time, ImageResult* result) {
  auto texture = texture_id;
  int source_width = frame_width;
  int source_height = frame_height;
  ImageResult image_result = {
      .width = 0,
      .height = 0
  };

  for (auto& effect : effects_) {
    texture = effect.second->ProcessImage(texture, source_width, source_height,
                                          target_width, target_height, current_time, &image_result);
    if (image_result.width != 0 && image_result.height != 0) {
      source_width = image_result.width;
      source_height = image_result.height;

      if (result != nullptr) {
        result->width = source_width;
        result->height = source_height;
      }
    }
  }
  for (auto& filter : filters_) {
    texture = filter.second->ProcessImage(texture, source_width, source_height, current_time);
  }
  return texture;
}

std::string ImageProcess::GetEffectParam(int effect_id, const char* effect_name) const {
  if (effects_.empty() || effect_id < 0) {
    LOGE("%s %s %d effect is empty or effect_id: %d < 0", __FILE_NAME__, __func__ , __LINE__, effect_id);
    return "";
  }
//    auto result = effects_.find(effect_id);
//    if (result != effects_.end()) {
//        return result->second->GetEffectParam(effect_name);
//    }
  return "";
}

int ImageProcess::OnAddEffect(int effect_id, const char* config_path, const char* extra_config,
                              EffectInputType type, bool encrypt) {
  auto effect = new Effect();
  effect->SetEffectParamCallback(effect_param_callback_);
  auto ret = effect->AddConfig(effect_id, config_path, extra_config, type, encrypt);
  if (ret == 0) {
    effects_.insert(std::pair<int, Effect*>(effect_id, effect));
  } else {
    delete effect;
    return -1;
  }
  effect->CallbackEffectParam(effect_id, effect_param_callback_);
  return ret;
}

int ImageProcess::OnUpdateEffect(int effect_id, const char *config, EffectInputType type) {
  if (effects_.empty() || effect_id < 0) {
    LOGE("%s %s %d effect is empty or effect_id: %d < 0", __FILE_NAME__, __func__ , __LINE__, effect_id);
    return -1;
  }
  auto result = effects_.find(effect_id);
  if (result != effects_.end()) {
    auto ret = result->second->UpdateConfig(effect_id, config, type, false);
    if (ret == 0) {
      result->second->CallbackEffectParam(effect_id, effect_param_callback_);
    }
    return ret;
  }
  return 0;
}

void ImageProcess::OnUpdateEffectTime(int effect_id, int start_time, int end_time) {
  auto result = effects_.find(effect_id);
  if (result != effects_.end()) {
    result->second->UpdateTime(start_time, end_time);
    result->second->CallbackEffectParam(effect_id, effect_param_callback_);
  }
}

void ImageProcess::OnUpdateSubEffectTime(int effect_id, const char *name, int start_time, int end_time) {
  auto result = effects_.find(effect_id);
  if (result != effects_.end()) {
    result->second->UpdateTimeByName(name, start_time, end_time);
    result->second->CallbackEffectParam(effect_id, effect_param_callback_);
  }
}

void ImageProcess::OnAddSubEffectTimeByName(int effect_id, const char *name, int start_time, int end_time) {
  auto result = effects_.find(effect_id);
  if (result != effects_.end()) {
    result->second->AddSubEffectTimeByName(name, start_time, end_time);
    result->second->CallbackEffectParam(effect_id, effect_param_callback_);
  }
}

void ImageProcess::OnUpdateEffectParamFloat(int effect_id, const std::string& effect_name,
                                            const std::string& param_name, float value) {
  auto result = effects_.find(effect_id);
  if (result != effects_.end()) {
    result->second->UpdateParamFloat(effect_name, param_name, value);
  }
}

void ImageProcess::OnUpdateEffectParamInt(int effect_id, const std::string& effect_name,
                                          const std::string& param_name, int value) {
  auto result = effects_.find(effect_id);
  if (result != effects_.end()) {
    result->second->UpdateParamInt(effect_name, param_name, value);
  }
}

void ImageProcess::OnDeleteEffect(int effect_id) {
  auto effect_iterator = effects_.find(effect_id);
  if (effect_iterator != effects_.end()) {
    delete effect_iterator->second;
    effects_.erase(effect_iterator);
  }
}

void ImageProcess::ClearEffect() {
  for (auto& effect : effects_) {
    delete effect.second;
  }
  effects_.clear();
}

int ImageProcess::OnAddFilter(int effect_id, const char *config_path, bool encrypt) {
  return OnAddFilter(effect_id, config_path, -1, -1, encrypt);
}

int ImageProcess::OnAddFilter(int effect_id, const char *config_path, int start_time, int end_time, bool encrypt) {
  std::string config;
  config.append(config_path).append("/config.json");
  std::string config_buffer;
  std::string buffer = media::read_file_to_string(config);
  if (buffer.empty()) {
    LOGE("%s %s %d decode decrypt error: %s", __FILE_NAME__, __func__ , __LINE__, config.c_str());
    return -1;
  }
  config_buffer.append(buffer);
  /// buffer类型, 直接解析
  Json::Value root;
  JSONCPP_STRING err;
  Json::CharReaderBuilder builder;
  const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
  if (!reader->parse(config_buffer.c_str(), config_buffer.c_str() + config_buffer.length(), &root,
                     &err)) {
    /// 解析失败
    LOGE("%s %s %d parse: %s error: %s", __FILE_NAME__, __func__ , __LINE__, config.c_str(), err.c_str());
    return -2;
  }
  auto lut_json = root["lut"];
  if (lut_json.isNull()) {
    LOGE("%s %s %d config: %s lut is null.", __FILE_NAME__, __func__ , __LINE__, config_buffer.c_str());
    return -3;
  }
  auto lut = lut_json.asString();
  int intensity = 100;
  auto intensity_json = root["intensity"];
  if (!intensity_json.isNull()) {
    intensity = intensity_json.asInt();
  }
  std::string lut_path;
  lut_path.append(config_path).append("/").append(lut);
  int lut_width = 0;
  int lut_height = 0;
  unsigned char* lut_buffer;
  int channels;
  lut_buffer = stbi_load(lut_path.c_str(), &lut_width, &lut_height, &channels,
                         STBI_rgb_alpha);
  if (lut_buffer == nullptr) {
    LOGE("%s %s %d decode lut: %s error.", __FILE_NAME__, __func__ , __LINE__, lut_path.c_str());
    return -4;
  }
  int ret = 0;
  if ((lut_width == 64 && lut_height == 64) || (lut_width == 512 && lut_height == 512) || (lut_width == 1024 && lut_height == 1024)) {
    auto result = filters_.find(effect_id);
    if (result == filters_.end()) {
      auto filter = new Filter(lut_buffer, lut_width, lut_height);
      filter->SetStartTime(start_time);
      filter->SetEndTime(end_time);
      filter->SetIntensity(intensity);
      filters_.insert(std::pair<int, Filter*>(effect_id, filter));
    } else {
      auto filter = filters_[effect_id];
      if (start_time != -1 && end_time != -1) {
        filter->SetStartTime(start_time);
        filter->SetEndTime(end_time);
      }
      filter->SetIntensity(intensity);
      filter->UpdateLut(lut_buffer, lut_width, lut_height);
    }
  } else {
    LOGE("%s %s %d lut: %s width: %d height: %d invalid.", __FILE_NAME__, __func__ , __LINE__,
         lut_path.c_str(), lut_width, lut_height);
    ret = -5;
  }
  free(lut_buffer);
  return ret;
}

void ImageProcess::OnUpdateFilterIntensity(int effect_id, int intensity) {
  auto result = filters_.find(effect_id);
  if (result != filters_.end()) {
    filters_[effect_id]->SetIntensity(intensity);
  }
}

void ImageProcess::OnUpdateFilterTime(int effect_id, int start_time, int end_time) {
  auto result = filters_.find(effect_id);
  if (result != filters_.end()) {
    auto filter = filters_[effect_id];
    filter->SetStartTime(start_time);
    filter->SetEndTime(end_time);
  }
}

void ImageProcess::OnDeleteFilter(int effect_id) {
  auto result = filters_.find(effect_id);
  if (result != filters_.end()) {
    auto filter = filters_[effect_id];
    delete filter;
    filters_.erase(effect_id);
  }
}

void ImageProcess::ClearFilter() {
  for (auto& filter : filters_) {
    delete filter.second;
  }
  filters_.clear();
}

void ImageProcess::SetEffectParamCallback(EffectParamCallback *callback) {
  effect_param_callback_ = callback;
}

}  // namespace effect
