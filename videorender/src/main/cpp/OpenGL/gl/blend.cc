
#include "blend.h"
#include <cstdio>
#include <cstdlib>
#include "OpenGL/gl.h"
#include "log.h"
#include "program.h"

namespace effect {

Blend::Blend(const char *fragment_shader)
    : frame_buffer_texture_id_(0)
    , frame_buffer_id_(0)
    , frame_buffer_(nullptr)
    , source_width_(0)
    , source_height_(0) {
  default_vertex_coordinates_ = new GLfloat[8];
  default_texture_coordinates_ = new GLfloat[8];
  default_vertex_coordinates_[0] = -1.0F;
  default_vertex_coordinates_[1] = -1.0F;
  default_vertex_coordinates_[2] = 1.0F;
  default_vertex_coordinates_[3] = -1.0F;
  default_vertex_coordinates_[4] = -1.0F;
  default_vertex_coordinates_[5] = 1.0F;
  default_vertex_coordinates_[6] = 1.0F;
  default_vertex_coordinates_[7] = 1.0F;

  default_texture_coordinates_[0] = 0.0F;
  default_texture_coordinates_[1] = 0.0F;
  default_texture_coordinates_[2] = 1.0F;
  default_texture_coordinates_[3] = 0.0F;
  default_texture_coordinates_[4] = 0.0F;
  default_texture_coordinates_[5] = 1.0F;
  default_texture_coordinates_[6] = 1.0F;
  default_texture_coordinates_[7] = 1.0F;

  program_ = Program::CreateProgram(BLEND_VERTEX_SHADER, fragment_shader);
  second_program_ = Program::CreateProgram(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
  glUseProgram(second_program_);
}

Blend::~Blend() {
  if (nullptr != default_vertex_coordinates_) {
    delete[] default_vertex_coordinates_;
    default_vertex_coordinates_ = nullptr;
  }
  if (nullptr != default_texture_coordinates_) {
    delete[] default_texture_coordinates_;
    default_texture_coordinates_ = nullptr;
  }
  DeleteFrameBuffer();
  if (program_ != 0) {
    glDeleteProgram(program_);
    program_ = 0;
  }
  if (second_program_ != 0) {
    glDeleteProgram(second_program_);
    second_program_ = 0;
  }
}

void Blend::CreateFrameBuffer(int width, int height) {
  if (frame_buffer_texture_id_ == 0) {
    glGenTextures(1, &frame_buffer_texture_id_);
    glGenFramebuffers(1, &frame_buffer_id_);
  }
  glBindTexture(GL_TEXTURE_2D, frame_buffer_texture_id_);
  glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id_);
  glSafeTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                   nullptr);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         frame_buffer_texture_id_, 0);

  int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
#if __ANDROID__
    LOGE("%s %s %d frame buffer error", __FILE_NAME__, __func__ , __LINE__);
#endif
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Blend::DeleteFrameBuffer() {
  if (frame_buffer_texture_id_ != 0) {
    glDeleteTextures(1, &frame_buffer_texture_id_);
    frame_buffer_texture_id_ = 0;
  }
  if (frame_buffer_id_ != 0) {
    glDeleteFramebuffers(1, &frame_buffer_id_);
    frame_buffer_id_ = 0;
  }
  if (nullptr != frame_buffer_) {
    delete frame_buffer_;
    frame_buffer_ = nullptr;
  }
}

int Blend::ProcessImage(int texture_id, int sticker_texture_id, int frame_width, int frame_height, GLfloat *matrix,
                        float alpha_factor) {
  return ProcessImage(texture_id, sticker_texture_id, frame_width, frame_height, frame_width, frame_height, matrix,
                      default_vertex_coordinates_, default_texture_coordinates_, alpha_factor);
}

int Blend::ProcessImage(int texture_id, int sticker_texture_id, int frame_width, int frame_height, GLfloat *matrix,
                        GLfloat *texture_coordinate, float alpha_factor) {
  return ProcessImage(texture_id, sticker_texture_id, frame_width, frame_height, frame_width, frame_height, matrix,
                      default_vertex_coordinates_, texture_coordinate, alpha_factor);
}

int Blend::ProcessImage(int texture_id, int sticker_texture_id, int frame_width, int frame_height,
                        int target_width, int target_height, GLfloat *matrix,
                        GLfloat *vertex_coordinate, GLfloat *texture_coordinate, float alpha_factor) {
  if (source_width_ != frame_width || source_height_ != frame_height) {
    source_width_ = frame_width;
    source_height_ = frame_height;
    CreateFrameBuffer(frame_width, frame_height);
  }
  int frame_buffer_texture_id = texture_id;
  if (texture_id == frame_buffer_texture_id_) {
    // todo 这里有可能会导致图像放大
    if (frame_buffer_ == nullptr) {
      frame_buffer_ = new FrameBuffer();
    }
    frame_buffer_texture_id = frame_buffer_->ProcessImage(texture_id, frame_width, frame_height);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id_);
  glViewport(0, 0, frame_width, frame_height);
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(second_program_);
  //    glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, frame_buffer_texture_id);
  auto blend_input_image_texture_location = glGetUniformLocation(second_program_,
                                                                 "inputImageTexture");
  glUniform1i(blend_input_image_texture_location, 2);
  auto position2_location = glGetAttribLocation(second_program_, "position");
  glEnableVertexAttribArray(position2_location);
  glVertexAttribPointer(position2_location, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                        default_vertex_coordinates_);
  auto input_texture_coordinate2_location = glGetAttribLocation(second_program_,
                                                                "inputTextureCoordinate");
  glEnableVertexAttribArray(input_texture_coordinate2_location);
  glVertexAttribPointer(input_texture_coordinate2_location, 2, GL_FLOAT, GL_FALSE,
                        2 * sizeof(GLfloat), default_texture_coordinates_);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glDisableVertexAttribArray(position2_location);
  glDisableVertexAttribArray(input_texture_coordinate2_location);

  glUseProgram(program_);
  glEnable(GL_BLEND);
  //    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, frame_buffer_texture_id);
  auto input_image_texture_location = glGetUniformLocation(program_, "inputImageTexture");
  glUniform1i(input_image_texture_location, 2);
  glActiveTexture(GL_TEXTURE3);
  // bind 贴纸纹理
  glBindTexture(GL_TEXTURE_2D, sticker_texture_id);
  auto input_image_texture2_location = glGetUniformLocation(program_, "inputImageTexture2");
  glUniform1i(input_image_texture2_location, 3);
  auto alpha_factor_location = glGetUniformLocation(program_, "alphaFactor");
  glUniform1f(alpha_factor_location, alpha_factor);
  auto matrix_location = glGetUniformLocation(program_, "matrix");

  // 设置矩阵
  if (nullptr == matrix) {
    GLfloat m[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, m);
  } else {
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);
  }

  auto position_location = glGetAttribLocation(program_, "position");
  glEnableVertexAttribArray(position_location);
  glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                        vertex_coordinate);
  auto input_texture_coordinate_location = glGetAttribLocation(program_,
                                                               "inputTextureCoordinate");
  glEnableVertexAttribArray(input_texture_coordinate_location);
  glVertexAttribPointer(input_texture_coordinate_location, 2, GL_FLOAT, GL_FALSE,
                        2 * sizeof(GLfloat), texture_coordinate);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glDisableVertexAttribArray(position_location);
  glDisableVertexAttribArray(input_texture_coordinate_location);
  glDisable(GL_BLEND);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return frame_buffer_texture_id_;
}

}  // namespace effect