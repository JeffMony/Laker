
#ifndef OPENGL_GL_H_
#define OPENGL_GL_H_

#include <stdio.h>
#include <algorithm>
#include "log.h"
#include "common.h"

static const char* GetRenderErrorCodeString(GLenum errorCode) {
  switch (errorCode) {
    case GL_NO_ERROR:
      return "GL_NO_ERROR";

    case GL_INVALID_ENUM:
      return "GL_INVALID_ENUM";

    case GL_INVALID_VALUE:
      return "GL_INVALID_VALUE";

    case GL_INVALID_OPERATION:
      return "GL_INVALID_OPERATION";

    case GL_OUT_OF_MEMORY:
      return "GL_OUT_OF_MEMORY";

    default:
      return "UncommonError";
  }
}

#ifndef GL_CHECK
#define GL_CHECK(x)                                                                              \
    x;                                                                                           \
    {                                                                                            \
        GLenum glError = glGetError();                                                           \
        if(glError != GL_NO_ERROR) {                                                             \
            LOGE("glGetError() = %i (0x%.8x) at %s:%i\n", glError, glError, __FILE__, __LINE__); \
        }                                                                                        \
    }
#endif

/**
 * 安全的上传一个纹理
 * 当buffer 宽高超出设备最大纹理限制时，内部会crop buffer ，保证纹理宽高在安全值范围，此时画面会发生错乱，保证程序不崩溃
 * @param image_scale 该参数内部会 scale buffer，效率较低，在单张图片资源加载的时候可以开启。
 */
static void glSafeTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height,
                              GLint border, GLenum format, GLenum type, unsigned char *pixels,
                              bool image_scale = false, int test_max_texture = 0) {
  GLint texture_max;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texture_max);
  int draw_max = std::max(width, height);
  // 👇是调试代码
  if (test_max_texture > 0) {
    texture_max = test_max_texture;
  }
  if(draw_max <= texture_max) {
    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
  } else {
    float scale = static_cast<float>(texture_max) / static_cast<float>(draw_max);
    int out_width = static_cast<float>(floor(width * scale));
    int out_height = static_cast<float>(floor(height * scale));
    if(pixels == nullptr) {
      glTexImage2D(target, level, internalformat, out_width, out_height, border, format, type, pixels);
      return;
    }
    int channel = 1;
    switch (internalformat) {
      case GL_RGBA:
        channel = 4;
        break;
      case GL_RGB:
        channel = 3;
        break;
      default:
        break;
    }
    unsigned char *image_data = new unsigned char[out_width * out_height * channel];
    memset(image_data, 0, out_width * out_height * channel);
    if (image_scale) {
      // scale buffer data
      stbir_resize_uint8(pixels, width, height, 0, image_data, out_width, out_height, 0, channel);
    } else {
      unsigned char *from = pixels;
      unsigned char *to = image_data;
      for (int i = 0; i < out_height; i++) {
        memcpy(to, from, out_width * channel);
        to += out_width * channel;
        from += width * channel;
      }
    }
    glTexImage2D(target, level, internalformat, out_width, out_height, border, format, type, image_data);
    SAFE_DELETE_ARRAY(image_data)
  }
}

/// 默认顶点坐标
static float DEFAULT_VERTEX_COORDINATE[] = {
    -1.F, -1.F,
    1.F, -1.F,
    -1.F, 1.F,
    1.F, 1.F
};

/// 默认纹理坐标
static float DEFAULT_TEXTURE_COORDINATE[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
};

/// 纹理旋转90度坐标
static float TEXTURE_COORDINATE_ROTATED_90[8] = {
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 1.0f
};

/// 纹理旋转180度坐标
static float TEXTURE_COORDINATE_ROTATED_180[8] = {
    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};

/// 纹理旋转270度坐标
static float TEXTURE_COORDINATE_ROTATED_270[8] = {
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 0.0f
};

/// 纹理竖直方向FLIP坐标
static float TEXTURE_COORDINATE_VERTICAL_FLIP[8] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
};

/// 纹理水平方向FLIP坐标
static float TEXTURE_COORDINATE_HORIZONTAL_FLIP[8] = {
    1.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
};

/// 默认带matrix的顶点shader
static const char* DEFAULT_VERTEX_MATRIX_SHADER =
    "#ifdef GL_ES                                                                           \n"
    "precision highp float;                                                                 \n"
    "#else                                                                                  \n"
    "#define highp                                                                          \n"
    "#endif                                                                                 \n"
    "attribute vec4 position;                                                               \n"
    "attribute vec4 inputTextureCoordinate;                                                 \n"
    "varying vec2 textureCoordinate;                                                        \n"
    "uniform highp mat4 textureMatrix;                                                      \n"
    "void main() {                                                                          \n"
    "  textureCoordinate = (textureMatrix * inputTextureCoordinate).xy;                     \n"
    "  gl_Position = position;                                                              \n"
    "}                                                                                      \n";

static const char* DEFAULT_VERTEX_MVP_SHADER =
    "#ifdef GL_ES                                                                           \n"
    "precision highp float;                                                                 \n"
    "#else                                                                                  \n"
    "#define highp                                                                          \n"
    "#endif                                                                                 \n"
    "attribute vec4 position;                                                               \n"
    "attribute vec4 inputTextureCoordinate;                                                 \n"
    "varying vec2 textureCoordinate;                                                        \n"
    "uniform highp mat4 textureMatrix;                                                      \n"
    "void main() {                                                                          \n"
    "  textureCoordinate = inputTextureCoordinate.xy;                                       \n"
    "  gl_Position = textureMatrix * position;                                              \n"
    "}                                                                                      \n";

/// 默认OES fragment shader
static const char* DEFAULT_OES_FRAGMENT_SHADER =
    "#extension GL_OES_EGL_image_external : require                                         \n"
    "precision mediump float;                                                               \n"
    "uniform samplerExternalOES inputImageTexture;                                          \n"
    "varying vec2 textureCoordinate;                                                        \n"
    "void main() {                                                                          \n"
    "  gl_FragColor = texture2D(inputImageTexture, textureCoordinate);                      \n"
    "}                                                                                      \n";

/// 默认顶点shader
static const char* DEFAULT_VERTEX_SHADER =
    "#ifdef GL_ES                                                                           \n"
    "precision highp float;                                                                 \n"
    "#endif                                                                                 \n"
    "attribute vec4 position;                                                               \n"
    "attribute vec4 inputTextureCoordinate;                                                 \n"
    "varying vec2 textureCoordinate;                                                        \n"
    "void main() {                                                                          \n"
    "    gl_Position = position;                                                            \n"
    "    textureCoordinate = inputTextureCoordinate.xy;                                     \n"
    "}                                                                                      \n";

/// 默认fragment shader
static const char* DEFAULT_FRAGMENT_SHADER =
    "#ifdef GL_ES                                                                           \n"
    "precision highp float;                                                                 \n"
    "#endif                                                                                 \n"
    "varying vec2 textureCoordinate;                                                        \n"
    "uniform sampler2D inputImageTexture;                                                   \n"
    "void main() {                                                                          \n"
    "    gl_FragColor = texture2D(inputImageTexture, textureCoordinate);                    \n"
    "}                                                                                      \n";


#endif  // OPENGL_GL_H_