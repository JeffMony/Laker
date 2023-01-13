

/// 高斯模糊
#include "gaussian_blur.h"
#include "OpenGL/gl.h"

namespace effect {

const auto BLUR_SHADER = STRINGIZE(
    uniform sampler2D inputImageTexture;
    varying vec2 textureCoordinate;
    // 模糊半径
    uniform int blurRadius;
    // 模糊步长
    uniform float blurWidth;
    uniform float blurHeight;
    // 总权重
    uniform float sumWeight;
    // PI
    const float PI = 3.1415926;

    // 边界值处理
    vec2 clampCoordinate(vec2 coordinate) {
      return vec2(clamp(coordinate.x, 0.0, 1.0), clamp(coordinate.y, 0.0, 1.0));
    }

    // 计算权重
    float getWeight(int i) {
      float sigma = float(blurRadius) / 3.0; /// NOLINT
      return (1.0 / sqrt(2.0 * PI * sigma * sigma)) * exp(-float(i * i) / (2.0 * sigma * sigma)) / sumWeight; /// NOLINT
    }

    void main() {
      vec4 sourceColor = texture2D(inputImageTexture, textureCoordinate);

      if (blurRadius <= 1) {
        gl_FragColor = sourceColor;
        return;
      }

      float weight = getWeight(0);

      vec3 finalColor = sourceColor.rgb * weight;

      vec2 blurOffset = vec2(blurWidth, blurHeight);
      for (int i = 1; i < blurRadius; i++) {
        weight = getWeight(i);
        finalColor += texture2D(inputImageTexture, clampCoordinate(textureCoordinate - blurOffset * float(i))).rgb * weight; /// NOLINT
        finalColor += texture2D(inputImageTexture, clampCoordinate(textureCoordinate + blurOffset * float(i))).rgb * weight; /// NOLINT
      }

      gl_FragColor = vec4(finalColor, sourceColor.a);
    }
);

GaussianBlur::GaussianBlur(int width, int height)
    : blur_offset_(1.f), blur_radius_(5) {
  width_ = width;
  height_ = height;
  std::string header;
  header.append("#ifdef GL_ES\n")
      .append("precision highp float;\n")
      .append("#endif\n");
  vertical_blur_ = new VerticalGaussianBlur(
      width_, height_, DEFAULT_VERTEX_SHADER, (header + BLUR_SHADER).c_str());
  horizontal_blur_ = new HorizontalGaussianBlur(
      width_, height_, DEFAULT_VERTEX_SHADER, (header + BLUR_SHADER).c_str());
}

GaussianBlur::~GaussianBlur() {
  SAFE_DELETE(vertical_blur_)
  SAFE_DELETE(horizontal_blur_)
}

void GaussianBlur::SetBlurOffset(float offset) { blur_offset_ = offset; }

void GaussianBlur::SetBlurRadius(int radius) { blur_radius_ = radius; }

int GaussianBlur::ProcessImage(int texture_id) {
  float sum_weight = 0.f;
  if (blur_radius_ >= 1) {
    auto sigma = blur_radius_ / 3.f;
    for (int i = 0; i < blur_radius_; i++) {
      auto weight = (1.f / sqrt(2.f * M_PI * sigma * sigma)) *
          exp(-(i * i) / (2 * sigma * sigma));
      sum_weight += weight;
      if (i != 0) {
        sum_weight += weight;
      }
    }
  }
  // radius = 30 offset = 1 高斯模糊
  // radius = 30 offset = 5 毛玻璃
  vertical_blur_->SetBlurParam(blur_radius_, blur_offset_, sum_weight);
  horizontal_blur_->SetBlurParam(blur_radius_, blur_offset_, sum_weight);
  auto horizontal_blur_id =
      horizontal_blur_->ProcessImage(texture_id, width_, height_);
  return vertical_blur_->ProcessImage(horizontal_blur_id, width_, height_);
}

VerticalGaussianBlur::VerticalGaussianBlur(int width, int height,
                                           const char *vertex_shader,
                                           const char *fragment_shader)
    : FrameBuffer(width, height, vertex_shader, fragment_shader) {
  width_ = width;
  height_ = height;
  blur_radius_ = 30;
  blur_offset_ = 1.f;
  sum_weight_ = 0.f;
}

VerticalGaussianBlur::~VerticalGaussianBlur() = default;

void VerticalGaussianBlur::SetBlurParam(int radius, float offset,
                                        float weight) {
  blur_radius_ = radius;
  blur_offset_ = offset;
  sum_weight_ = weight;
}

void VerticalGaussianBlur::RunOnDrawTasks() {
  SetInt("blurRadius", blur_radius_);
  SetFloat("blurWidth", 0);
  SetFloat("blurHeight", blur_offset_ / height_);
  SetFloat("sumWeight", sum_weight_);
}

HorizontalGaussianBlur::HorizontalGaussianBlur(int width, int height,
                                               const char *vertex_shader,
                                               const char *fragment_shader)
    : FrameBuffer(width, height, vertex_shader, fragment_shader) {
  width_ = width;
  height_ = height;
  blur_radius_ = 30;
  blur_offset_ = 1.f;
  sum_weight_ = 0.f;
}

HorizontalGaussianBlur::~HorizontalGaussianBlur() = default;

void HorizontalGaussianBlur::SetBlurParam(int radius, float offset,
                                          float weight) {
  blur_radius_ = radius;
  blur_offset_ = offset;
  sum_weight_ = weight;
}

void HorizontalGaussianBlur::RunOnDrawTasks() {
  SetInt("blurRadius", blur_radius_);
  SetFloat("blurWidth", blur_offset_ / width_);
  SetFloat("blurHeight", 0);
  SetFloat("sumWeight", sum_weight_);
}

}  // namespace effect