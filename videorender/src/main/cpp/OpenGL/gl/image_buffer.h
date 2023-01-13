

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "OpenGL/gl.h"

namespace effect {

/**
 * 把图片渲染到一个纹理上
 */
class ImageBuffer {
 public:
  ImageBuffer(int width, int height, unsigned char* image_buffer) : width_(width), height_(height) {
    if (width % 2 != 0 || height % 2 != 0) {
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }
    glGenTextures(1, &texture_id_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSafeTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  ~ImageBuffer() {
    glDeleteTextures(1, &texture_id_);
  }

  void UpdateImage(int width, int height, unsigned char* image_buffer) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glSafeTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
    width_ = width;
    height_ = height;
  }

  GLuint GetTextureId() {
    return texture_id_;
  }

  int GetWidth() { return width_; }
  int GetHeight() { return height_; }

 private:
  GLuint texture_id_;
  int width_;
  int height_;
};

}  // namespace effect