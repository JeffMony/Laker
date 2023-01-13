
#include "gradual_blur_sub_effect.h"
#include "json/json.h"
#include "common.h"
#include "log.h"

namespace effect {

GradualBlurSubEffect::GradualBlurSubEffect() {
  gradual_blur_ = new GradualBlur();
}

GradualBlurSubEffect::~GradualBlurSubEffect() {
  SAFE_DELETE(gradual_blur_)
}

void GradualBlurSubEffect::SetBlurType(BlurType type) {
  gradual_blur_->SetBlurType(type);
}

void GradualBlurSubEffect::SetGradient(bool is_gradual) {
  gradual_blur_->SetGradient(is_gradual);
}

void GradualBlurSubEffect::SetGradualShape(GradualShape shape) {
  gradual_blur_->SetGradualShape(shape);
}

void GradualBlurSubEffect::SetDrawLine(bool draw_line) {
  gradual_blur_->SetDrawLine(draw_line);
}

void GradualBlurSubEffect::SetCircleParams(float center_x, float center_y, float in_radius, float out_radius) {
  gradual_blur_->SetCircleParams(center_x, center_y, in_radius, out_radius);
}

void GradualBlurSubEffect::SetRectParams(float out_start_y, float out_end_y, float in_start_y, float in_end_y) {
  gradual_blur_->SetRectParams(out_start_y, out_end_y, in_start_y, in_end_y);
}

void GradualBlurSubEffect::SetLevel(int level, int gradual_level) {
  gradual_blur_->SetLevel(level, gradual_level);
}

void GradualBlurSubEffect::SetBackgroundImagePath(const std::string &path) {
  int width;
  int height;
  unsigned char *data;
  int channels;
  data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
  if (width == 0 || height == 0 || data == nullptr) {
    LOGE("%s %s %d Load image failed, %s", __FILE_NAME__, __func__ , __LINE__, path.c_str());
    return;
  }
  gradual_blur_->SetBackgroundImage(data, width, height);
  free(data);
}

void GradualBlurSubEffect::SetForegroundImagePath(const std::string &path) {
  int width;
  int height;
  unsigned char *data;
  int channels;
  data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
  if (width == 0 || height == 0 || data == nullptr) {
    LOGE("%s %s %d Load image failed, %s", __FILE_NAME__, __func__ , __LINE__, path.c_str());
    return;
  }
  gradual_blur_->SetForegroundImage(data, width, height);
  free(data);
}

GLuint GradualBlurSubEffect::ProcessImage(const std::list<SubEffect *> &sub_effects,
                                          int origin_texture_id, int texture_id,
                                          int frame_width, int frame_height,
                                          int target_width, int target_height,
                                          int64_t current_time, ImageResult *result) {
  if (effect_param_callback_ != nullptr) {
    Json::Value value;

    /// 回调背景图片的宽高
    if (gradual_blur_->GetWidth() > 0 && gradual_blur_->GetHeight() > 0) {
      value["bg_width"] = gradual_blur_->GetWidth();
      value["bg_height"] = gradual_blur_->GetHeight();
      effect_param_callback_->EffectLocationParam(value.toStyledString());
    }
  }
  return gradual_blur_->ProcessImage(origin_texture_id, target_width, target_height);
}

}  // namespace effect