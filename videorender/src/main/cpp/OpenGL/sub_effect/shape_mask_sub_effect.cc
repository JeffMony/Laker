
#include "shape_mask_sub_effect.h"
#include "json/json.h"
#include "common.h"
#include "log.h"

namespace effect {

ShapeMaskSubEffect::ShapeMaskSubEffect()
    : shape_mask_(nullptr) {
  shape_mask_ = new ShapeMask();
}

ShapeMaskSubEffect::~ShapeMaskSubEffect() {
  SAFE_DELETE(shape_mask_)
}

void ShapeMaskSubEffect::SetShapeType(ShapeType shape_type) {
  shape_mask_->SetShapeType(shape_type);
}

void ShapeMaskSubEffect::SetRoundArgument(float center_x, float center_y, float radius) {
  shape_mask_->SetRoundArgument(center_x, center_y, radius);
}

void ShapeMaskSubEffect::SetStarArgument(float center_x, float center_y, float r_min, float r_max) {
  shape_mask_->SetStarArgument(center_x, center_y, r_min, r_max);
}

void ShapeMaskSubEffect::SetHeartArgument(float center_x, float center_y, float scale_x, float scale_y) {
  shape_mask_->SetHeartArgument(center_x, center_y, scale_x, scale_y);
}

void ShapeMaskSubEffect::SetRectArgument(float x, float y, float w, float h) {
  shape_mask_->SetRectArgument(x, y, w, h);
}

void ShapeMaskSubEffect::SetBackgroundImagePath(const std::string &path) {
  int width;
  int height;
  unsigned char *data;
  int channels;
  data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
  if (width == 0 || height == 0 || data == nullptr) {
    LOGE("%s %s %d Load image failed, %s", __FILE_NAME__, __func__ , __LINE__, path.c_str());
    return;
  }
  shape_mask_->SetBackgroundImage(data, width, height);
  free(data);
}

GLuint ShapeMaskSubEffect::ProcessImage(const std::list<SubEffect *> &sub_effects,
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
    if (shape_mask_->GetWidth() > 0 && shape_mask_->GetHeight() > 0) {
      value["bg_width"] = shape_mask_->GetWidth();
      value["bg_height"] = shape_mask_->GetHeight();
      effect_param_callback_->EffectLocationParam(value.toStyledString());
    }
  }
  return shape_mask_->ProcessImage(origin_texture_id, target_width, target_height);
}

}  // namespace effect