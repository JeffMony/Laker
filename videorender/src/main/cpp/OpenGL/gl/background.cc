
#include "background.h"
#include <string>
#include "OpenGL/gl.h"
#include "log.h"
#include "tools.h"
#include "common.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "program.h"

/// 高斯模糊时纹理的最大值
#define BLUR_WIDTH_MAX_SIZE 128 * 4

namespace effect {

Background::Background()
    : program_(0)
    , second_program_(0)
    , frame_type_(FIT)
    , frame_buffer_id_(0)
    , texture_id_(0)
    , source_width_(0)
    , source_height_(0)
    , frame_width_(0)
    , frame_height_(0)
    , target_width_(0)
    , target_height_(0)
    , red_(0)
    , green_(0)
    , blue_(0)
    , alpha_(1)
    , blur_(0)
    , rotate_(0.f)
    , translation_x_(0.f)
    , translation_y_(0.f)
    , scale_(1.f)
    , gaussian_blur_(nullptr)
    , scale_frame_buffer_(nullptr)
    , background_image_texture_(0)
    , image_width_(0)
    , image_height_(0)
    , background_type_(BackgroundType::kNone)
    , texture_wrap_(GL_CLAMP_TO_EDGE) {
  std::string vertex_shader = STRINGIZE(
      attribute vec3 position;
      attribute vec2 inputTextureCoordinate;
      varying vec2 textureCoordinate;
      uniform mat4 textureMatrix;
      void main() {
        vec4 p = textureMatrix * vec4(position, 1.);
        textureCoordinate = inputTextureCoordinate;
        gl_Position = p;
      });

  std::string header;
  header.append("#ifdef GL_ES\n").append("precision highp float;\n").append("#endif\n");
  std::string background_fragment = STRINGIZE(
      varying vec2 textureCoordinate;
      uniform vec4 color;
      uniform sampler2D inputImageTexture;
      uniform mat4 inverseModel;
      void main() {
        vec2 uv = (vec4((textureCoordinate.x), textureCoordinate.y, 0.0, 1.0)).xy;
        vec4 texture = texture2D(inputImageTexture, uv);
        vec4 outColor = texture + color;
        gl_FragColor = outColor;
      });

  program_ = Program::CreateProgram(vertex_shader.c_str(), (header + background_fragment).c_str());
  second_program_ = Program::CreateProgram(vertex_shader.c_str(), DEFAULT_FRAGMENT_SHADER);
  vertex_coordinate_ = new GLfloat[8];
  memcpy(vertex_coordinate_, DEFAULT_VERTEX_COORDINATE, sizeof(GLfloat) * 8);
  background_vertex_coordinate_ = new GLfloat[8];
  memcpy(background_vertex_coordinate_, DEFAULT_VERTEX_COORDINATE, sizeof(GLfloat) * 8);
}

Background::~Background() {
  DeleteFrameBuffer();
  DeleteBackgroundImageTexture();
  if (program_ != 0) {
    glDeleteProgram(program_);
  }
  program_ = 0;
  if (second_program_ != 0) {
    glDeleteProgram(second_program_);
  }
  second_program_ = 0;
  SAFE_DELETE(gaussian_blur_)
  SAFE_DELETE(scale_frame_buffer_)
  SAFE_DELETE_ARRAY(vertex_coordinate_)
  SAFE_DELETE_ARRAY(background_vertex_coordinate_)
}

void Background::SetOriginalScaleDisplay() {
  background_type_ = BackgroundType::kBackgroundOriginalScaleDisplay;
  ResetBackgroundColor();
  SAFE_DELETE(gaussian_blur_);
  SAFE_DELETE(scale_frame_buffer_)
  DeleteBackgroundImageTexture();
}

void Background::SetFrameType(int frame_type) {
  frame_type_ = frame_type;
}

void Background::SetColor(int red, int green, int blue, int alpha) {
  red_ = red;
  green_ = green;
  blue_ = blue;
  alpha_ = alpha;
  background_type_ = BackgroundType::kBackgroundColor;
  SAFE_DELETE(gaussian_blur_);
  SAFE_DELETE(scale_frame_buffer_)
  DeleteBackgroundImageTexture();
}

void Background::SetImage(unsigned char* buffer, int width, int height) {
  if (background_image_texture_ == 0) {
    glGenTextures(1, &background_image_texture_);
  }
  if (width % 2 != 0 || height % 2 != 0) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  }
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, background_image_texture_);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glSafeTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
  glBindTexture(GL_TEXTURE_2D, 0);
  image_width_ = width;
  image_height_ = height;
  background_type_ = BackgroundType::kBackgroundImage;
  SAFE_DELETE(gaussian_blur_)
  SAFE_DELETE(scale_frame_buffer_)
  ResetBackgroundColor();
}

void Background::SetBlur(int blur) {
  blur_ = blur;
  background_type_ = BackgroundType::kBackgroundBlur;
  DeleteBackgroundImageTexture();
  ResetBackgroundColor();
}

void Background::SetRotate(float rotate) {
  rotate_ = rotate;
}

void Background::SetTranslation(float x, float y) {
  translation_x_ = x;
  translation_y_ = y;
}

void Background::SetScale(float scale) {
  scale_ = scale;
}

void Background::SetTextureWrap(int wrap) {
  if (wrap == TextureWrapType::kRepeat) {
    texture_wrap_ = GL_REPEAT;
  } else {
    texture_wrap_ = GL_CLAMP_TO_EDGE;
  }
}

GLuint Background::ProcessImage(GLuint origin_texture_id, GLuint texture_id, int frame_width,
                                int frame_height, int target_width, int target_height, ImageResult* result) {
  if (program_ == 0) {
    LOGE("%s %s %d program error: %d", __FILE_NAME__, __func__ , __LINE__, program_);
    return texture_id;
  }
  if (target_width_ != target_width || target_height_ != target_height ||
      frame_width != frame_width_ || frame_height != frame_height_) {
    target_width_ = target_width;
    target_height_ = target_height;
    frame_width_ = frame_width;
    frame_height_ = frame_height;
    CropVertexCoordinate(frame_width, frame_height, target_width, target_height,
                         frame_type_, vertex_coordinate_);
    CropVertexCoordinate(frame_width, frame_height, target_width, target_height,
                         FILL, background_vertex_coordinate_);
    CreateFrameBuffer(target_width, target_height);
  }

  glm::mat4 projection = glm::ortho(0.f, static_cast<float>(target_width),
                                    static_cast<float>(target_height), 0.f, 1.f, 100.f);
  glm::vec3 position = glm::vec3(0.f, 0.f, 10.f);
  glm::vec3 direction = glm::vec3(0.f, 0.f, 0.f);
  glm::vec3 up = glm::vec3(0.f, 1.f, 0);
  glm::mat4 view_matrix = glm::lookAt(position, direction, up);

  auto background_texture_id = GetBackgroundTextureId(texture_id, frame_width, frame_height);
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id_))
  GL_CHECK(glViewport(0, 0, target_width, target_height))
  GL_CHECK(glClearColor(0.f, 0.f, 0.f, 1.f))
  GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  GL_CHECK(glUseProgram(program_))
  /// 画背景
  /// 画背景时,只处理旋转, 不处理缩放和位移
  if (background_type_ == BackgroundType::kBackgroundColor ||
      background_type_ == BackgroundType::kBackgroundOriginalScaleDisplay) {
    GL_CHECK(glActiveTexture(GL_TEXTURE0))
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0))
    auto color_location = glGetUniformLocation(program_, "color");
    GL_CHECK(glUniform4f(color_location, static_cast<float>(red_) / 255.f,
                         static_cast<float>(green_) / 255.f,
                         static_cast<float>(blue_) / 255.f,
                         static_cast<float>(alpha_) / 255.f));
  } else {
    GL_CHECK(glActiveTexture(GL_TEXTURE0))
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, background_texture_id));
    auto color_location = glGetUniformLocation(program_, "color");
    GL_CHECK(glUniform4f(color_location, 0.f, 0.f, 0.f, 0.f))
    auto input_image_texture_location = glGetUniformLocation(program_, "inputImageTexture");
    GL_CHECK(glUniform1i(input_image_texture_location, 0))
  }
  auto position_location = glGetAttribLocation(program_, "position");
  GL_CHECK(glEnableVertexAttribArray(position_location))
  GL_CHECK(glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE,
                                 2 * sizeof(GLfloat), background_vertex_coordinate_));
  auto input_texture_coordinate_location = glGetAttribLocation(program_, "inputTextureCoordinate");
  GL_CHECK(glEnableVertexAttribArray(input_texture_coordinate_location));
  GL_CHECK(glVertexAttribPointer(input_texture_coordinate_location, 2, GL_FLOAT, GL_FALSE,
                                 2 * sizeof(GLfloat), DEFAULT_TEXTURE_COORDINATE))
  auto background_matrix = glm::mat4(1);
  if (rotate_ > 0) {
    background_matrix = glm::translate(background_matrix,
                                       glm::vec3(target_width / 2, target_height / 2, 0.f));
    background_matrix = glm::rotate(background_matrix, glm::radians(rotate_),
                                    glm::vec3(0.f, 0.f, 1.f));
    background_matrix = glm::scale(background_matrix, glm::vec3(1.5f, 1.5f, 1.f));
    background_matrix = glm::translate(background_matrix,
                                       glm::vec3(-target_width / 2, -target_height / 2, 0.f));
  }
  auto inverse_model_location = glGetUniformLocation(program_, "inverseModel");
  GL_CHECK(glUniformMatrix4fv(inverse_model_location, 1, GL_FALSE, glm::value_ptr(background_matrix)))

  background_matrix = projection * view_matrix * background_matrix;
  auto background_matrix_location = glGetUniformLocation(program_, "textureMatrix");
  GL_CHECK(glUniformMatrix4fv(background_matrix_location, 1, GL_FALSE, glm::value_ptr(background_matrix)))

  GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4))
  GL_CHECK(glDisableVertexAttribArray(position_location))
  GL_CHECK(glDisableVertexAttribArray(input_texture_coordinate_location))

  auto matrix = glm::mat4(1);
  matrix = glm::translate(matrix, glm::vec3(translation_x_, translation_y_, 1.f));
  matrix = glm::translate(matrix, glm::vec3(frame_width / 2, frame_height / 2, 0.f));
  matrix = glm::rotate(matrix, glm::radians(rotate_), glm::vec3(0.f, 0.f, 1.f));
  matrix = glm::scale(matrix, glm::vec3(scale_, scale_, 1.f));
  matrix = glm::translate(matrix, glm::vec3(-frame_width / 2, -frame_height / 2, 0.f));
  matrix = projection * view_matrix * matrix;
  // 画视频内容
  GL_CHECK(glUseProgram(second_program_))
  GL_CHECK(glActiveTexture(GL_TEXTURE1))
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture_id))
  auto blend_input_image_texture_location = glGetUniformLocation(second_program_, "inputImageTexture");
  GL_CHECK(glUniform1i(blend_input_image_texture_location, 1));
  auto position2_location = glGetAttribLocation(second_program_, "position");
  GL_CHECK(glEnableVertexAttribArray(position2_location));
  GL_CHECK(glVertexAttribPointer(position2_location, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertex_coordinate_));
  auto input_texture_coordinate2_location = glGetAttribLocation(second_program_, "inputTextureCoordinate");
  GL_CHECK(glEnableVertexAttribArray(input_texture_coordinate2_location));
  GL_CHECK(glVertexAttribPointer(input_texture_coordinate2_location, 2, GL_FLOAT, GL_FALSE,
                                 2 * sizeof(GLfloat), DEFAULT_TEXTURE_COORDINATE));
  auto matrix_location = glGetUniformLocation(second_program_, "textureMatrix");
  GL_CHECK(glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(matrix)))
  GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4))
  GL_CHECK(glDisableVertexAttribArray(position2_location))
  GL_CHECK(glDisableVertexAttribArray(input_texture_coordinate2_location))
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0))
  if (result != nullptr) {
    result->width = target_width_;
    result->height = target_height_;
  }
  return texture_id_;
}

void Background::ProcessBackground(GLuint texture_id, int frame_width, int frame_height) {
  // NOLINT
}

void Background::CropVertexCoordinate(int source_width, int source_height,
                                      int target_width, int target_height,
                                      int frame_type, float *vertex_coordinate) {
  float target_ratio = target_width * 1.f / target_height;
  float scale_width = 1.f;
  float scale_height = 1.f;
  if (frame_type == 0) {
    /// 上下或者左右留黑
    float source_ratio = source_width * 1.f / source_height;
    if (source_ratio > target_ratio) {
      scale_width = 1.f;
      scale_height = target_ratio / source_ratio;
    } else {
      scale_width = source_ratio / target_ratio;
      scale_height = 1.f;
    }
  } else if (frame_type == 1) {
    /// 铺满target但是内容会被裁剪掉
    float source_ratio = source_width * 1.f / source_height;
    if (source_ratio > target_ratio) {
      scale_width = source_ratio / target_ratio;
      scale_height = 1.f;
    } else {
      scale_width = 1.f;
      scale_height = target_ratio / source_ratio;
    }
  }
  auto width = scale_width * target_width;
  auto height = scale_height * target_height;
  vertex_coordinate[0] = (target_width - width) / 2;
  vertex_coordinate[1] = (target_height - height) / 2 + height;

  vertex_coordinate[2] = (target_width - width) / 2 + width;
  vertex_coordinate[3] = (target_height - height) / 2 + height;
  vertex_coordinate[4] = (target_width - width) / 2;
  vertex_coordinate[5] = (target_height - height) / 2;
  vertex_coordinate[6] = (target_width - width) / 2 + width;
  vertex_coordinate[7] = (target_height - height) / 2;
}

void Background::DeleteBackgroundImageTexture() {
  if (background_image_texture_ != 0) {
    glDeleteTextures(1, &background_image_texture_);
  }
  background_image_texture_ = 0;
}

void Background::ResetBackgroundColor() {
  red_ = 0;
  green_ = 0;
  blue_ = 0;
  alpha_ = 0;
}

GLuint Background::GetScaleTexture(GLuint texture_id, int frame_width, int frame_height) {
  if (background_type_ == BackgroundType::kBackgroundBlur || background_type_ == BackgroundType::kBackgroundImage) {
    /// 只有高斯模糊的值大于0时才缩小
    if (blur_ < MIN_BLUR_VALUE || blur_ > MAX_BLUR_VALUE) {
      return texture_id;
    }
    if (scale_frame_buffer_ == nullptr) {
      scale_frame_buffer_ = new FrameBuffer();
    }
    auto ratio = BLUR_WIDTH_MAX_SIZE / static_cast<float>(frame_width);
    int width = BLUR_WIDTH_MAX_SIZE;
    auto height = static_cast<int>(frame_height * ratio);
    scale_frame_buffer_->SetOutput(width, height);
    return scale_frame_buffer_->ProcessImage(texture_id, width, height);
  }
  return texture_id;
}

GLuint Background::GetBackgroundTextureId(GLuint texture_id, int frame_width, int frame_height) {
  if (background_type_ == BackgroundType::kBackgroundBlur || background_type_ == BackgroundType::kBackgroundImage) {
    auto ratio = BLUR_WIDTH_MAX_SIZE / static_cast<float>(frame_width);
    auto height = static_cast<int>(frame_height * ratio);
    if (blur_ >= MIN_BLUR_VALUE && gaussian_blur_ == nullptr) {
      gaussian_blur_ = new GaussianBlur(BLUR_WIDTH_MAX_SIZE, height);
    }
    auto background_texture = background_image_texture_ > 0 ? background_image_texture_ : texture_id;
    GLuint scale_texture_id = background_texture;
    auto texture_width = background_texture > 0 ? image_width_ : frame_width;
    auto texture_height = background_texture > 0 ? image_height_ : frame_height;
    if (texture_width > BLUR_WIDTH_MAX_SIZE) {
      /// 纹理的宽大于模糊设置的最大宽时, 缩小纹理
      scale_texture_id = GetScaleTexture(background_texture, texture_width, texture_height);
    }
    if (gaussian_blur_ != nullptr) {
      gaussian_blur_->SetBlurRadius(blur_);
      scale_texture_id = gaussian_blur_->ProcessImage(scale_texture_id);
    }
    return scale_texture_id;
  }
  return texture_id;
}

GLuint Background::CreateFrameBuffer(int width, int height) {
  if (frame_buffer_id_ == 0) {
    glGenTextures(1, &texture_id_);
    glGenFramebuffers(1, &frame_buffer_id_);
  }
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id_);
  glSafeTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrap_);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrap_);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id_, 0);

  int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
#if __ANDROID__
    LOGE("%s %s %d frame buffer error", __FILE_NAME__, __func__ , __LINE__);
#endif
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return frame_buffer_id_;
}

void Background::DeleteFrameBuffer() {
  if (frame_buffer_id_ != 0) {
    glDeleteFramebuffers(1, &frame_buffer_id_);
  }
  frame_buffer_id_ = 0;
  if (texture_id_ != 0) {
    glDeleteTextures(1, &texture_id_);
  }
  texture_id_ = 0;
}

}  // namespace effect