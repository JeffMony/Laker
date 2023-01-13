
#include "color_adjust.h"

#include "common.h"
#include "tools.h"
#include "OpenGL/gl.h"

namespace effect {

/// 亮度的shader
const auto COLOR_BRIGHTNESS_FRAGMENT_SHADER = STRINGIZE(
    varying highp vec2 textureCoordinate;

    uniform sampler2D inputImageTexture;
    uniform lowp float brightness;

    void main() {
      lowp vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);
      gl_FragColor = vec4((textureColor.rgb + vec3(brightness)), textureColor.a);
    }
);

/// 对比度的shader
const auto COLOR_CONTRAST_FRAGMENT_SHADER = STRINGIZE(
    varying highp vec2 textureCoordinate;

    uniform sampler2D inputImageTexture;
    uniform lowp float contrast;

    void main() {
      lowp vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);
      gl_FragColor = vec4(((textureColor.rgb - vec3(0.5)) * contrast + vec3(0.5)), textureColor.a);
    }
);

/// 色温的shader
const auto COLOR_TEMPERATURE_FRAGMENT_SHADER = STRINGIZE(
    uniform sampler2D inputImageTexture;
    varying highp vec2 textureCoordinate;

    uniform lowp float temperature;
    uniform lowp float tint;

    const lowp vec3 warmFilter = vec3(0.93, 0.54, 0.0);

    const mediump mat3 RGBtoYIQ = mat3(0.299, 0.587, 0.114, 0.596, -0.274, -0.322, 0.212, -0.523, 0.311);
    const mediump mat3 YIQtoRGB = mat3(1.0, 0.956, 0.621, 1.0, -0.272, -0.647, 1.0, -1.105, 1.702);

    void main() {
      lowp vec4 source = texture2D(inputImageTexture, textureCoordinate);
      mediump vec3 yiq = RGBtoYIQ * source.rgb;
      yiq.b = clamp(yiq.b + tint*0.5226*0.1, -0.5226, 0.5226);
      lowp vec3 rgb = YIQtoRGB * yiq;

      lowp vec3 processed = vec3(
          (rgb.r < 0.5 ? (2.0 * rgb.r * warmFilter.r) : (1.0 - 2.0 * (1.0 - rgb.r) * (1.0 - warmFilter.r))),
          (rgb.g < 0.5 ? (2.0 * rgb.g * warmFilter.g) : (1.0 - 2.0 * (1.0 - rgb.g) * (1.0 - warmFilter.g))),
          (rgb.b < 0.5 ? (2.0 * rgb.b * warmFilter.b) : (1.0 - 2.0 * (1.0 - rgb.b) * (1.0 - warmFilter.b)))
      );

      gl_FragColor = vec4(mix(rgb, processed, temperature), source.a);
    }
);

/// 饱和度的shader
const auto COLOR_SATURATION_FRAGMENT_SHADER = STRINGIZE(
    varying highp vec2 textureCoordinate;

    uniform sampler2D inputImageTexture;
    uniform lowp float saturation;

    const mediump vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);

    void main() {
      lowp vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);
      lowp float luminance = dot(textureColor.rgb, luminanceWeighting);
      lowp vec3 greyScaleColor = vec3(luminance);
      gl_FragColor = vec4(mix(greyScaleColor, textureColor.rgb, saturation), textureColor.a);
    }
);

/// 锐度的顶点shader
const auto COLOR_SHARPNESS_VERTEX_SHADER = STRINGIZE(
    attribute vec4 position;
    attribute vec4 inputTextureCoordinate;

    uniform float imageWidthFactor;
    uniform float imageHeightFactor;
    uniform float sharpness;

    varying vec2 textureCoordinate;
    varying vec2 leftTextureCoordinate;
    varying vec2 rightTextureCoordinate;
    varying vec2 topTextureCoordinate;
    varying vec2 bottomTextureCoordinate;

    varying float centerMultiplier;
    varying float edgeMultiplier;

    void main() {
      gl_Position = position;

      mediump vec2 widthStep = vec2(imageWidthFactor, 0.0);
      mediump vec2 heightStep = vec2(0.0, imageHeightFactor);

      textureCoordinate = inputTextureCoordinate.xy;
      leftTextureCoordinate = inputTextureCoordinate.xy - widthStep;
      rightTextureCoordinate = inputTextureCoordinate.xy + widthStep;
      topTextureCoordinate = inputTextureCoordinate.xy + heightStep;
      bottomTextureCoordinate = inputTextureCoordinate.xy - heightStep;

      centerMultiplier = 1.0 + 4.0 * sharpness;
      edgeMultiplier = sharpness;
    }
);

/// 锐度的片段shader
const auto COLOR_SHARPNESS_FRAGMENT_SHADER = STRINGIZE(
    precision highp float;

    varying highp vec2 textureCoordinate;
    varying highp vec2 leftTextureCoordinate;
    varying highp vec2 rightTextureCoordinate;
    varying highp vec2 topTextureCoordinate;
    varying highp vec2 bottomTextureCoordinate;

    varying highp float centerMultiplier;
    varying highp float edgeMultiplier;

    uniform sampler2D inputImageTexture;

    void main() {
      mediump vec3 textureColor = texture2D(inputImageTexture, textureCoordinate).rgb;
      mediump vec3 leftTextureColor = texture2D(inputImageTexture, leftTextureCoordinate).rgb;
      mediump vec3 rightTextureColor = texture2D(inputImageTexture, rightTextureCoordinate).rgb;
      mediump vec3 topTextureColor = texture2D(inputImageTexture, topTextureCoordinate).rgb;
      mediump vec3 bottomTextureColor = texture2D(inputImageTexture, bottomTextureCoordinate).rgb;

      gl_FragColor = vec4((textureColor * centerMultiplier - (leftTextureColor * edgeMultiplier +
          rightTextureColor * edgeMultiplier + topTextureColor * edgeMultiplier +
          bottomTextureColor * edgeMultiplier)), texture2D(inputImageTexture, bottomTextureCoordinate).a);
    }
);

/// 汇总的颜色处理
const auto NORMAL_COLOR_FRAGMENT_SHADER = STRINGIZE(
    varying highp vec2 textureCoordinate;
    uniform sampler2D inputImageTexture;

    uniform bool adjust_bright;
    uniform bool adjust_contrast;
    uniform bool adjust_temperature;
    uniform bool adjust_saturation;
    uniform bool adjust_grain;

    /// 亮度指标
    uniform lowp float brightness;

    /// 对比度指标
    uniform lowp float contrast;

    /// 调节色温的指标
    uniform lowp float temperature;
    uniform lowp float tint;

    const lowp vec3 warmFilter = vec3(0.93, 0.54, 0.0);

    const mediump mat3 RGBtoYIQ = mat3(0.299, 0.587, 0.114, 0.596, -0.274, -0.322, 0.212, -0.523, 0.311);
    const mediump mat3 YIQtoRGB = mat3(1.0, 0.956, 0.621, 1.0, -0.272, -0.647, 1.0, -1.105, 1.702);

    /// 调节饱和度的指标
    uniform lowp float saturation;
    const mediump vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);

    /// 调节胶片颗粒感
    uniform lowp float grain;

    /// 调节亮度的函数
    vec4 apply_adjust_brightness(vec4 input_color) {
      return vec4((input_color.rgb + vec3(brightness)), input_color.a);
    }

    /// 调节对比度的函数
    vec4 apply_adjust_contrast(vec4 input_color) {
      return vec4(((input_color.rgb - vec3(0.5)) * contrast + vec3(0.5)), input_color.a);
    }

    /// 调节色温的函数
    vec4 apply_adjust_temperature(vec4 input_color) {
      mediump vec3 yiq = RGBtoYIQ * input_color.rgb;
      yiq.b = clamp(yiq.b + tint * 0.5226 * 0.1, -0.5226, 0.5226);
      lowp vec3 rgb = YIQtoRGB * yiq;

      lowp vec3 processed = vec3(
          (rgb.r < 0.5 ? (2.0 * rgb.r * warmFilter.r) : (1.0 - 2.0 * (1.0 - rgb.r) * (1.0 - warmFilter.r))),
          (rgb.g < 0.5 ? (2.0 * rgb.g * warmFilter.g) : (1.0 - 2.0 * (1.0 - rgb.g) * (1.0 - warmFilter.g))),
          (rgb.b < 0.5 ? (2.0 * rgb.b * warmFilter.b) : (1.0 - 2.0 * (1.0 - rgb.b) * (1.0 - warmFilter.b)))
      );

      return vec4(mix(rgb, processed, temperature), input_color.a);
    }

    /// 调节饱和度
    vec4 apply_adjust_saturation(vec4 input_color) {
      lowp float luminance = dot(input_color.rgb, luminanceWeighting);
      lowp vec3 greyScaleColor = vec3(luminance);
      return vec4(mix(greyScaleColor, input_color.rgb, saturation), input_color.a);
    }

    /// 调节胶片颗粒感
    vec4 apply_film_grain(vec4 input_color) {
      float noise = (fract(sin(dot(textureCoordinate, vec2(12.9898, 78.233) * 2.0)) * 43758.5453));
      return (input_color - noise * grain);
    }

    void main() {
      lowp vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);
      vec4 result_color = textureColor;
      if (adjust_bright) {
        result_color = apply_adjust_brightness(result_color);
      }
      if (adjust_contrast) {
        result_color = apply_adjust_contrast(result_color);
      }
      if (adjust_temperature) {
        result_color = apply_adjust_temperature(result_color);
      }
      if (adjust_saturation) {
        result_color = apply_adjust_saturation(result_color);
      }
      if (adjust_grain) {
        result_color = apply_film_grain(result_color);
      }
      gl_FragColor = result_color;
    }
);

ColorAdjustBuffer::ColorAdjustBuffer(int width, int height, const char *vertex_shader, const char *fragment_shader)
    : FrameBuffer(width, height, vertex_shader, fragment_shader)
    , width_(width)
    , height_(height)
    , brightness_level_(0.0f)
    , contrast_level_(0.0f)
    , temperature_level_(0.0f)
    , saturation_level_(0.0f)
    , grain_level_(0.0f)
    , sharpness_level_(0.0f)
    , method_bit_(0)
    , adjust_bright_(false)
    , adjust_contrast_(false)
    , adjust_temperature_(false)
    , adjust_saturation_(false)
    , adjust_grain_(false) {

}

ColorAdjustBuffer::~ColorAdjustBuffer() {}

/**
 * 设置色彩调整的level
 * brightness -1.0f ~ 1.0f
 * contrast 0.0f ~ 2.0f
 * temperature 2000.0f ~ 8000.0f
 * saturation 0.0f ~ 2.0f
 * grain 0.0f ~ 0.5f
 * sharpness -4.0f ~ 4.0f
 */

void ColorAdjustBuffer::SetBrightnessLevel(float brightness_level) {
  brightness_level_ = brightness_level / 100.0f;
}

void ColorAdjustBuffer::SetContrastLevel(float contrast_level) {
  contrast_level_ = contrast_level / 100.0f + 1.0f;
}

void ColorAdjustBuffer::SetTemperatureLevel(float temperature_level) {
  float result = temperature_level / 100.0f * 3000.0f + 5000.0f;
  result = result < 5000 ? (float) (0.0004 * (result - 5000.0)) : (float) (0.00006 * (result - 5000.0));
  temperature_level_ = result;
}

void ColorAdjustBuffer::SetSaturationLevel(float saturation_level) {
  saturation_level_ = saturation_level / 100.0f + 1.0f;
}

void ColorAdjustBuffer::SetGrainLevel(float grain_level) {
  grain_level_ = grain_level / 100 * 0.5f;
}

void ColorAdjustBuffer::SetSharpnessLevel(float sharpness_level) {
  sharpness_level_ = sharpness_level / 100.0f * 4.0f;
}

void ColorAdjustBuffer::SetMethodBit(int method_bit) {
  method_bit_ = method_bit;
  int result = method_bit & 0b1;
  if (result != 0) {
    adjust_bright_ = true;
  }
  result = method_bit & 0b10;
  if (result != 0) {
    adjust_contrast_ = true;
  }
  result = method_bit & 0b100;
  if (result != 0) {
    adjust_temperature_ = true;
  }
  result = method_bit & 0b1000;
  if (result != 0) {
    adjust_saturation_ = true;
  }
  result = method_bit & 0b10000;
  if (result != 0) {
    adjust_grain_ = true;
  }
}

void ColorAdjustBuffer::RunOnDrawTasks() {
  int result = method_bit_ & 0b11111;
  int result2 = method_bit_ & 0b100000;
  if (result != 0) {
    SetInt("adjust_bright", adjust_bright_);
    SetInt("adjust_contrast", adjust_contrast_);
    SetInt("adjust_temperature", adjust_temperature_);
    SetInt("adjust_saturation", adjust_saturation_);
    SetInt("adjust_grain", adjust_grain_);

    SetFloat("brightness", brightness_level_);
    SetFloat("contrast", contrast_level_);
    SetFloat("temperature", temperature_level_);
    SetFloat("tint", 0.0f);
    SetFloat("saturation", saturation_level_);
    SetFloat("grain", grain_level_);
  }
  if (result2 != 0) {
    SetFloat("sharpness", sharpness_level_);
    SetFloat("imageWidthFactor", 1.0f / width_);
    SetFloat("imageHeightFactor", 1.0f / height_);
  }
}

/// 下面是ColorAdjust主类
ColorAdjust::ColorAdjust()
    : brightness_level_(0.0f)
    , contrast_level_(0.0f)
    , temperature_level_(0.0f)
    , saturation_level_(0.0f)
    , grain_level_(0.0f)
    , sharpness_level_(0.0f)
    , method_bit_(0)
    , bg_image_texture_id_(0)
    , bg_image_width_(0)
    , bg_image_height_(0)
    , normal_color_adjust_buffer_(nullptr)
    , sharp_color_adjust_buffer_(nullptr) {

}

ColorAdjust::~ColorAdjust() {
  if (bg_image_texture_id_ != 0) {
    glDeleteTextures(1, &bg_image_texture_id_);
    bg_image_texture_id_ = 0;
  }
  SAFE_DELETE(normal_color_adjust_buffer_)
  SAFE_DELETE(sharp_color_adjust_buffer_)
}

void ColorAdjust::SetBrightnessLevel(float brightness_level) {
  brightness_level_ = brightness_level;
}

void ColorAdjust::SetContrastLevel(float contrast_level) {
  contrast_level_ = contrast_level;
}

void ColorAdjust::SetTemperatureLevel(float temperature_level) {
  temperature_level_ = temperature_level;
}

void ColorAdjust::SetSaturationLevel(float saturation_level) {
  saturation_level_ = saturation_level;
}

void ColorAdjust::SetGrainLevel(float grain_level) {
  grain_level_ = grain_level;
}

void ColorAdjust::SetSharpnessLevel(float sharpness_level) {
  sharpness_level_ = sharpness_level;
}

void ColorAdjust::SetMethodBit(int method_bit) {
  method_bit_ = method_bit;
}

void ColorAdjust::SetBackgroundImage(unsigned char *buffer, int width, int height) {
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

int ColorAdjust::GetWidth() {
  return bg_image_width_;
}

int ColorAdjust::GetHeight() {
  return bg_image_height_;
}

GLuint ColorAdjust::ProcessImage(GLuint texture_id, int width, int height) {
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

  /**
   * 0b1表示调节亮度
   * 0b10表示调节对比度
   * 0b100表示调节色温
   * 0b1000表示调节饱和度
   * 0b10000表示调节胶片颗粒度
   * 0b100000表示调节锐度
   *
   * 0b101001
   */
  int result = method_bit_ & 0b11111;
  if (result != 0) {
    if (normal_color_adjust_buffer_ == nullptr) {
      normal_color_adjust_buffer_ = new ColorAdjustBuffer(width, height, DEFAULT_VERTEX_SHADER,
                                                          (media::shader_header() + NORMAL_COLOR_FRAGMENT_SHADER).c_str());
    }
    normal_color_adjust_buffer_->SetBrightnessLevel(brightness_level_);
    normal_color_adjust_buffer_->SetContrastLevel(contrast_level_);
    normal_color_adjust_buffer_->SetTemperatureLevel(temperature_level_);
    normal_color_adjust_buffer_->SetSaturationLevel(saturation_level_);
    normal_color_adjust_buffer_->SetGrainLevel(grain_level_);
    normal_color_adjust_buffer_->SetMethodBit(method_bit_);
  }

  result = method_bit_ & 0b100000;
  if (result != 0) {
    if (sharp_color_adjust_buffer_ == nullptr) {
      sharp_color_adjust_buffer_ = new ColorAdjustBuffer(width, height, (media::shader_header() + COLOR_SHARPNESS_VERTEX_SHADER).c_str(),
                                                         (media::shader_header() + COLOR_SHARPNESS_FRAGMENT_SHADER).c_str());
    }
    sharp_color_adjust_buffer_->SetSharpnessLevel(sharpness_level_);
    sharp_color_adjust_buffer_->SetMethodBit(method_bit_);
  }

  GLuint result_texture_id = bg_image_texture_id;
  if (normal_color_adjust_buffer_ != nullptr) {
    result_texture_id = normal_color_adjust_buffer_->ProcessImage(bg_image_texture_id, width, height);
  }

  if (sharp_color_adjust_buffer_ != nullptr) {
    result_texture_id = sharp_color_adjust_buffer_->ProcessImage(result_texture_id, width, height);
  }
  return result_texture_id;
}

}  // namespace effect