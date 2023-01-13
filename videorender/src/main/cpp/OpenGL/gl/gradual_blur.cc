
#include "gradual_blur.h"
#include "OpenGL/gl.h"
#include "log.h"
#include "common.h"

#define PI 3.14159265

namespace effect {

const auto GRADUAL_BLUR_FRAGMENT_SHADER = STRINGIZE(
    uniform sampler2D inputImageTexture;
varying vec2 textureCoordinate;

const float PI = 3.14159265;

uniform int blur_levels[2];              /// blur_level, gradual_blur_level
uniform float blur_params[2];            /// 深层模糊参数 blurWidth, blurHeight
uniform float gradient_blur_params[2];   /// 过渡模糊参数 blurWidth, blurHeight
uniform float total_weights[2];          /// weight, gradual_weight

uniform int gradient_shape;              /// 定义过渡模糊的形状, 目前有两个: 圆形和矩形, 后期是可以增加的
uniform float wh_ratio;                  /// 宽高比, 校准圆形使用的
uniform float shape_params[4];           /// 形状位置的参数

uniform bool drawLine;                   /// 调试环境下，会加上边界线, 这些线应该上层绘制会好一点

/// 边界值处理
vec2 clampCoordinate(vec2 coordinate) {
return vec2(clamp(coordinate.x, 0.0, 1.0), clamp(coordinate.y, 0.0, 1.0));
}

/// 计算权重
float getSubWeight(int i, int blur_level, float totalWeight) {
  float sigma = float(blur_level) / 3.0; /// NOLINT
  return (1.0 / sqrt(2.0 * PI * sigma * sigma)) * exp(-float(i * i) / (2.0 * sigma * sigma)) / totalWeight; /// NOLINT
}

void transformColor(vec4 sourceColor, int blur_level, float blur_width, float blur_height, float sum_weight) {

  float weight = getSubWeight(0, blur_level, sum_weight);

  vec3 finalColor = sourceColor.rgb * weight;
  vec2 blurOffset = vec2(blur_width, blur_height);

  for (int i = 1; i < blur_level; i++) {
    weight = getSubWeight(i, blur_level, sum_weight);
    finalColor += texture2D(inputImageTexture, clampCoordinate(textureCoordinate - blurOffset * float(i))).rgb * weight; /// NOLINT
    finalColor += texture2D(inputImageTexture, clampCoordinate(textureCoordinate + blurOffset * float(i))).rgb * weight; /// NOLINT
  }

  gl_FragColor = vec4(finalColor, sourceColor.a);
}

/// 渲染矩形
void renderRectGradientBlur(vec4 sourceColor, float out_start_y, float out_end_y, float in_start_y, float in_end_y) {

  if (textureCoordinate.y >= in_start_y && textureCoordinate.y <= in_end_y) {
    gl_FragColor = sourceColor;
    return;
  }

  /// 打开这个开关才能显示线
  if (drawLine) {
    /// 画一条细线区分位置
    if (abs(textureCoordinate.y - in_start_y) < 0.0005 || abs(textureCoordinate.y - out_start_y) < 0.0005) {
      gl_FragColor = vec4(1.0, 1.0, 1.0, 0.0);
      return;
    }

    if (abs(textureCoordinate.y - out_end_y) < 0.0005 || abs(textureCoordinate.y - in_end_y) < 0.0005) {
      gl_FragColor = vec4(1.0, 1.0, 1.0, 0.0);
      return;
    }
  }

  /// 过渡模糊的范围
  if ((textureCoordinate.y < in_start_y && textureCoordinate.y >= out_start_y) ||
      (textureCoordinate.y > in_end_y && textureCoordinate.y <= out_end_y)) {

    int blur_level = blur_levels[1];
    float blur_width = gradient_blur_params[0];
    float blur_height = gradient_blur_params[1];

    float temp_level = 0.0;

    if (textureCoordinate.y < in_start_y && textureCoordinate.y >= out_start_y) {
      temp_level = float(blur_level) * (in_start_y - textureCoordinate.y) / (in_start_y - out_start_y);
    }

    if (textureCoordinate.y > in_end_y && textureCoordinate.y <= out_end_y) {
      temp_level = temp_level * (textureCoordinate.y - in_end_y) / (out_end_y - in_end_y);
    }

    if (abs(temp_level) < 1.0) {
      gl_FragColor = sourceColor;
      return;
    }

    transformColor(sourceColor, int(temp_level), blur_width, blur_height, total_weights[1]);
    return;
  }

  int blur_level = blur_levels[0];

  if (blur_level < 1) {
    gl_FragColor = sourceColor;
    return;
  }
  float blur_width = blur_params[0];
  float blur_height = blur_params[1];

  transformColor(sourceColor, blur_level, blur_width, blur_height, total_weights[0]);
}

/// 渲染圆形
void renderCircleGradientBlur(vec4 sourceColor, float center_x, float center_y, float in_radius, float out_radius) {
  float in_circle = in_radius * in_radius;
  float out_circle = out_radius * out_radius;
  float center_rx = center_x * wh_ratio;
  float center_ry = center_y;

  float center_dis = (textureCoordinate.x * wh_ratio - center_rx) * (textureCoordinate.x * wh_ratio - center_rx) +
      (textureCoordinate.y - center_ry) * (textureCoordinate.y - center_ry);

  if (drawLine) {
    if (abs(center_dis - in_circle) < 0.00025) {
      gl_FragColor = vec4(1.0, 1.0, 1.0, 0.0);
      return;
    }

    if (abs(center_dis - out_circle) < 0.00025) {
      gl_FragColor = vec4(1.0, 1.0, 1.0, 0.0);
      return;
    }
  }

  float out_alpha = smoothstep(out_circle, out_circle - 0.0001, center_dis);
  float in_alpha = smoothstep(in_circle, in_circle - 0.0001, center_dis);
  if (abs(out_alpha) < 0.0001) {
    /// 外圆外的区域做完全模糊
    int blur_level = blur_levels[0];

    if (blur_level < 1) {
      gl_FragColor = sourceColor;
      return;
    }
    float blur_width = blur_params[0];
    float blur_height = blur_params[1];

    transformColor(sourceColor, blur_level, blur_width, blur_height, total_weights[1]);
    return;
  } else {
    if (abs(in_alpha) < 0.0001) {
      int blur_level = blur_levels[1];

      float temp_level = 0.0;
      temp_level = float(blur_level) * (sqrt(center_dis) - in_radius) / (out_radius - in_radius);

      if (abs(temp_level) < 1.0) {
        gl_FragColor = sourceColor;
        return;
      }
      float blur_width = gradient_blur_params[0];
      float blur_height = gradient_blur_params[1];
      transformColor(sourceColor, int(temp_level), blur_width, blur_height, total_weights[0]);
      return;
    }
  }

  gl_FragColor = sourceColor;
}

void main() {
  vec4 sourceColor = texture2D(inputImageTexture, textureCoordinate);

  /// 0 表示没有任何形状
  /// 1 表示 圆形
  /// 2 表示 矩形
  ///   后面是可以扩展的
  if (gradient_shape == 0) {
    transformColor(sourceColor, blur_levels[0], blur_params[0], blur_params[1], total_weights[0]);
  } else if (gradient_shape == 1) {
    renderCircleGradientBlur(sourceColor, shape_params[0], shape_params[1], shape_params[2], shape_params[3]);
  } else if (gradient_shape == 2) {
    renderRectGradientBlur(sourceColor, shape_params[0], shape_params[1], shape_params[2], shape_params[3]);
  }

}
);

GradualBlur::GradualBlur()
    : blur_type_(GAUSSIAN)
    , gradual_shape_(NONE)
    , draw_line_(false)
    , is_gradual_(false)
    , center_x_(0.0f)
    , center_y_(0.0f)
    , in_radius_(0.0f)
    , out_radius_(0.0f)
    , out_start_y_(0.0f)
    , out_end_y_(0.0f)
    , in_start_y_(0.0f)
    , in_end_y_(0.0f)
    , level_(1)
    , gradual_level_(1)
    , bg_image_width_(0)
    , bg_image_height_(0)
    , weight_(0.0f)
    , gradual_weight_(0.0f)
    , bg_image_texture_id_(0)
    , fg_image_texture_id_(0)
    , vertical_blur_(nullptr)
    , horizontal_blur_(nullptr)
    , mix_(nullptr) {

}

GradualBlur::~GradualBlur() {
  if (bg_image_texture_id_ != 0) {
    glDeleteTextures(1, &bg_image_texture_id_);
    bg_image_texture_id_ = 0;
  }

  if (fg_image_texture_id_ != 0) {
    glDeleteTextures(1, &fg_image_texture_id_);
    fg_image_texture_id_ = 0;
  }

  SAFE_DELETE(vertical_blur_)
  SAFE_DELETE(horizontal_blur_)
  SAFE_DELETE(mix_)
}

void GradualBlur::SetBlurType(BlurType type) {
  blur_type_ = type;
}

void GradualBlur::SetGradualShape(GradualShape shape) {
  gradual_shape_ = shape;
}

void GradualBlur::SetDrawLine(bool draw_line) {
  draw_line_ = draw_line;
}

void GradualBlur::SetGradient(bool is_gradual) {
  is_gradual_ = is_gradual;
}

void GradualBlur::SetCircleParams(float center_x, float center_y, float in_radius, float out_radius) {
  center_x_ = center_x;
  center_y_ = center_y;
  in_radius_ = in_radius;
  out_radius_ = out_radius;
}

void GradualBlur::SetRectParams(float out_start_y, float out_end_y, float in_start_y, float in_end_y) {
  out_start_y_ = out_start_y;
  out_end_y_ = out_end_y;
  in_start_y_ = in_start_y;
  in_end_y_ = in_end_y;
}

void GradualBlur::SetLevel(int level, int gradual_level) {
  level_ = level;
  gradual_level_ = gradual_level;
}

void GradualBlur::SetBackgroundImage(unsigned char *buffer, int width, int height) {
  if (bg_image_texture_id_ == 0) {
    glGenTextures(1, &bg_image_texture_id_);
  }
  if (width % 2 != 0 || height % 2 != 0) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  }
  glBindTexture(GL_TEXTURE_2D, bg_image_texture_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);
  bg_image_width_ = width;
  bg_image_height_ = height;
}

void GradualBlur::SetForegroundImage(unsigned char *buffer, int width, int height) {
  if (fg_image_texture_id_ == 0) {
    glGenTextures(1, &fg_image_texture_id_);
  }
  if (width % 2 != 0 || height % 2 != 0) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  }
  glBindTexture(GL_TEXTURE_2D, fg_image_texture_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);
  if (fg_image_texture_id_ != 0) {

    std::string header;
    header.append("#ifdef GL_ES\n")
        .append("precision highp float;\n")
        .append("#else\n")
        .append("#define highp\n")
        .append("#define mediump\n")
        .append("#define lowp\n")
        .append("#endif\n");

    std::string fragment_shader = STRINGIZE(
    varying vec2 textureCoordinate;
    uniform sampler2D inputImageTexture;
    uniform sampler2D inputImageTexture2;
    void main() {
      vec4 color1 = texture2D(inputImageTexture, textureCoordinate);
      vec4 color2 = texture2D(inputImageTexture2, textureCoordinate);
      vec3 color = mix(color1.rgb, color2.rgb, color2.a);
      gl_FragColor = vec4(color,1.0);
    });
    mix_ = new effect::ProcessBuffer();
    mix_->Init(DEFAULT_VERTEX_SHADER, (header + fragment_shader).c_str());
  }
}

float GradualBlur::GetWeight(int level) {
  float ret_weight = 0.0;
  if (level >= 1) {
    float sigma = level * 1.0f / 3.0;
    for (int i = 0; i < level; i++) {
      float weight = (1.0 / sqrt(2.0 * PI * sigma * sigma)) * exp(-float(i * i) / (2.0 * sigma * sigma));
      ret_weight += weight;
      if (i != 0) {
        ret_weight += weight;
      }
    }
  }
  return ret_weight;
}

GLuint GradualBlur::ProcessImage(GLuint texture_id, int width, int height) {
  GLuint bg_image_texture_id;
  if (texture_id != 0) {
    bg_image_texture_id = texture_id;
  } else {
    bg_image_texture_id = bg_image_texture_id_;
  }

  if (width != 0) {
    bg_image_width_ = width;
  }
  if (height != 0) {
    bg_image_height_ = height;
  }

  GLuint result_texture_id;

  if (level_ < 1) {
    result_texture_id = bg_image_texture_id;
  } else {
    /// 对齐传入GLSL中的参数
    std::string header;
    header.append("#ifdef GL_ES\n").append("precision highp float;\n").append("#endif\n");
    vertical_blur_ = new VerticalBlur(bg_image_width_, bg_image_height_, DEFAULT_VERTEX_SHADER, (header + GRADUAL_BLUR_FRAGMENT_SHADER).c_str());
    horizontal_blur_ = new HorizontalBlur(bg_image_width_, bg_image_height_, DEFAULT_VERTEX_SHADER, (header + GRADUAL_BLUR_FRAGMENT_SHADER).c_str());

    weight_ = GetWeight(level_);
    gradual_weight_ = GetWeight(gradual_level_);

    if (!is_gradual_) {
      gradual_shape_ = NONE;
    }

    int blur_offset = 1.0f;
    if (blur_type_ == GAUSSIAN) {
      blur_offset = 1.0f;
    } else if (blur_type_ == FROST_GLASS) {
      blur_offset = 5.0f;
    }

    vertical_blur_->SetBlurParams(level_ * 1.0f, blur_offset, weight_,
                                  gradual_level_ * 1.0f, blur_offset, gradual_weight_);
    vertical_blur_->SetDrawLine(draw_line_);
    vertical_blur_->SetGradualShape(gradual_shape_);
    if (gradual_shape_ == CIRCLE) {
      vertical_blur_->SetShapeParams(center_x_, center_y_, in_radius_, out_radius_);
    } else if (gradual_shape_ == RECT) {
      vertical_blur_->SetShapeParams(out_start_y_, out_end_y_, in_start_y_, in_end_y_);
    }

    horizontal_blur_->SetBlurParams(level_ * 1.0f, 1.0f, weight_,
                                    gradual_level_ * 1.0f, 1.0f, gradual_weight_);

    horizontal_blur_->SetDrawLine(draw_line_);
    horizontal_blur_->SetGradualShape(gradual_shape_);

    if (gradual_shape_ == CIRCLE) {
      horizontal_blur_->SetShapeParams(center_x_, center_y_, in_radius_, out_radius_);
    } else if (gradual_shape_ == RECT) {
      horizontal_blur_->SetShapeParams(out_start_y_, out_end_y_, in_start_y_, in_end_y_);
    }

    auto horizontal_blur_id = horizontal_blur_->ProcessImage(bg_image_texture_id, bg_image_width_, bg_image_height_);
    GLuint blur_texture_id = vertical_blur_->ProcessImage(horizontal_blur_id, bg_image_width_, bg_image_height_);
    result_texture_id = blur_texture_id;
  }

  if (fg_image_texture_id_ != 0 && mix_ != nullptr) {
    mix_->CreateFrameBuffer(width, height);
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, mix_->GetFrameBufferId()))
    GL_CHECK(glClearColor(0.f, 0.f, 0.f, 0.f))
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))
    GL_CHECK(glViewport(0, 0, width, height))

    mix_->ActiveProgram();
    mix_->ActiveAttribute(DEFAULT_VERTEX_COORDINATE, DEFAULT_TEXTURE_COORDINATE);

    GL_CHECK(glActiveTexture(GL_TEXTURE0))
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, result_texture_id))
    mix_->SetInt("inputImageTexture", 0);

    GL_CHECK(glActiveTexture(GL_TEXTURE1))
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, fg_image_texture_id_))
    mix_->SetInt("inputImageTexture2", 1);

    mix_->DrawArrays();

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0))
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0))

    return mix_->GetTextureId();
  } else {
    return result_texture_id;
  }
}

int GradualBlur::GetWidth() {
  return bg_image_width_;
}

int GradualBlur::GetHeight() {
  return bg_image_height_;
}


VerticalBlur::VerticalBlur(int width, int height, const char *vertex_shader, const char *fragment_shader)
    : FrameBuffer(width, height, vertex_shader, fragment_shader)
    , width_(width)
    , height_(height)
    , shape_(NONE)
    , draw_line_(false)
    , blur_level_(0.0f)
    , blur_offset_(1.0f)
    , weight_(0.0f)
    , gradual_blur_level_(0.0f)
    , gradual_blur_offset_(1.0f)
    , gradual_weight_(0.0f)
    , shape1_(0.0f)
    , shape2_(0.0f)
    , shape3_(0.0f)
    , shape4_(0.0f) {

}

VerticalBlur::~VerticalBlur() = default;

void VerticalBlur::SetDrawLine(bool draw_line) {
  draw_line_ = draw_line;
}

void VerticalBlur::SetGradualShape(GradualShape shape) {
  shape_ = shape;
}

void VerticalBlur::SetBlurParams(int blur_level, float blur_offset, float weight, int gradual_blur_level,
                                 float gradual_blur_offset, float gradual_weight) {
  blur_level_ = blur_level;
  blur_offset_ = blur_offset;
  weight_ = weight;
  gradual_blur_level_ = gradual_blur_level;
  gradual_blur_offset_ = gradual_blur_offset;
  gradual_weight_ = gradual_weight;
}

void VerticalBlur::SetShapeParams(float shape1, float shape2, float shape3, float shape4) {
  shape1_ = shape1;
  shape2_ = shape2;
  shape3_ = shape3;
  shape4_ = shape4;
}

void VerticalBlur::RunOnDrawTasks() {
  int blur_levels[] = { blur_level_, gradual_blur_level_ };
  float blur_params[] = { 0, blur_offset_ / height_ };
  float gradient_blur_params[] = { 0, gradual_blur_offset_ / height_ };
  float total_weights[] = { weight_, gradual_weight_ };
  SetIntArray("blur_levels", 2, blur_levels);
  SetFloatArray("total_weights", 2, total_weights);
  SetFloatArray("blur_params", 2, blur_params);
  SetFloatArray("gradient_blur_params", 2, gradient_blur_params);
  SetInt("gradient_shape", int(shape_));
  SetInt("drawLine", draw_line_);
  float ratio = width_ * 1.0f / height_;
  SetFloat("wh_ratio", ratio);
  float shape_params[] = {shape1_, shape2_, shape3_, shape4_};
  SetFloatArray("shape_params", 4, shape_params);
}

HorizontalBlur::HorizontalBlur(int width, int height, const char *vertex_shader, const char *fragment_shader)
    : FrameBuffer(width, height, vertex_shader, fragment_shader)
    , width_(width)
    , height_(height)
    , shape_(NONE)
    , draw_line_(false)
    , blur_level_(0.0f)
    , blur_offset_(1.0f)
    , weight_(0.0f)
    , gradual_blur_level_(0.0f)
    , gradual_blur_offset_(1.0f)
    , gradual_weight_(0.0f)
    , shape1_(0.0f)
    , shape2_(0.0f)
    , shape3_(0.0f)
    , shape4_(0.0f) {

}

HorizontalBlur::~HorizontalBlur() = default;

void HorizontalBlur::SetDrawLine(bool draw_line) {
  draw_line_ = draw_line;
}

void HorizontalBlur::SetGradualShape(GradualShape shape) {
  shape_ = shape;
}

void HorizontalBlur::SetBlurParams(int blur_level, float blur_offset, float weight, int gradual_blur_level,
                                   float gradual_blur_offset, float gradual_weight) {
  blur_level_ = blur_level;
  blur_offset_ = blur_offset;
  weight_ = weight;
  gradual_blur_level_ = gradual_blur_level;
  gradual_blur_offset_ = gradual_blur_offset;
  gradual_weight_ = gradual_weight;
}

void HorizontalBlur::SetShapeParams(float shape1, float shape2, float shape3, float shape4) {
  shape1_ = shape1;
  shape2_ = shape2;
  shape3_ = shape3;
  shape4_ = shape4;
}

void HorizontalBlur::RunOnDrawTasks() {
  int blur_levels[] = { blur_level_, gradual_blur_level_ };
  float blur_params[] = { blur_offset_ / width_, 0 };
  float gradient_blur_params[] = { gradual_blur_offset_ / width_, 0 };
  float total_weights[] = { weight_, gradual_weight_ };
  SetIntArray("blur_levels", 2, blur_levels);
  SetFloatArray("total_weights", 2, total_weights);
  SetFloatArray("blur_params", 2, blur_params);
  SetFloatArray("gradient_blur_params", 2, gradient_blur_params);
  SetInt("gradient_shape", int(shape_));
  SetInt("drawLine", draw_line_);
  float ratio = width_ * 1.0f / height_;
  SetFloat("wh_ratio", ratio);
  float shape_params[] = {shape1_, shape2_, shape3_, shape4_};
  SetFloatArray("shape_params", 4, shape_params);
}

}  // namespace effect