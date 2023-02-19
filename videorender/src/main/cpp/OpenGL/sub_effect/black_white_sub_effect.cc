//
// Created by jeff lee on 2023/2/19.
//

#include "black_white_sub_effect.h"
#include "common.h"
#include "tools.h"
#include "gl.h"

namespace effect {

const auto BLACK_WHITE_FRAGMENT_SHADER = STRINGIZE(
    varying highp vec2 textureCoordinate;
    uniform sampler2D inputImageTexture;

    void main() {
      vec4 resultColor = texture2D(inputImageTexture, textureCoordinate);
      /// 对rgb三个通道值取和除以3.0f
      float color = (resultColor.r + resultColor.g + resultColor.b) / 3.0;
      gl_FragColor = vec4(color, color, color, resultColor.a);
    }
);

BlackWhiteSubEffect::BlackWhiteSubEffect()
  : black_white_buffer_(nullptr) {

}

BlackWhiteSubEffect::~BlackWhiteSubEffect() {
  SAFE_DELETE(black_white_buffer_)
}

GLuint BlackWhiteSubEffect::ProcessImage(
    const std::list<SubEffect *> &sub_effects,
    int origin_texture_id,
    int texture_id,
    int frame_width,
    int frame_height,
    int target_width,
    int target_height,
    int64_t current_time,
    ImageResult *result) {
  if (black_white_buffer_ == nullptr) {
    black_white_buffer_ = new BlackWhiteBuffer(frame_width, frame_height, DEFAULT_VERTEX_SHADER, (media::shader_header() + BLACK_WHITE_FRAGMENT_SHADER).c_str());
  }
  return black_white_buffer_->ProcessImage(origin_texture_id, frame_width, frame_height);
}

}