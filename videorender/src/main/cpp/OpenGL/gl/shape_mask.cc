
#include "shape_mask.h"
#include "OpenGL/gl.h"
#include "common.h"
#include "log.h"
#include "tools.h"

namespace effect {

/// 绘制圆形
const auto ROUND_FRAGMENT_SHADER = STRINGIZE(
    uniform sampler2D inputImageTexture;
varying vec2 textureCoordinate;

uniform float center_x;      /// 圆心x坐标
uniform float center_y;      /// 圆心y坐标
uniform float wh_ratio;      /// 宽高比, 校准圆形半径使用的
uniform float radius;        /// 圆半径

void main() {
  vec4 sourceColor = texture2D(inputImageTexture, textureCoordinate);
  float round_dis = radius * radius;
  float center_rx = center_x * wh_ratio;
  float center_ry = center_y;
  float center_dis = (textureCoordinate.x * wh_ratio - center_rx) * (textureCoordinate.x * wh_ratio - center_rx) +
      (textureCoordinate.y - center_ry) * (textureCoordinate.y - center_ry);
  float alpha = smoothstep(round_dis, round_dis - 0.001, center_dis);
  vec3 resultColor = mix(vec3(0.0, 0.0, 0.0), sourceColor.rgb, alpha);
  gl_FragColor = vec4(resultColor, 1.0);
}
);

/// 绘制五角星
const auto STAR_FRAGMENT_SHADER = STRINGIZE(
    uniform sampler2D inputImageTexture;
varying vec2 textureCoordinate;

uniform float center_x;      /// 中心x坐标
uniform float center_y;      /// 中心y坐标
uniform float wh_ratio;      /// 宽高比, 校准图形比例使用到的
uniform float r_min;
uniform float r_max;

void main() {
  vec4 sourceColor = texture2D(inputImageTexture, textureCoordinate);
  vec2 position = textureCoordinate;
  position.x = position.x * wh_ratio;
  float center_rx = center_x * wh_ratio;
  float center_ry = center_y;
  float n_points = 5.0;
  float starAngle = 2.0 * 3.1415926 / n_points;
  vec3 p0 = r_max * vec3(cos(starAngle / 2.0), sin(starAngle / 2.0), 0.0);
  vec3 p1 = r_min * vec3(cos(0.0), sin(0.0), 0.0);
  vec3 d = p1 - p0;
  position -= vec2(center_rx, center_ry);
  float r = sqrt(position.x * position.x + position.y * position.y);
  float angle = atan(position.y, position.x) + 3.1415926 + 0.31;
  float a = mod(angle, starAngle) / starAngle;
  if (a >= 0.5) {
    a = 1.0 - a;
  }
  vec3 d1 = r * vec3(cos(a * starAngle), sin(a * starAngle), 0) - p0;
  float in_out = smoothstep(0.0, 0.0001, cross(d, d1).z);
  vec3 col = mix(sourceColor.rgb, vec3(0.0, 0.0, 0.0), in_out);
  gl_FragColor = vec4(col, 1.0);
}
);

/// 绘制心形
const auto HEART_FRAGMENT_SHADER = STRINGIZE(
    uniform sampler2D inputImageTexture;
varying vec2 textureCoordinate;
uniform float center_x;
uniform float center_y;
uniform float scale_x;
uniform float scale_y;
uniform float wh_ratio;

float heartLength(vec2 pos) {
pos.y -= 0.35;
pos *= 2.2;
float r = abs(atan(pos.x, pos.y) / 3.1415926);
float r2 = r * r;
float s = (39.0 * r - 66.0 * r2 + 30.0 * r2 * r) / (6.0 - 5.0 * r);
return length(pos) / s;
}

vec4 heartEffect(vec2 pos, vec4 srcColor) {
  float heartLen = heartLength(pos);
  float heart = (1.0 - smoothstep(1.0, 1.001, heartLen));
  return vec4(srcColor.rgb, heart);
}

vec4 mixin(vec4 src1, vec4 src2) {
  return vec4(mix(src1.rgb, src2.rgb, src2.a), max(src1.a, src2.a));
}

void main() {
  vec4 sourceColor = texture2D(inputImageTexture, textureCoordinate);
  vec2 position = textureCoordinate;
  position.x = (center_x - position.x) * wh_ratio * scale_x;
  position.y = (center_y - position.y) * scale_y;
  vec4 bgColor = vec4(0.0, 0.0, 0.0, 1.0);
  gl_FragColor = mixin(bgColor, heartEffect(position, sourceColor));
}
);

/// 绘制矩形
const auto RECTANGLE_FRAGMENT_SHADER = STRINGIZE(
    uniform sampler2D inputImageTexture;
varying vec2 textureCoordinate;

uniform float x;
uniform float y;
uniform float w;
uniform float h;

void main() {
  vec4 sourceColor = texture2D(inputImageTexture, textureCoordinate);
  vec2 position = textureCoordinate;
  if ((x <= position.x && (x + w) >= position.x) && (y <= position.y && (y + h) >= position.y)) {
    gl_FragColor = sourceColor;
  } else {
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  }
}
);

ShapeMask::ShapeMask()
    : shape_type_(ShapeType::NONE_SHAPE)
    , bg_image_texture_id_(0)
    , bg_image_width_(0)
    , bg_image_height_(0)
    , center_x_(0.0f)
    , center_y_(0.0f)
    , radius_(0.0f)
    , r_min_(0.0f)
    , r_max_(0.0f)
    , scale_x_(0.0f)
    , scale_y_(0.0f)
    , rect_w_(0.0f)
    , rect_h_(0.0f)
    , shape_mask_buffer_(nullptr) {
}

ShapeMask::~ShapeMask() {
  if (bg_image_texture_id_ != 0) {
    glDeleteTextures(1, &bg_image_texture_id_);
    bg_image_texture_id_ = 0;
  }
  SAFE_DELETE(shape_mask_buffer_)
}

void ShapeMask::SetShapeType(ShapeType shape_type) {
  shape_type_ = shape_type;
}

void ShapeMask::SetRoundArgument(float center_x, float center_y, float radius) {
  center_x_ = center_x;
  center_y_ = center_y;
  radius_ = radius;
}

void ShapeMask::SetStarArgument(float center_x, float center_y, float r_min, float r_max) {
  center_x_ = center_x;
  center_y_ = center_y;
  r_min_ = r_min;
  r_max_ = r_max;
}

void ShapeMask::SetHeartArgument(float center_x, float center_y, float scale_x, float scale_y) {
  center_x_ = center_x;
  center_y_ = center_y;
  scale_x_ = scale_x;
  scale_y_ = scale_y;
}

void ShapeMask::SetRectArgument(float x, float y, float w, float h) {
  center_x_ = x;
  center_y_ = y;
  rect_w_ = w;
  rect_h_ = h;
}

void ShapeMask::SetBackgroundImage(unsigned char *buffer, int width, int height) {
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

GLuint ShapeMask::ProcessImage(GLuint texture_id, int width, int height) {
  GLuint result_texture_id = texture_id;
  if (result_texture_id == 0) {
    result_texture_id = bg_image_texture_id_;
  }

  if (shape_mask_buffer_ == nullptr) {
    if (shape_type_ == ShapeType::ROUND) {
      shape_mask_buffer_ = new ShapeMaskBuffer(width, height, DEFAULT_VERTEX_SHADER,
                                               (media::shader_header() + ROUND_FRAGMENT_SHADER).c_str());
      shape_mask_buffer_->SetShapeType(shape_type_);
      shape_mask_buffer_->SetRoundArgument(center_x_, center_y_, radius_);
    } else if (shape_type_ == ShapeType::STAR) {
      shape_mask_buffer_ = new ShapeMaskBuffer(width, height, DEFAULT_VERTEX_SHADER,
                                               (media::shader_header() + STAR_FRAGMENT_SHADER).c_str());
      shape_mask_buffer_->SetShapeType(shape_type_);
      shape_mask_buffer_->SetStarArgument(center_x_, center_y_, r_min_, r_max_);
    } else if (shape_type_ == ShapeType::HEART) {
      shape_mask_buffer_ = new ShapeMaskBuffer(width, height, DEFAULT_VERTEX_SHADER,
                                               (media::shader_header() + HEART_FRAGMENT_SHADER).c_str());
      shape_mask_buffer_->SetShapeType(shape_type_);
      shape_mask_buffer_->SetHeartArgument(center_x_, center_y_, scale_x_, scale_y_);
    } else if (shape_type_ == ShapeType::RECTANGLE) {
      shape_mask_buffer_ = new ShapeMaskBuffer(width, height, DEFAULT_VERTEX_SHADER,
                                               (media::shader_header() + RECTANGLE_FRAGMENT_SHADER).c_str());
      shape_mask_buffer_->SetShapeType(shape_type_);
      shape_mask_buffer_->SetRectArgument(center_x_, center_y_, rect_w_, rect_h_);
    }
  }
  if (shape_mask_buffer_ != nullptr) {
    return shape_mask_buffer_->ProcessImage(result_texture_id, width, height);
  }
  return result_texture_id;
}

int ShapeMask::GetWidth() {
  return bg_image_width_;
}

int ShapeMask::GetHeight() {
  return bg_image_height_;
}

ShapeMaskBuffer::ShapeMaskBuffer(int width, int height, const char *vertex_shader, const char *fragment_shader)
    : FrameBuffer(width, height, vertex_shader, fragment_shader)
    , shape_type_(ShapeType::NONE_SHAPE)
    , width_(width)
    , height_(height)
    , center_x_(0.0f)
    , center_y_(0.0f)
    , radius_(0.0f)
    , r_min_(0.0f)
    , r_max_(0.0f)
    , scale_x_(0.0f)
    , scale_y_(0.0f)
    , rect_w_(0.0f)
    , rect_h_(0.0f) {

}

ShapeMaskBuffer::~ShapeMaskBuffer() {}

void ShapeMaskBuffer::SetShapeType(ShapeType shape_type) {
  shape_type_ = shape_type;
}

void ShapeMaskBuffer::SetRoundArgument(float center_x, float center_y, float radius) {
  center_x_ = center_x;
  center_y_ = center_y;
  radius_ = radius;
}

void ShapeMaskBuffer::SetStarArgument(float center_x, float center_y, float r_min, float r_max) {
  center_x_ = center_x;
  center_y_ = center_y;
  r_min_ = r_min;
  r_max_ = r_max;
}

void ShapeMaskBuffer::SetHeartArgument(float center_x, float center_y, float scale_x, float scale_y) {
  center_x_ = center_x;
  center_y_ = center_y;
  scale_x_ = scale_x;
  scale_y_ = scale_y;
}

void ShapeMaskBuffer::SetRectArgument(float x, float y, float w, float h) {
  center_x_ = x;
  center_y_ = y;
  rect_w_ = w;
  rect_h_ = h;
}

void ShapeMaskBuffer::RunOnDrawTasks() {
  if (shape_type_ == ShapeType::ROUND) {
    SetFloat("center_x", center_x_);
    SetFloat("center_y", center_y_);
    SetFloat("radius", radius_);
    SetFloat("wh_ratio", width_ * 1.0f / height_);
  } else if (shape_type_ == ShapeType::STAR) {
    SetFloat("center_x", center_x_);
    SetFloat("center_y", center_y_);
    SetFloat("r_min", r_min_);
    SetFloat("r_max", r_max_);
    SetFloat("wh_ratio", width_ * 1.0f / height_);
  } else if (shape_type_ == ShapeType::HEART) {
    SetFloat("center_x", center_x_);
    SetFloat("center_y", center_y_);
    SetFloat("scale_x", scale_x_);
    SetFloat("scale_y", scale_y_);
    SetFloat("wh_ratio", width_ * 1.0f / height_);
  } else if (shape_type_ == ShapeType::RECTANGLE) {
    SetFloat("x", center_x_);
    SetFloat("y", center_y_);
    SetFloat("w", rect_w_);
    SetFloat("h", rect_h_);
  }
}

}  // namespace effect