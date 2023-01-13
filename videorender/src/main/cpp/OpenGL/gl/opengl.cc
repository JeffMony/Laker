
#include "opengl.h"
#include <cstring>
#include "OpenGL/gl.h"
#include "log.h"
#include "common.h"
#include "program.h"

namespace effect {

OpenGL::OpenGL()
    : type_(TEXTURE_2D)
    , program_(0)
    , width_(0)
    , height_(0) {
  vertex_coordinate_ = new GLfloat[8];
  memcpy(vertex_coordinate_, DEFAULT_VERTEX_COORDINATE, 8 * sizeof(GLfloat));
  program_ = Program::CreateProgram(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
}

OpenGL::OpenGL(int width, int height)
    : type_(TEXTURE_2D)
    , program_(0)
    , width_(width)
    , height_(height) {
  vertex_coordinate_ = new GLfloat[8];
  memcpy(vertex_coordinate_, DEFAULT_VERTEX_COORDINATE, 8 * sizeof(GLfloat));
  program_ = Program::CreateProgram(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
}

OpenGL::OpenGL(const char* vertex, const char* fragment)
    : type_(TEXTURE_2D)
    , program_(0)
    , width_(0)
    , height_(0) {
  vertex_coordinate_ = new GLfloat[8];
  memcpy(vertex_coordinate_, DEFAULT_VERTEX_COORDINATE, 8 * sizeof(GLfloat));
  program_ = Program::CreateProgram(vertex, fragment);
}

OpenGL::OpenGL(int width, int height, const char* vertex, const char* fragment)
    : type_(TEXTURE_2D)
    , program_(0)
    , width_(width)
    , height_(height) {
  vertex_coordinate_ = new GLfloat[8];
  memcpy(vertex_coordinate_, DEFAULT_VERTEX_COORDINATE, 8 * sizeof(GLfloat));
  program_ = Program::CreateProgram(vertex, fragment);
}

OpenGL::~OpenGL() {
  if (program_ > 0) {
    glDeleteProgram(program_);
    program_ = 0;
  }
  SAFE_DELETE_ARRAY(vertex_coordinate_)
}

void OpenGL::SetTextureType(TextureType type) {
  type_ = type;
}

void OpenGL::SetFrame(int source_width, int source_height, int target_width, int target_height, FrameType frame_type) {
  LOGI("enter: %s %s %d source_width: %d source_height: %d target_width: %d target_height: %d frame_type: %d",
       __FILE_NAME__, __func__ , __LINE__, source_width, source_height, target_width, target_height, frame_type);
  float target_ratio = target_width * 1.f / target_height;
  float scale_width = 1.0F;
  float scale_height = 1.0F;
  if (frame_type == FIT) {
    float source_ratio = source_width * 1.f / source_height;
    if (source_ratio > target_ratio) {
      scale_width = 1.f;
      scale_height = target_ratio / source_ratio;
    } else {
      scale_width = source_ratio / target_ratio;
      scale_height = 1.f;
    }
  } else if (frame_type == FILL) {
    float source_ratio = source_width * 1.f / source_height;
    if (source_ratio > target_ratio) {
      scale_width = source_ratio / target_ratio;
      scale_height = 1.f;
    } else {
      scale_width = 1.f;
      scale_height = target_ratio / source_ratio;
    }
  }
  LOGI("%s %s %d scale_width: %f scale_height: %f", __FILE_NAME__, __func__ , __LINE__, scale_width, scale_height);
  vertex_coordinate_[0] = -scale_width;
  vertex_coordinate_[1] = -scale_height;
  vertex_coordinate_[2] = scale_width;
  vertex_coordinate_[3] = -scale_height;
  vertex_coordinate_[4] = -scale_width;
  vertex_coordinate_[5] = scale_height;
  vertex_coordinate_[6] = scale_width;
  vertex_coordinate_[7] = scale_height;
}

void OpenGL::SetOutput(int width, int height) {
  this->width_ = width;
  this->height_ = height;
}

void OpenGL::SetInt(const char *name, int value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform1i(location, value);
}

void OpenGL::SetIntArray(const char *name, int size, int *value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform1iv(location, size, value);
}

void OpenGL::SetFloat(const char *name, float value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform1f(location, value);
}

void OpenGL::SetFloatArray(const char *name, int size, GLfloat *value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform1fv(location, size, value);
}

void OpenGL::SetUniform4f(const char *name, float v0, float v1, float v2, float v3) {
  auto location = glGetUniformLocation(program_, name);
  glUniform4f(location, v0, v1, v2, v3);
}

void OpenGL::SetFloatVec2(const char *name, int size, GLfloat *value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform2fv(location, size, value);
}

void OpenGL::SetFloatVec3(const char *name, int size, GLfloat *value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform3fv(location, size, value);
}

void OpenGL::SetFloatVec4(const char *name, int size, GLfloat *value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform4fv(location, size, value);
}

void OpenGL::SetUniformMatrix3f(const char *name, int size, GLfloat *matrix) {
  auto location = glGetUniformLocation(program_, name);
  glUniformMatrix3fv(location, size, GL_FALSE, matrix);
}

void OpenGL::SetUniformMatrix4f(const char *name, int size, GLfloat *matrix) {
  auto location = glGetUniformLocation(program_, name);
  glUniformMatrix4fv(location, size, GL_FALSE, matrix);
}

void OpenGL::ProcessImage(GLuint texture_id) {
  ProcessImage(texture_id, vertex_coordinate_, DEFAULT_TEXTURE_COORDINATE);
}

void OpenGL::ProcessImage(GLuint texture_id, GLfloat *matrix) {
  ProcessImage(texture_id, vertex_coordinate_, DEFAULT_TEXTURE_COORDINATE, matrix);
}

void OpenGL::ProcessImage(GLuint texture_id, const GLfloat *vertex_coordinate, const GLfloat *texture_coordinate) {
  ProcessImage(texture_id, vertex_coordinate, texture_coordinate, nullptr);
}

void OpenGL::ProcessImage(GLuint texture_id, const GLfloat *vertex_coordinate,
                          const GLfloat *texture_coordinate, GLfloat *matrix) {
  if (program_ == 0) {
    LOGE("%s %s %d program error: %d", __FILE_NAME__, __func__ , __LINE__, program_);
    return;
  }
  if (this->width_ > 0 && this->height_ > 0) {
    GL_CHECK(glViewport(0, 0, this->width_, this->height_));
  }
  GL_CHECK(glClearColor(0.f, 0.f, 0.f, 1.f));
  GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  GL_CHECK(glUseProgram(program_));
  auto position_attribute = glGetAttribLocation(program_, "position");
  GL_CHECK(glEnableVertexAttribArray(position_attribute));
  GL_CHECK(glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertex_coordinate));
  auto texture_coordinate_attribute = glGetAttribLocation(program_, "inputTextureCoordinate");
  GL_CHECK(glEnableVertexAttribArray(texture_coordinate_attribute));
  GL_CHECK(glVertexAttribPointer(texture_coordinate_attribute, 2, GL_FLOAT, GL_FALSE,
                                 2 * sizeof(GLfloat), texture_coordinate));
  if (matrix != nullptr) {
    GL_CHECK(SetUniformMatrix4f("textureMatrix", 1, matrix));
  }
  GL_CHECK(glActiveTexture(GL_TEXTURE0));
#if __ANDROID__
  GL_CHECK(glBindTexture(type_ == TEXTURE_OES ? GL_TEXTURE_EXTERNAL_OES : GL_TEXTURE_2D, texture_id));
#elif __APPLE__
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture_id));
#endif
  SetInt("inputImageTexture", 0);
  RunOnDrawTasks();
  GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
  OnDrawArrays();
  GL_CHECK(glDisableVertexAttribArray(position_attribute));
  GL_CHECK(glDisableVertexAttribArray(texture_coordinate_attribute));
#if __ANDROID__
  GL_CHECK(glBindTexture(type_ == TEXTURE_OES ? GL_TEXTURE_EXTERNAL_OES : GL_TEXTURE_2D, 0));
#elif __APPLE__
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
#endif
}

void OpenGL::RunOnDrawTasks() {
  // NOLINT
}

void OpenGL::OnDrawArrays() {
  // NOLINT
}

}  // namespace effect