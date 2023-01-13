
#include "background_sub_effect.h"
#include "log.h"

namespace effect {

BackgroundSubEffect::BackgroundSubEffect() {
  background_ = new Background();
}

BackgroundSubEffect::~BackgroundSubEffect() {
  SAFE_DELETE(background_)
}

void BackgroundSubEffect::SetOriginalScaleDisplay() {
  background_->SetOriginalScaleDisplay();
}

void BackgroundSubEffect::SetFrameType(int frame_type) {
  background_->SetFrameType(frame_type);
}

void BackgroundSubEffect::SetBlur(int blur) {
  if (blur < 0) {
    LOGE("%s %s %d blur: %d", __FILE_NAME__, __func__ , __LINE__, blur);
    return;
  }
  background_->SetBlur(blur);
}

void BackgroundSubEffect::SetColor(int red, int green, int blue, int alpha) {
  background_->SetColor(red, green, blue, alpha);
}

void BackgroundSubEffect::SetRotate(float rotate) {
  background_->SetRotate(rotate);
}

void BackgroundSubEffect::SetTranslation(float x, float y) {
  background_->SetTranslation(x, y);
}

void BackgroundSubEffect::SetScale(float scale) {
  background_->SetScale(scale);
}

int BackgroundSubEffect::SetImagePath(const std::string &path) {
  int width;
  int height;
  unsigned char *data;
  int channels;
  data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
  if (width == 0 || height == 0 || data == nullptr) {
    LOGE("%s %s %d load image: %s error.", __FILE_NAME__, __func__ , __LINE__, path.c_str());
    return -1;
  }
  image_width_ = width;
  image_height_ = height;
  background_->SetImage(data, width, height);
  free(data);
  return 0;
}

void BackgroundSubEffect::SetTextureWrap(int wrap) {
  background_->SetTextureWrap(wrap);
}

GLuint BackgroundSubEffect::ProcessImage(const std::list<SubEffect *> &sub_effects,
                                         int origin_texture_id, int texture_id,
                                         int frame_width, int frame_height,
                                         int target_width, int target_height,
                                         int64_t current_time, ImageResult* result) {
  return background_->ProcessImage(origin_texture_id, texture_id,
                                   frame_width, frame_height,
                                   target_width, target_height, result);
}

}  // namespace effect