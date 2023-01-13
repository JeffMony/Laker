
#include "process_buffer.h"
#include <cstring>
#include "OpenGL/gl.h"
#include "log.h"
#include "common.h"
#include "program.h"

namespace effect {

ProcessBuffer::ProcessBuffer()
    : frame_buffer_id_(0)
    , texture_id_(0)
    , program_(0)
    , vao_id_(0)
    , vertex_coordinate_(nullptr)
    , texture_coordinate_(nullptr)
    , red_(0.f)
    , green_(0.f)
    , blue_(0.f)
    , alpha_(1.f)
    , view_width_(0)
    , view_height_(0)
    , width_(0)
    , height_(0)
    , x_(0)
    , y_(0) {
  vertex_coordinate_ = new GLfloat[8];
  texture_coordinate_ = new GLfloat[8];
  memcpy(vertex_coordinate_, DEFAULT_VERTEX_COORDINATE, 8 * sizeof(GLfloat));
  memcpy(texture_coordinate_, DEFAULT_TEXTURE_COORDINATE, 8 * sizeof(GLfloat));
}

ProcessBuffer::~ProcessBuffer() {
  Destroy();
  SAFE_DELETE_ARRAY(vertex_coordinate_);
  SAFE_DELETE_ARRAY(texture_coordinate_);
}

int ProcessBuffer::Init(const char *vertex_shader, const char *fragment_shader) {
  if (program_ > 0) {
    return 0;
  }
  program_ = Program::CreateProgram(vertex_shader, fragment_shader);
  if (program_ <= 0) {
    return -1;
  }
  return 0;
}

void ProcessBuffer::SetInt(const char *name, int value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform1i(location, value);
}

void ProcessBuffer::SetFloat(const char *name, float value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform1f(location, value);
}

void ProcessBuffer::SetUniform4f(const char *name, float v0, float v1, float v2, float v3) {
  auto location = glGetUniformLocation(program_, name);
  glUniform4f(location, v0, v1, v2, v3);
}

void ProcessBuffer::SetFloatVec1(const char *name, int size, GLfloat *value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform1fv(location, size, value);
}

void ProcessBuffer::SetFloatVec2(const char *name, int size, GLfloat *value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform2fv(location, size, value);
}

void ProcessBuffer::SetFloatVec3(const char *name, int size, GLfloat *value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform3fv(location, size, value);
}

void ProcessBuffer::SetFloatVec4(const char *name, int size, GLfloat *value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform4fv(location, size, value);
}

void ProcessBuffer::SetUniformMatrix3f(const char *name, int size, GLfloat *matrix) {
  auto location = glGetUniformLocation(program_, name);
  glUniformMatrix3fv(location, size, GL_FALSE, matrix);
}

void ProcessBuffer::SetUniformMatrix4f(const char *name, int size, GLfloat *matrix) {
  auto location = glGetUniformLocation(program_, name);
  glUniformMatrix4fv(location, size, GL_FALSE, matrix);
}

void ProcessBuffer::ActiveProgram(int width, int height) {
  if (program_ <= 0) {
    return;
  }
  GL_CHECK(glUseProgram(program_))
}

void ProcessBuffer::SetViewportPosition(int x, int y) {
  x_ = x;
  y_ = y;
}

void ProcessBuffer::SetViewportSize(int width, int height) {
  view_width_ = width;
  view_height_ = height;
}

void ProcessBuffer::Clear(bool clear_color_buffer_bit) {
  GL_CHECK(glClearColor(red_, green_, blue_, alpha_))
  if (clear_color_buffer_bit) {
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))
  }
  if (view_width_ > 0 && view_height_ > 0) {
    GL_CHECK(glViewport(x_, y_, view_width_, view_height_))
  }
}

void ProcessBuffer::SetClearColor(float red, float green, float blue, float alpha) {
  red_ = red;
  green_ = green;
  blue_ = blue;
  alpha_ = alpha;
}

void ProcessBuffer::ActiveAttributeName(const char* name,
                                        GLint size,
                                        GLenum type,
                                        GLboolean normalized,
                                        GLsizei stride,
                                        GLvoid *pointer) {
  auto index = glGetAttribLocation(program_, name);
  if (index >= 0) {
    auto attribute = static_cast<GLuint>(index);
    GL_CHECK(glEnableVertexAttribArray(attribute))
    GL_CHECK(glVertexAttribPointer(attribute,
                                   size,
                                   type,
                                   normalized,
                                   stride,
                                   pointer));
  }

}

void ProcessBuffer::ActiveBufferPosition(GLint size,
                                         GLenum type,
                                         GLboolean normalized,
                                         GLsizei stride,
                                         GLvoid* pointer) {
  auto positionAttribute = static_cast<GLuint>(glGetAttribLocation(program_, "position"));
  GL_CHECK(glEnableVertexAttribArray(positionAttribute))
  GL_CHECK(glVertexAttribPointer(positionAttribute,
                                 size,
                                 type,
                                 normalized,
                                 stride,
                                 pointer))
}

void ProcessBuffer::ActiveTextureAttribute(GLint size,
                                           GLenum type,
                                           GLboolean normalized,
                                           GLsizei stride,
                                           GLvoid* pointer) {
  auto textureCoordinateAttribute = static_cast<GLuint>(glGetAttribLocation(program_, "inputTextureCoordinate"));
  GL_CHECK(glEnableVertexAttribArray(textureCoordinateAttribute))
  GL_CHECK(glVertexAttribPointer(textureCoordinateAttribute,
                                 size,
                                 type,
                                 normalized,
                                 stride,
                                 pointer))
}

void ProcessBuffer::ActiveTextureAttribute(const char *name, GLfloat *texture_coordinate, int texture_coordinate_size) {
  auto textureCoordinateAttribute = static_cast<GLuint>(glGetAttribLocation(program_, name));
  GL_CHECK(glEnableVertexAttribArray(textureCoordinateAttribute))
  GL_CHECK(glVertexAttribPointer(textureCoordinateAttribute,
                                 texture_coordinate_size,
                                 GL_FLOAT, GL_FALSE,
                                 texture_coordinate_size * sizeof(GLfloat),
                                 texture_coordinate))
}

void ProcessBuffer::ActiveAttribute(GLfloat* vertex_coordinate,
                                    GLfloat* texture_coordinate,
                                    int vertex_coordinate_size,
                                    int texture_coordinate_size) {
  auto positionAttribute = static_cast<GLuint>(glGetAttribLocation(program_, "position"));
  GL_CHECK(glEnableVertexAttribArray(positionAttribute))
  GL_CHECK(glVertexAttribPointer(positionAttribute,
                                 vertex_coordinate_size,
                                 GL_FLOAT, GL_FALSE,
                                 vertex_coordinate_size * sizeof(GLfloat),
                                 vertex_coordinate == nullptr ? vertex_coordinate_ : vertex_coordinate))
  auto texture_coordinate_index = glGetAttribLocation(program_, "inputTextureCoordinate");
  if (texture_coordinate_index >= 0) {
    auto textureCoordinateAttribute = static_cast<GLuint>(texture_coordinate_index);
    GL_CHECK(glEnableVertexAttribArray(textureCoordinateAttribute))
    GL_CHECK(glVertexAttribPointer(textureCoordinateAttribute,
                                   texture_coordinate_size,
                                   GL_FLOAT,
                                   GL_FALSE,
                                   texture_coordinate_size * sizeof(GLfloat),
                                   texture_coordinate == nullptr ? texture_coordinate_
                                                                 : texture_coordinate))

  }
}

void ProcessBuffer::ActiveVapAttribute(GLfloat* vertex_coordinate,
                                       GLfloat* texture_coordinate,
                                       GLfloat* rgb_texture_coordinate,
                                       GLfloat* alpha_texture_coordinate,
                                       int vertex_coordinate_size,
                                       int texture_coordinate_size) {
  auto positionAttribute = static_cast<GLuint>(glGetAttribLocation(program_, "position"));
  GL_CHECK(glEnableVertexAttribArray(positionAttribute))
  GL_CHECK(glVertexAttribPointer(positionAttribute,
                                 vertex_coordinate_size,
                                 GL_FLOAT, GL_FALSE,
                                 vertex_coordinate_size * sizeof(GLfloat),
                                 vertex_coordinate == nullptr ? vertex_coordinate_ : vertex_coordinate))
  auto texture_coordinate_index = glGetAttribLocation(program_, "inputTextureCoordinate");
  if (texture_coordinate_index >= 0) {
    auto textureCoordinateAttribute = static_cast<GLuint>(texture_coordinate_index);
    GL_CHECK(glEnableVertexAttribArray(textureCoordinateAttribute))
    GL_CHECK(glVertexAttribPointer(textureCoordinateAttribute,
                                   texture_coordinate_size,
                                   GL_FLOAT,
                                   GL_FALSE,
                                   texture_coordinate_size * sizeof(GLfloat),
                                   texture_coordinate == nullptr ? texture_coordinate_
                                                                 : texture_coordinate))
  }
  auto texture_coordinate_rgb_index = glGetAttribLocation(program_, "inputTextureCoordinateRGB");
  if (texture_coordinate_rgb_index >= 0) {
    auto textureCoordinateAttribute = static_cast<GLuint>(texture_coordinate_rgb_index);
    GL_CHECK(glEnableVertexAttribArray(textureCoordinateAttribute))
    GL_CHECK(glVertexAttribPointer(textureCoordinateAttribute,
                                   texture_coordinate_size,
                                   GL_FLOAT,
                                   GL_FALSE,
                                   texture_coordinate_size * sizeof(GLfloat),
                                   rgb_texture_coordinate == nullptr ? texture_coordinate_
                                                                     : rgb_texture_coordinate))
  }
  auto texture_coordinate_a_index = glGetAttribLocation(program_, "inputTextureCoordinateA");
  if (texture_coordinate_a_index >= 0) {
    auto textureCoordinateAttribute = static_cast<GLuint>(texture_coordinate_a_index);
    GL_CHECK(glEnableVertexAttribArray(textureCoordinateAttribute))
    GL_CHECK(glVertexAttribPointer(textureCoordinateAttribute,
                                   texture_coordinate_size,
                                   GL_FLOAT,
                                   GL_FALSE,
                                   texture_coordinate_size * sizeof(GLfloat),
                                   alpha_texture_coordinate == nullptr ? texture_coordinate_
                                                                       : alpha_texture_coordinate))
  }
}

void ProcessBuffer::ActiveTexture(GLuint texture_id, int index, const char* name) {
  GLenum target = GL_TEXTURE1 + index;
  GL_CHECK(glActiveTexture(target))
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture_id));
  SetInt(name, 1 + index);
}

void ProcessBuffer::DrawArrays() {
  GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4))
}

int ProcessBuffer::CreateFrameBuffer(int width, int height) {
  if (width == width_ && height == height_) {
    return 0;
  }
  if (frame_buffer_id_ == 0) {
    GL_CHECK(glGenTextures(1, &texture_id_))
    GL_CHECK(glGenFramebuffers(1, &frame_buffer_id_))
  }
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture_id_))
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id_))
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr))
  GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
  GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))
  GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
  GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
  GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id_, 0))

  int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    LOGE("%s %s %d frame buffer error", __FILE_NAME__, __func__ , __LINE__);
    return -1;
  }
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0))
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0))
  width_ = width;
  height_ = height;
  return 0;
}

void ProcessBuffer::Disable() {
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0))
}

void ProcessBuffer::BindFrameBuffer() {
  if (frame_buffer_id_ == 0) {
    return;
  }
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id_))
//  GL_CHECK(glSafeTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr))
//  GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
//  GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))
//  GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
//  GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
//  GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id_, 0))
}

void ProcessBuffer::UnBindFrameBuffer() {
//  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0))
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0))
}

void ProcessBuffer::Destroy() {
  if (vao_id_ > 0) {
    GL_CHECK(glDeleteBuffers(1, &vao_id_))
    vao_id_ = 0;
  }
  if (program_ > 0) {
    glDeleteProgram(program_);
    program_ = 0;
  }
  if (frame_buffer_id_ > 0) {
    glDeleteFramebuffers(1, &frame_buffer_id_);
    frame_buffer_id_ = 0;
  }
  if (texture_id_ > 0) {
    glDeleteTextures(1, &texture_id_);
    texture_id_ = 0;
  }
  width_ = 0;
  height_ = 0;
  texture_id_ = 0;
  frame_buffer_id_ = 0;
  SAFE_DELETE_ARRAY(vertex_coordinate_)
  SAFE_DELETE_ARRAY(texture_coordinate_)
}

}  // namespace effect