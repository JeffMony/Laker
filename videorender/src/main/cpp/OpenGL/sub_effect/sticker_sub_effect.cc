
#include "sticker_sub_effect.h"
#include "OpenGL/gl.h"
#include "log.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace effect {

StickerSubEffect::StickerSubEffect()
    : width_(0)
    , height_(0)
    , center_x_(0)
    , center_y_(0)
    , scale_(1.f)
    , rotate_(0)
    , image_buffer_(nullptr)
    , image_path_()
    , horizontal_mirror_(false)
    , vertical_mirror_(false) {
  blend_ = BlendFactory::CreateBlend(NORMAL_BLEND);
}

StickerSubEffect::~StickerSubEffect() {
  SAFE_DELETE(blend_)
  SAFE_DELETE(image_buffer_)
}

int StickerSubEffect::SetStickerInfo(const std::string &image) {
  image_path_.clear();
  image_path_.append(image);
  SAFE_DELETE(image_buffer_)
  int image_width;
  int image_height;
  unsigned char *data;
  int channels;
  data = stbi_load(image.c_str(), &image_width, &image_height, &channels, STBI_rgb_alpha);
  if (image_width == 0 || image_height == 0 || data == nullptr) {
    LOGE("%s %s %d load image: %s error.", __FILE_NAME__, __func__ , __LINE__, image.c_str());
    return -1;
  }
  image_buffer_ = new ImageBuffer(image_width, image_height, data);
  free(data);
  width_ = image_width;
  height_ = image_height;
  return 0;
}

std::string StickerSubEffect::GetStickerInfo() {
  return image_path_;
}

void StickerSubEffect::SetCenter(float center_x, float center_y) {
  center_x_ = center_x;
  center_y_ = center_y;
}

void StickerSubEffect::SetScale(float scale) {
  scale_ = scale;
}

void StickerSubEffect::SetRotate(float rotate) {
  rotate_ = rotate;
}

void StickerSubEffect::SetHorizontalMirror(bool mirror) {
  horizontal_mirror_ = mirror;
}

bool StickerSubEffect::GetHorizontalMirror() {
  return horizontal_mirror_;
}

void StickerSubEffect::SetVerticalMirror(bool mirror) {
  vertical_mirror_ = mirror;
}

bool StickerSubEffect::GetVerticalMirror() {
  return vertical_mirror_;
}

GLuint StickerSubEffect::ProcessImage(const std::list<SubEffect *> &sub_effects,
                                      int origin_texture_id,
                                      int texture_id,
                                      int frame_width,
                                      int frame_height,
                                      int target_width,
                                      int target_height,
                                      int64_t current_time,
                                      ImageResult *result) {
  if (image_buffer_ == nullptr) {
    return texture_id;
  }
  float frame_x = 0.f;
  float frame_y = 0.f;
  float image_width;
  float image_height;
  float sticker_width;
  float sticker_height;

  sticker_width = image_width = static_cast<float>(image_buffer_->GetWidth());
  sticker_height = image_height = static_cast<float>(image_buffer_->GetHeight());

  GLfloat texture_coordinate[] = {
      (float) frame_x / image_width, (float) frame_y / image_height,
      (float) (frame_x + sticker_width) / image_width, (float) frame_y / image_height,
      (float) frame_x / image_width, (float) (frame_y + sticker_height) / image_height,
      (float) (frame_x + sticker_width) / image_width, (float) (frame_y + sticker_height) / image_height
  };
  GLfloat vertex_coordinate[] = {
      -1.f, -1.f,
      1.f, -1.f,
      -1.f, 1.f,
      1.f, 1.f
  };
  glm::mat4 matrix = glm::mat4(1);
  float x = center_x_ * target_width - sticker_width / 2;
  float y = center_y_ * target_height - sticker_height / 2;
  float center_x = sticker_width / 2.f;
  float center_y = sticker_height / 2.f;

  glm::mat4 projection = glm::ortho(0.f, static_cast<float>(frame_width), static_cast<float>(frame_height),
                                    0.f, 1.f, 100.f);
  glm::vec3 position = glm::vec3(0.f, 0.f, 10.f);
  glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
  glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
  glm::mat4 view_matrix = glm::lookAt(position, direction, up);
  if (horizontal_mirror_) {
    /// 左右镜像
    vertex_coordinate[0] = sticker_width;
    vertex_coordinate[1] = sticker_height;

    vertex_coordinate[2] = 0;
    vertex_coordinate[3] = sticker_height;

    vertex_coordinate[4] = sticker_width;
    vertex_coordinate[5] = 0;

    vertex_coordinate[6] = 0;
    vertex_coordinate[7] = 0;
  } else if (vertical_mirror_) {
    /// 上下镜像
    vertex_coordinate[0] = 0;
    vertex_coordinate[1] = 0;

    vertex_coordinate[2] = sticker_width;
    vertex_coordinate[3] = 0;

    vertex_coordinate[4] = 0;
    vertex_coordinate[5] = sticker_height;

    vertex_coordinate[6] = sticker_width;
    vertex_coordinate[7] = sticker_height;
  } else {
    /// 正常显示
    vertex_coordinate[0] = 0;
    vertex_coordinate[1] = sticker_height;

    vertex_coordinate[2] = sticker_width;
    vertex_coordinate[3] = sticker_height;

    vertex_coordinate[4] = 0;
    vertex_coordinate[5] = 0;

    vertex_coordinate[6] = sticker_width;
    vertex_coordinate[7] = 0;
  }

  /// 这里传到opengl执行的时候是先旋转在位移
  matrix = glm::translate(matrix, glm::vec3(x, y, 1.f));
  matrix = glm::translate(matrix, glm::vec3(center_x, center_y, 0));
  matrix = glm::rotate(matrix, glm::radians(rotate_), glm::vec3(0.f, 0.f, 1.f));
  matrix = glm::scale(matrix, glm::vec3(scale_, scale_, scale_));
  matrix = glm::translate(matrix, glm::vec3(-center_x, -center_y, 0));
  matrix = projection * view_matrix * matrix;
  return blend_->ProcessImage(texture_id,
                              image_buffer_->GetTextureId(),
                              frame_width, frame_height,
                              target_width, target_height,
                              glm::value_ptr(matrix),
                              vertex_coordinate,
                              texture_coordinate,
                              1.0f);
}

}  // namespace effect