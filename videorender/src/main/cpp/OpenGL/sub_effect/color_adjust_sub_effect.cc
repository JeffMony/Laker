
#include "color_adjust_sub_effect.h"
#include "json/json.h"
#include "common.h"
#include "log.h"

namespace effect {

ColorAdjustSubEffect::ColorAdjustSubEffect()
    : color_adjust_(nullptr) {
  color_adjust_ = new ColorAdjust();
}

ColorAdjustSubEffect::~ColorAdjustSubEffect() {
  SAFE_DELETE(color_adjust_)
}

void ColorAdjustSubEffect::SetBrightnessLevel(float brightness_level) {
  color_adjust_->SetBrightnessLevel(brightness_level);
}

void ColorAdjustSubEffect::SetContrastLevel(float contrast_level) {
  color_adjust_->SetContrastLevel(contrast_level);
}

void ColorAdjustSubEffect::SetTemperatureLevel(float temperature_level) {
  color_adjust_->SetTemperatureLevel(temperature_level);
}

void ColorAdjustSubEffect::SetSaturationLevel(float saturation_level) {
  color_adjust_->SetSaturationLevel(saturation_level);
}

void ColorAdjustSubEffect::SetGrainLevel(float grain_level) {
  color_adjust_->SetGrainLevel(grain_level);
}

void ColorAdjustSubEffect::SetSharpnessLevel(float sharpness_level) {
  color_adjust_->SetSharpnessLevel(sharpness_level);
}

void ColorAdjustSubEffect::SetMethodBit(int method_bit) {
  color_adjust_->SetMethodBit(method_bit);
}

void ColorAdjustSubEffect::SetBackgroundImagePath(const std::string &path) {
  int width;
  int height;
  unsigned char *data;
  int channels;
  data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
  if (width == 0 || height == 0 || data == nullptr) {
    LOGE("%s %s %d Load image failed, %s", __FILE_NAME__, __func__ , __LINE__, path.c_str());
    return;
  }
  color_adjust_->SetBackgroundImage(data, width, height);
  free(data);
}

GLuint ColorAdjustSubEffect::ProcessImage(const std::list<SubEffect *> &sub_effects,
                                          int origin_texture_id,
                                          int texture_id,
                                          int frame_width,
                                          int frame_height,
                                          int target_width,
                                          int target_height,
                                          int64_t current_time,
                                          ImageResult *result) {
  if (effect_param_callback_ != nullptr) {
    Json::Value value;
    if (color_adjust_->GetWidth() > 0 && color_adjust_->GetHeight() > 0) {
      value["bg_width"] = color_adjust_->GetWidth();
      value["bg_height"] = color_adjust_->GetHeight();
      effect_param_callback_->EffectLocationParam(value.toStyledString());
    }
  }
  return color_adjust_->ProcessImage(origin_texture_id, target_width, target_height);
}

}  // namespace effect