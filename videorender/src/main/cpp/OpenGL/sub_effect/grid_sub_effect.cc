//
// Created by jeff lee on 2023/2/19.
//

#include "grid_sub_effect.h"
#include "common.h"
#include "tools.h"
#include "gl.h"

namespace effect {

const auto GRID_FRAGMENT_SHADER = STRINGIZE(
    varying highp vec2 textureCoordinate;
    uniform sampler2D inputImageTexture;
    void main() {
      vec2 uv = textureCoordinate;
      if (uv.x <= 0.5) {
        uv.x = uv.x * 2.0;
      } else {
        uv.x = (uv.x - 0.5) * 2.0;
      }
      if (uv.y <= 0.5) {
        uv.y = uv.y * 2.0;
      } else {
        uv.y = (uv.y - 0.5) * 2.0;
      }
      gl_FragColor = texture2D(inputImageTexture, uv);
    }

);

GridSubEffect::GridSubEffect()
  : grid_buffer_(nullptr) {

}

GridSubEffect::~GridSubEffect() {
  SAFE_DELETE(grid_buffer_)
}

GLuint GridSubEffect::ProcessImage(
    const std::list<SubEffect *> &sub_effects,
    int origin_texture_id,
    int texture_id,
    int frame_width,
    int frame_height,
    int target_width,
    int target_height,
    int64_t current_time,
    ImageResult *result) {
  if (grid_buffer_ == nullptr) {
    grid_buffer_ = new GridBuffer(frame_width, frame_height, DEFAULT_VERTEX_SHADER, (media::shader_header() + GRID_FRAGMENT_SHADER).c_str());
  }
  return grid_buffer_->ProcessImage(origin_texture_id, frame_width, frame_height);
}

}