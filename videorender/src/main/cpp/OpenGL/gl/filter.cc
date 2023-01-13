
#include "filter.h"
#include "OpenGL/gl.h"

static const char* FILTER_FRAGMENT_SHADER =
    "#ifdef GL_ES                                                                                \n"
    "precision highp float;                                                                      \n"
    "#else                                                                                       \n"
    "#define highp                                                                               \n"
    "#define mediump                                                                             \n"
    "#define lowp                                                                                \n"
    "#endif                                                                                      \n"
    "varying highp vec2 textureCoordinate;                                                       \n"
    "uniform sampler2D inputImageTexture;                                                        \n"
    "uniform sampler2D inputImageTextureLookup;                                                  \n"
    "uniform float intensity;                                                                    \n"
    "void main () {                                                                              \n"
    " vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);                       \n"
    " if (textureCoordinate.y < 1.0) {                                                           \n"
    "     float yColor = textureColor.b * 63.0;                                                  \n"
    "     vec2 quad1;                                                                            \n"
    "     quad1.y = floor(floor(yColor) / 8.0);                                                  \n"
    "     quad1.x = floor(yColor) - (quad1.y * 8.0);                                             \n"
    "     vec2 quad2;                                                                            \n"
    "     quad2.y = floor(ceil(yColor) / 8.0);                                                   \n"
    "     quad2.x = ceil(yColor) - (quad2.y * 8.0);                                              \n"
    "     vec2 texPos1;                                                                          \n"
    "     texPos1.x = (quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);    \n"
    "     texPos1.y = (quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);    \n"
    "     vec2 texPos2;                                                                          \n"
    "     texPos2.x = (quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);    \n"
    "     texPos2.y = (quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);    \n"
    "     vec4 newColor1;                                                                        \n"
    "     vec4 newColor2;                                                                        \n"
    "     newColor1 = texture2D(inputImageTextureLookup, texPos1);                               \n"
    "     newColor2 = texture2D(inputImageTextureLookup, texPos2);                               \n"
    "     vec4 newColor = mix(newColor1, newColor2, fract(yColor));                              \n"
    "     gl_FragColor = mix(textureColor, vec4(newColor.rgb, textureColor.a), intensity);       \n"
    " } else {                                                                                   \n"
    "     gl_FragColor = textureColor;                                                           \n"
    " }                                                                                          \n"
    "}\n";

static const char* FILTER64_FRAGMENT_SHADER =
    "#ifdef GL_ES                                                                                \n"
    "precision highp float;                                                                      \n"
    "#else                                                                                       \n"
    "#define highp                                                                               \n"
    "#define mediump                                                                             \n"
    "#define lowp                                                                                \n"
    "#endif                                                                                      \n"
    "varying highp vec2 textureCoordinate;                                                       \n"
    "uniform sampler2D inputImageTexture;                                                        \n"
    "uniform sampler2D inputImageTextureLookup;                                                  \n"
    "uniform float intensity;                                                                    \n"
    "void main () {                                                                              \n"
    " vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);                       \n"
    " if (textureCoordinate.y < 1.0) {                                                           \n"
    "     float yColor = textureColor.b * 15.0;                                                  \n"
    "     vec2 quad1;                                                                            \n"
    "     quad1.y = floor(floor(yColor) / 4.0);                                                  \n"
    "     quad1.x = floor(yColor) - (quad1.y * 4.0);                                             \n"
    "     vec2 quad2;                                                                            \n"
    "     quad2.y = floor(ceil(yColor) / 4.0);                                                   \n"
    "     quad2.x = ceil(yColor) - (quad2.y * 4.0);                                              \n"
    "     vec2 texPos1;                                                                          \n"
    "     texPos1.x = (quad1.x * 0.125) + 0.5/64.0 + ((0.125 - 1.0/64.0) * textureColor.r);      \n"
    "     texPos1.y = (quad1.y * 0.125) + 0.5/64.0 + ((0.125 - 1.0/64.0) * textureColor.g);      \n"
    "     vec2 texPos2;                                                                          \n"
    "     texPos2.x = (quad2.x * 0.125) + 0.5/64.0 + ((0.125 - 1.0/64.0) * textureColor.r);      \n"
    "     texPos2.y = (quad2.y * 0.125) + 0.5/64.0 + ((0.125 - 1.0/64.0) * textureColor.g);      \n"
    "     vec4 newColor1;                                                                        \n"
    "     vec4 newColor2;                                                                        \n"
    "     newColor1 = texture2D(inputImageTextureLookup, texPos1);                               \n"
    "     newColor2 = texture2D(inputImageTextureLookup, texPos2);                               \n"
    "     vec4 newColor = mix(newColor1, newColor2, fract(yColor));                              \n"
    "     gl_FragColor = mix(textureColor, vec4(newColor.rgb, textureColor.a), intensity);       \n"
    " } else {                                                                                   \n"
    "     gl_FragColor = textureColor;                                                           \n"
    " }                                                                                          \n"
    "}\n";


static const char* FILTER_1024_FRAGMENT_SHADER =
    "#ifdef GL_ES                                                                                \n"
    "precision highp float;                                                                      \n"
    "#else                                                                                       \n"
    "#define highp                                                                               \n"
    "#define mediump                                                                             \n"
    "#define lowp                                                                                \n"
    "#endif                                                                                      \n"
    "varying highp vec2 textureCoordinate;                                                       \n"
    "uniform sampler2D inputImageTexture;                                                        \n"
    "uniform sampler2D inputImageTextureLookup;                                                  \n"
    "uniform float intensity;                                                                    \n"
    "void main () {                                                                              \n"
    " vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);                       \n"
    " if (textureCoordinate.y <= 1.0) {                                                          \n"
    "     float yColor = textureColor.b * 63.0;                                                  \n"
    "     vec2 quad1;                                                                            \n"
    "     quad1.y = floor(floor(yColor) / 8.0);                                                  \n"
    "     quad1.x = floor(yColor) - (quad1.y * 8.0);                                             \n"
    "     vec2 quad2;                                                                            \n"
    "     quad2.y = floor(ceil(yColor) / 8.0);                                                   \n"
    "     quad2.x = ceil(yColor) - (quad2.y * 8.0);                                              \n"
    "     vec2 texPos1;                                                                          \n"
    "     texPos1.x = (quad1.x * 0.125) + 0.5/1024.0 + ((0.125 - 1.0/1024.0) * textureColor.r);  \n"
    "     texPos1.y = (quad1.y * 0.125) + 0.5/1024.0 + ((0.125 - 1.0/1024.0) * textureColor.g);  \n"
    "     vec2 texPos2;                                                                          \n"
    "     texPos2.x = (quad2.x * 0.125) + 0.5/1024.0 + ((0.125 - 1.0/1024.0) * textureColor.r);  \n"
    "     texPos2.y = (quad2.y * 0.125) + 0.5/1024.0 + ((0.125 - 1.0/1024.0) * textureColor.g);  \n"
    "     vec4 newColor1;                                                                        \n"
    "     vec4 newColor2;                                                                        \n"
    "     newColor1 = texture2D(inputImageTextureLookup, texPos1);                               \n"
    "     newColor2 = texture2D(inputImageTextureLookup, texPos2);                               \n"
    "     vec4 newColor = mix(newColor1, newColor2, fract(yColor));                              \n"
    "     gl_FragColor = mix(textureColor, vec4(newColor.rgb, textureColor.a), intensity);       \n"
    " } else {                                                                                   \n"
    "     gl_FragColor = textureColor;                                                           \n"
    " }                                                                                          \n"
    "}\n";



namespace effect {

Filter::Filter(uint8_t* lut, int lut_width, int lut_height)
    : FrameBuffer(DEFAULT_VERTEX_SHADER,
                  (lut_width == 512 && lut_height == 512) ? FILTER_FRAGMENT_SHADER :
                  (lut_width == 1024 && lut_height == 1024) ? FILTER_1024_FRAGMENT_SHADER : FILTER64_FRAGMENT_SHADER)
    , texture_id_(0)
    , intensity_(100) {
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glSafeTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lut_width, lut_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, lut);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Filter::~Filter() {
  if (texture_id_ != 0) {
    glDeleteTextures(1, &texture_id_);
  }
  texture_id_ = 0;
}

void Filter::UpdateLut(uint8_t *lut, int lut_width, int lut_height) {
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glSafeTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lut_width, lut_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, lut);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Filter::SetIntensity(int intensity) {
  intensity_ = intensity;
}

void Filter::RunOnDrawTasks() {
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  SetInt("inputImageTextureLookup", 1);
  SetFloat("intensity", static_cast<float>(intensity_) / 100.f);
}

}  // namespace effect