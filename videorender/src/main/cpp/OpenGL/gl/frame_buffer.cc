
#include "frame_buffer.h"
#include "OpenGL/gl.h"
#include "log.h"

namespace effect {

FrameBuffer::FrameBuffer() : OpenGL()
    , start_time_(0)
    , end_time_(INT32_MAX)
    , frame_buffer_id_(0)
    , texture_id_(0)
    , width_(0)
    , height_(0)
    , image_texture_id_(0) {}

FrameBuffer::FrameBuffer(const char* vertex, const char* fragment) : OpenGL(vertex, fragment)
    , start_time_(0)
    , end_time_(INT32_MAX)
    , frame_buffer_id_(0)
    , texture_id_(0)
    , width_(0)
    , height_(0)
    , image_texture_id_(0) {}

FrameBuffer::FrameBuffer(int width, int height, const char* vertex, const char* fragment)
    : OpenGL(width, height, vertex, fragment)
    , start_time_(0)
    , end_time_(INT32_MAX)
    , frame_buffer_id_(0)
    , texture_id_(0)
    , width_(0)
    , height_(0)
    , image_texture_id_(0) {}

FrameBuffer::~FrameBuffer() {
  DeleteFrameBuffer();
  if (image_texture_id_ > 0) {
    glDeleteTextures(1, &image_texture_id_);
  }
  image_texture_id_ = 0;
}

void FrameBuffer::SetStartTime(int time) {
  start_time_ = time;
}

void FrameBuffer::SetEndTime(int time) {
  end_time_ = time;
}

GLuint FrameBuffer::ProcessImage(uint8_t *data, int width, int height, int64_t current_time) {
  if (image_texture_id_ == 0) {
    glGenTextures(1, &image_texture_id_);
  }
  if (width % 2 != 0 || height % 2 != 0) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  }
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, image_texture_id_);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glSafeTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glBindTexture(GL_TEXTURE_2D, 0);
  return ProcessImage(image_texture_id_, width, height,
                      DEFAULT_VERTEX_COORDINATE, TEXTURE_COORDINATE_VERTICAL_FLIP, current_time);
}

GLuint FrameBuffer::ProcessImageWithMatrix(uint8_t *data, int width, int height, const GLfloat* vertex_coordinate,
                                           const GLfloat* texture_coordinate, GLfloat* matrix, int64_t current_time) {
  if (image_texture_id_ == 0) {
    glGenTextures(1, &image_texture_id_);
  }
  if (width % 2 != 0 || height % 2 != 0) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  }
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, image_texture_id_);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glBindTexture(GL_TEXTURE_2D, 0);
  return ProcessImage(image_texture_id_, width, height, vertex_coordinate,
                      texture_coordinate, matrix, current_time);
}

GLuint FrameBuffer::ProcessImage(GLuint texture_id, int width, int height, int64_t current_time) {
  return ProcessImage(texture_id, width, height, nullptr, current_time);
}

GLuint FrameBuffer::ProcessImage(GLuint texture_id, int width, int height, GLfloat *matrix, int64_t current_time) {
  return ProcessImage(texture_id, width, height,
                      DEFAULT_VERTEX_COORDINATE, DEFAULT_TEXTURE_COORDINATE, matrix, current_time);
}

GLuint FrameBuffer::ProcessImage(GLuint texture_id, int width, int height, const GLfloat *vertex_coordinate,
                                 const GLfloat *texture_coordinate, int64_t current_time) {
  return ProcessImage(texture_id, width, height, vertex_coordinate, texture_coordinate, nullptr, current_time);
}

GLuint FrameBuffer::ProcessImage(GLuint texture_id, int width, int height, const GLfloat *vertex_coordinate,
                                 const GLfloat *texture_coordinate, GLfloat *matrix, int64_t current_time) {
  CreateFrameBuffer(width, height);
  /// 只有在当前时间范围内才会执行
  if (current_time >= start_time_ && current_time <= end_time_) {
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id_))
    if (width % 2 != 0 || height % 2 != 0) {
      GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1))
    }
    OpenGL::ProcessImage(texture_id, vertex_coordinate, texture_coordinate, matrix);
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0))
    return texture_id_;
  }
  return texture_id;
}

void FrameBuffer::CreateFrameBuffer(int width, int height) {
  if (width == width_ && height == height_) {
    return;
  }

  if (frame_buffer_id_ == 0) {
    GL_CHECK(glGenTextures(1, &texture_id_))
    GL_CHECK(glGenFramebuffers(1, &frame_buffer_id_))
  }
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id_))
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture_id_))
  GL_CHECK(glSafeTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                            nullptr))
  GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
  GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))
  GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
  GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
  GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id_, 0))
  int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    LOGE("%s %s %d frame buffer width: %d height: %d error", __FILE_NAME__, __func__ , __LINE__, width, height);
  }
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0))
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0))
  width_ = width;
  height_ = height;
}

void FrameBuffer::DeleteFrameBuffer() {
  if (frame_buffer_id_ > 0) {
    GL_CHECK(glDeleteFramebuffers(1, &frame_buffer_id_))
    frame_buffer_id_ = 0;
  }
  if (texture_id_ > 0) {
    GL_CHECK(glDeleteTextures(1, &texture_id_))
    texture_id_ = 0;
  }
}

}  // namespace effect