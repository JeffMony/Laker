
#ifndef OPENGL_GL_BLEND_H_
#define OPENGL_GL_BLEND_H_

#include "frame_buffer.h"

namespace effect {

#define NORMAL_BLEND      0
#define ADD_BLEND         4
#define OVERLAY_BLEND     7
#define ADD2_BLEND        1001
#define COLOR_DODGE_BLEND 1004
#define HARD_LIGHT_BLEND  1009
#define MULTIPLY_BLEND    1015
#define OVERLAY2_BLEND    1017
#define PIN_LIGHT_BLEND   1019
#define SCREEN_BLEND      1021
#define SOFT_LIGHT_BLEND  1022

static const char* BLEND_VERTEX_SHADER =
        "attribute vec3 position;                                                               \n"
        "attribute vec2 inputTextureCoordinate;                                                 \n"
        "varying vec2 textureCoordinate;                                                        \n"
        "varying vec2 textureCoordinate2;                                                       \n"
        "uniform mat4 matrix;                                                                   \n"
        "void main() {                                                                          \n"
        "    vec4 p = matrix * vec4(position, 1.);                                              \n"
        "    #ifdef GL_ES                                                                       \n"
        "    textureCoordinate = vec2(inputTextureCoordinate.x, 1.0 - inputTextureCoordinate.y);\n"
        "    #else                                                                              \n"
        "    textureCoordinate = inputTextureCoordinate;                                        \n"
        "    #endif                                                                             \n"
        "    textureCoordinate2 = p.xy * 0.5 + 0.5;                                             \n"
        "    gl_Position = p;                                                                   \n"
        "}                                                                                      \n";

static const char* BLEND_FRAGMENT_SHADER =
        "#ifdef GL_ES                                                                           \n"
        "precision mediump float;                                                               \n"
        "varying highp vec2 textureCoordinate;                                                  \n"
        "varying highp vec2 textureCoordinate2;                                                 \n"
        "#else                                                                                  \n"
        "varying vec2 textureCoordinate;                                                        \n"
        "varying vec2 textureCoordinate2;                                                       \n"
        "#endif                                                                                 \n"
        "uniform sampler2D inputImageTexture;                                                   \n"
        "uniform sampler2D inputImageTexture2;                                                  \n"
        "uniform float alphaFactor;                                                             \n"
        "void main() {                                                                          \n"
        "    vec4 fgColor = texture2D(inputImageTexture2, textureCoordinate);                   \n"
        "    fgColor = fgColor * alphaFactor;                                                   \n"
        "    gl_FragColor = fgColor;                                                            \n"
        "}                                                                                      \n";

static const char* ADD_BLEND_FRAGMENT_SHADER =
        "#ifdef GL_ES                                                                           \n"
        "precision mediump float;                                                               \n"
        "varying highp vec2 textureCoordinate;                                                  \n"
        "varying highp vec2 textureCoordinate2;                                                 \n"
        "#else                                                                                  \n"
        "varying vec2 textureCoordinate;                                                        \n"
        "varying vec2 textureCoordinate2;                                                       \n"
        "#endif                                                                                 \n"
        "uniform sampler2D inputImageTexture;                                                   \n"
        "uniform sampler2D inputImageTexture2;                                                  \n"
        "uniform float alphaFactor;                                                             \n"
        "uniform float bgResolutionWidth;                                                       \n"
        "uniform float bgResolutionHeight;                                                      \n"
        "float blendAdd(float base, float blend) {                                              \n"
        "   return min(base + blend, 1.0);                                                      \n"
        "}                                                                                      \n"
        "vec3 blendAdd(vec3 base, vec3 blend) {                                                 \n"
        "   return min(base + blend, vec3(1.0));                                                \n"
        "}                                                                                      \n"
        "vec3 blendFunc(vec3 base, vec3 blend, float opacity) {                                 \n"
        "   return (blendAdd(base, blend) * opacity + base * (1.0 - opacity));                  \n"
        "}                                                                                      \n"
        "void main() {                                                                          \n"
        "   vec4 fgColor = texture2D(inputImageTexture2, textureCoordinate);                    \n"
        "   fgColor = fgColor * alphaFactor;                                                    \n"
        "   vec4 bgColor = texture2D(inputImageTexture, textureCoordinate2);                    \n"
        "   if (fgColor.a == 0.0) {                                                             \n"
        "       gl_FragColor = bgColor;                                                         \n"
        "       return;                                                                         \n"
        "   }                                                                                   \n"
        "   vec3 color = blendFunc(bgColor.rgb, clamp(fgColor.rgb * (1.0 / fgColor.a), 0.0, 1.0), 1.0 ); \n"
        "   gl_FragColor = vec4(bgColor.rgb * (1.0 - fgColor.a) + color.rgb * fgColor.a, 1.0);  \n"
        "}                                                                                      \n";

static const char* OVERLAY_BLEND_FRAGMENT_SHADER =
        "#ifdef GL_ES                                                                           \n"
        "precision mediump float;                                                               \n"
        "varying highp vec2 textureCoordinate;                                                  \n"
        "varying highp vec2 textureCoordinate2;                                                 \n"
        "#else                                                                                  \n"
        "varying vec2 textureCoordinate;                                                        \n"
        "varying vec2 textureCoordinate2;                                                       \n"
        "#endif                                                                                 \n"
        "uniform sampler2D inputImageTexture;                                                   \n"
        "uniform sampler2D inputImageTexture2;                                                  \n"
        "uniform float alphaFactor;                                                             \n"
        "float blendOverlay(float base, float blend) {                                          \n"
        "   return base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0-blend));\n"
        "}                                                                                      \n"
        "vec3 blendOverlay(vec3 base, vec3 blend) {                                             \n"
        "   return vec3(blendOverlay(base.r, blend.r), blendOverlay(base.g, blend.g), blendOverlay(base.b, blend.b));\n"
        "}                                                                                      \n"
        "vec3 blendFunc(vec3 base, vec3 blend, float opacity) {                                 \n"
        "   return (blendOverlay(base, blend) * opacity + base * (1.0 - opacity));              \n"
        "}                                                                                      \n"
        "void main() {                                                                          \n"
        "   vec4 fgColor = texture2D(inputImageTexture2, textureCoordinate);                    \n"
        "   fgColor = fgColor * alphaFactor;                                                    \n"
        "   vec4 bgColor = texture2D(inputImageTexture, textureCoordinate2);                    \n"
        "   if (fgColor.a == 0.0) {                                                             \n"
        "       gl_FragColor = bgColor;                                                         \n"
        "       return;                                                                         \n"
        "   }                                                                                   \n"
        "   vec3 color = blendFunc(bgColor.rgb, clamp(fgColor.rgb * (1.0 / fgColor.a), 0.0, 1.0), 1.0 );\n"
        "   gl_FragColor = vec4(bgColor.rgb * (1.0 - fgColor.a) + color.rgb * fgColor.a, 1.0);  \n"
        "}                                                                                      \n";

static const char* COLOR_DODGE_BLEND_FRAGMENT_SHADER =
        "#ifdef GL_ES                                                                           \n"
        "precision mediump float;                                                               \n"
        "varying highp vec2 textureCoordinate;                                                  \n"
        "varying highp vec2 textureCoordinate2;                                                 \n"
        "#else                                                                                  \n"
        "varying vec2 textureCoordinate;                                                        \n"
        "varying vec2 textureCoordinate2;                                                       \n"
        "#endif                                                                                 \n"
        "uniform sampler2D inputImageTexture;                                                   \n"
        "uniform sampler2D inputImageTexture2;                                                  \n"
        "uniform float alphaFactor;                                                             \n"
        "uniform float bgResolutionWidth;                                                       \n"
        "uniform float bgResolutionHeight;                                                      \n"
        "float blendColorDodge(float base, float blend) {                                       \n"
        "   return (blend == 1.0) ? blend : min(base / (1.0 - blend), 1.0);                     \n"
        "}                                                                                      \n"
        "vec3 blendColorDodge(vec3 base, vec3 blend) {                                          \n"
        "   return vec3(blendColorDodge(base.r,blend.r),blendColorDodge(base.g,blend.g),blendColorDodge(base.b,blend.b));\n" // NOLINT
        "}                                                                                      \n"
        "vec3 blendFunc(vec3 base, vec3 blend, float opacity) {                                 \n"
        "   return (blendColorDodge(base, blend) * opacity + base * (1.0 - opacity));           \n"
        "}                                                                                      \n"
        "void main() {                                                                          \n"
        "   vec4 fgColor = texture2D(inputImageTexture2, textureCoordinate);                    \n"
        "   fgColor = fgColor * alphaFactor;                                                    \n"
        "   vec4 bgColor = texture2D(inputImageTexture, textureCoordinate2);                    \n"
        "   if (fgColor.a == 0.0) {                                                             \n"
        "       gl_FragColor = bgColor;                                                         \n"
        "       return;                                                                         \n"
        "   }                                                                                   \n"
        "   vec3 color = blendFunc(bgColor.rgb, clamp(fgColor.rgb * (1.0 / fgColor.a), 0.0, 1.0), 1.0 );\n"
        "   gl_FragColor = vec4(bgColor.rgb * (1.0 - fgColor.a) + color.rgb * fgColor.a, 1.0);  \n"
        "}                                                                                      \n";

static const char* HARD_LIGHT_BLEND_FRAGMENT_SHADER =
        "#ifdef GL_ES                                                                           \n"
        "precision mediump float;                                                               \n"
        "varying highp vec2 textureCoordinate;                                                  \n"
        "varying highp vec2 textureCoordinate2;                                                 \n"
        "#else                                                                                  \n"
        "varying vec2 textureCoordinate;                                                        \n"
        "varying vec2 textureCoordinate2;                                                       \n"
        "#endif                                                                                 \n"
        "uniform sampler2D inputImageTexture;                                                   \n"
        "uniform sampler2D inputImageTexture2;                                                  \n"
        "uniform float alphaFactor;                                                             \n"
        "float blendHardLight(float base, float blend) {                                        \n"
        "   return blend < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend));\n"
        "}                                                                                      \n"
        "vec3 blendHardLight(vec3 base, vec3 blend) {                                           \n"
        "   return vec3(blendHardLight(base.r,blend.r),blendHardLight(base.g,blend.g),blendHardLight(base.b,blend.b));\n" // NOLINT
        "}                                                                                      \n"
        "vec3 blendFunc(vec3 base, vec3 blend, float opacity) {                                 \n"
        "   return (blendHardLight(base, blend) * opacity + base * (1.0 - opacity));            \n"
        "}                                                                                      \n"
        "void main() {                                                                          \n"
        "   vec4 fgColor = texture2D(inputImageTexture2, textureCoordinate);                    \n"
        "   fgColor = fgColor * alphaFactor;                                                    \n"
        "   vec4 bgColor = texture2D(inputImageTexture, textureCoordinate2);                    \n"
        "   if (fgColor.a == 0.0) {                                                             \n"
        "       gl_FragColor = bgColor;                                                         \n"
        "       return;                                                                         \n"
        "   }                                                                                   \n"
        "   vec3 color = blendFunc(bgColor.rgb, clamp(fgColor.rgb * (1.0 / fgColor.a), 0.0, 1.0), 1.0 );\n"
        "   gl_FragColor = vec4(bgColor.rgb * (1.0 - fgColor.a) + color.rgb * fgColor.a, 1.0);  \n"
        "}                                                                                      \n";

static const char* MULTIPLY_BLEND_FRAGMENT_SHADER =
        "#ifdef GL_ES                                                                           \n"
        "precision mediump float;                                                               \n"
        "varying highp vec2 textureCoordinate;                                                  \n"
        "varying highp vec2 textureCoordinate2;                                                 \n"
        "#else                                                                                  \n"
        "varying vec2 textureCoordinate;                                                        \n"
        "varying vec2 textureCoordinate2;                                                       \n"
        "#endif                                                                                 \n"
        "uniform sampler2D inputImageTexture;                                                   \n"
        "uniform sampler2D inputImageTexture2;                                                  \n"
        "uniform float alphaFactor;                                                             \n"
        "vec3 blendMultiply(vec3 base, vec3 blend) {                                            \n"
        "   return base * blend;                                                                \n"
        "}                                                                                      \n"
        "vec3 blendFunc(vec3 base, vec3 blend, float opacity) {                                 \n"
        "   return (blendMultiply(base, blend) * opacity + base * (1.0 - opacity));             \n"
        "}                                                                                      \n"  // NOLINT
        "void main() {                                                                          \n"
        "   vec4 fgColor = texture2D(inputImageTexture2, textureCoordinate);                    \n"
        "   fgColor = fgColor * alphaFactor;                                                    \n"
        "   vec4 bgColor = texture2D(inputImageTexture, textureCoordinate2);                    \n"
        "   if (fgColor.a == 0.0) {                                                             \n"
        "       gl_FragColor = bgColor;                                                         \n"
        "       return;                                                                         \n"
        "   }                                                                                   \n"
        "   vec3 color = blendFunc(bgColor.rgb, clamp(fgColor.rgb * (1.0 / fgColor.a), 0.0, 1.0), 1.0 );\n" // NOLINT
        "   gl_FragColor = vec4(bgColor.rgb * (1.0 - fgColor.a) + color.rgb * fgColor.a, 1.0);  \n"
        "}                                                                                      \n";

static const char* PIN_LIGHT_BLEND_FRAGMENT_SHADER =
        "#ifdef GL_ES                                                                           \n"
        "precision mediump float;                                                               \n"
        "varying highp vec2 textureCoordinate;                                                  \n"
        "varying highp vec2 textureCoordinate2;                                                 \n"
        "#else                                                                                  \n"
        "varying vec2 textureCoordinate;                                                        \n"
        "varying vec2 textureCoordinate2;                                                       \n"
        "#endif                                                                                 \n"
        "uniform sampler2D inputImageTexture;                                                   \n"
        "uniform sampler2D inputImageTexture2;                                                  \n"
        "uniform float alphaFactor;                                                             \n"
        "float blendDarken(float base, float blend) {                                           \n"
        "   return min(blend, base);                                                            \n"
        "}                                                                                      \n"
        "float blendLighten(float base, float blend) {                                          \n"
        "   return max(blend, base);                                                            \n"
        "}                                                                                      \n" // NOLINT
        "float blendPinLight(float base, float blend) {                                         \n"
        "   return (blend<0.5)?blendDarken(base,(2.0*blend)):blendLighten(base,(2.0*(blend-0.5)));\n"
        "}                                                                                      \n"
        "vec3 blendPinLight(vec3 base, vec3 blend) {                                            \n"
        "   return vec3(blendPinLight(base.r,blend.r),blendPinLight(base.g,blend.g),blendPinLight(base.b,blend.b));\n"
        "}                                                                                      \n"
        "vec3 blendFunc(vec3 base, vec3 blend, float opacity) {                                 \n"
        "   return (blendPinLight(base, blend) * opacity + base * (1.0 - opacity));             \n"
        "}                                                                                      \n"
        "void main() {                                                                          \n"
        "   vec4 fgColor = texture2D(inputImageTexture2, textureCoordinate);                    \n"
        "   fgColor = fgColor * alphaFactor;                                                    \n"
        "   vec4 bgColor = texture2D(inputImageTexture, textureCoordinate2);                    \n"
        "   if (fgColor.a == 0.0) {                                                             \n"
        "       gl_FragColor = bgColor;                                                         \n"
        "       return;                                                                         \n"
        "   }                                                                                   \n"
        "   vec3 color = blendFunc(bgColor.rgb, clamp(fgColor.rgb * (1.0 / fgColor.a), 0.0, 1.0), 1.0 );\n"
        "   gl_FragColor = vec4(bgColor.rgb * (1.0 - fgColor.a) + color.rgb * fgColor.a, 1.0);  \n"
        "}                                                                                      \n";

static const char* SCREEN_BLEND_FRAGMENT_SHADER =
        "#ifdef GL_ES                                                                           \n"
        "precision mediump float;                                                               \n"
        "varying highp vec2 textureCoordinate;                                                  \n"
        "varying highp vec2 textureCoordinate2;                                                 \n"
        "#else                                                                                  \n"
        "varying vec2 textureCoordinate;                                                        \n"
        "varying vec2 textureCoordinate2;                                                       \n"
        "#endif                                                                                 \n"
        "uniform sampler2D inputImageTexture;                                                   \n"
        "uniform sampler2D inputImageTexture2;                                                  \n"
        "uniform float alphaFactor;                                                             \n"
        "float blendScreen(float base, float blend) {                                           \n"
        "   return 1.0 - ((1.0 - base) * (1.0 - blend));                                        \n"
        "}                                                                                      \n"
        "vec3 blendScreen(vec3 base, vec3 blend) {                                              \n"
        "   return vec3(blendScreen(base.r,blend.r),blendScreen(base.g,blend.g),blendScreen(base.b,blend.b));\n" // NOLINT
        "}                                                                                      \n"
        "vec3 blendFunc(vec3 base, vec3 blend, float opacity) {                                 \n"
        "   return (blendScreen(base, blend) * opacity + base * (1.0 - opacity));               \n"
        "}                                                                                      \n" // NOLINT
        "void main() {                                                                          \n"
        "   vec4 fgColor = texture2D(inputImageTexture2, textureCoordinate);                    \n"
        "   fgColor = fgColor * alphaFactor;                                                    \n"
        "   vec4 bgColor = texture2D(inputImageTexture, textureCoordinate2);                    \n"
        "   if (fgColor.a == 0.0) {                                                             \n"
        "       gl_FragColor = bgColor;                                                         \n"
        "       return;                                                                         \n"
        "   }                                                                                   \n"
        "   vec3 color = blendFunc(bgColor.rgb, clamp(fgColor.rgb * (1.0 / fgColor.a), 0.0, 1.0), 1.0 );\n"
        "   gl_FragColor = vec4(bgColor.rgb * (1.0 - fgColor.a) + color.rgb * fgColor.a, 1.0);  \n"
        "}                                                                                      \n";

static const char* SOFT_LIGHT_BLEND_FRAGMENT_SHADER =
        "#ifdef GL_ES                                                                           \n"
        "precision mediump float;                                                               \n"
        "varying highp vec2 textureCoordinate;                                                  \n"
        "varying highp vec2 textureCoordinate2;                                                 \n"
        "#else                                                                                  \n"
        "varying vec2 textureCoordinate;                                                        \n"
        "varying vec2 textureCoordinate2;                                                       \n"
        "#endif                                                                                 \n"
        "uniform sampler2D inputImageTexture;                                                   \n"
        "uniform sampler2D inputImageTexture2;                                                  \n"
        "uniform float alphaFactor;                                                             \n"
        "float blendSoftLight(float base, float blend) {                                        \n"
        "   return (blend<0.5)?(base+(2.0*blend-1.0)*(base-base*base)):(base+(2.0*blend-1.0)*(sqrt(base)-base));\n" // NOLINT
        "}                                                                                      \n"
        "vec3 blendSoftLight(vec3 base, vec3 blend) {                                           \n"
        "   return vec3(blendSoftLight(base.r,blend.r),blendSoftLight(base.g,blend.g),blendSoftLight(base.b,blend.b));\n" // NOLINT
        "}                                                                                      \n"
        "vec3 blendFunc(vec3 base, vec3 blend, float opacity) {                                 \n"
        "   return (blendSoftLight(base, blend) * opacity + base * (1.0 - opacity));            \n"
        "}                                                                                      \n" // NOLINT
        "void main() {                                                                          \n"
        "   vec4 fgColor = texture2D(inputImageTexture2, textureCoordinate);                    \n"
        "   fgColor = fgColor * alphaFactor;                                                    \n"
        "   vec4 bgColor = texture2D(inputImageTexture, textureCoordinate2);                    \n"
        "   if (fgColor.a == 0.0) {                                                             \n"
        "       gl_FragColor = bgColor;                                                         \n"
        "       return;                                                                         \n"
        "   }                                                                                   \n"
        "   vec3 color = blendFunc(bgColor.rgb, clamp(fgColor.rgb * (1.0 / fgColor.a), 0.0, 1.0), 1.0 );\n"
        "   gl_FragColor = vec4(bgColor.rgb * (1.0 - fgColor.a) + color.rgb * fgColor.a, 1.0);  \n"
        "}                                                                                      \n";

static const char* BLEND_SHADER =
        "#ifdef GL_ES\n"
        "precision highp float;\n"
        "#else\n"
        "#define highp\n"
        "#define mediump\n"
        "#define lowp\n"
        "#endif\n"
        "varying vec2 textureCoordinate;\n"
        "uniform sampler2D inputImageTexture;\n"
        "uniform sampler2D blendTexture;\n"
        "uniform int baseTexWidth;\n"
        "uniform int baseTexHeight;\n"
        "uniform vec2 fullBlendTexSize;\n"
        "uniform int blendMode;\n"
        "uniform float alphaFactor;\n"
        "// normal"
        "vec3 blendNormal(vec3 base, vec3 blend) {\n"
        "    return blend;\n"
        "}\n"
        "vec3 blendNormal(vec3 base, vec3 blend, float opacity) {\n"
        "    return (blendNormal(base, blend) * opacity blend * (1.0 - opacity));\n"
        "}\n"
        "// add"
        "float blendAdd(float base, float blend) {\n"
        "    return min(base+blend,1.0);\n"
        "}\n"
        "vec3 blendAdd(vec3 base, vec3 blend) {\n"
        "    return min(base+blend,vec3(1.0));\n"
        "}\n"
        "// average"
        "vec3 blendAverage(vec3 base, vec3 blend) {\n"
        "    return (base+blend)/2.0;\n"
        "}\n"
        "// color burn\n"
        "float blendColorBurn(float base, float blend) {\n"
        "    return (blend==0.0)?blend:max((1.0-((1.0-base)/blend)),0.0);\n"
        "}\n"
        "vec3 blendColorBurn(vec3 base, vec3 blend) {\n"
        "    return vec3(blendColorBurn(base.r,blend.r),blendColorBurn(base.g,blend.g),blendColorBurn(base.b,blend.b));\n" // NOLINT
        "}\n"
        "//color dodge\n"
        "float blendColorDodge(float base, float blend) {\n"
        "    return (blend==1.0)?blend:min(base/(1.0-blend),1.0);\n"
        "}\n"
        "vec3 blendColorDodge(vec3 base, vec3 blend) {\n"
        "    return vec3(blendColorDodge(base.r,blend.r),blendColorDodge(base.g,blend.g),blendColorDodge(base.b,blend.b));\n" // NOLINT
        "}\n"
        "// darken"
        "float blendDarken(float base, float blend) {\n"
        "    return min(blend,base);\n"
        "}\n"
        "vec3 blendDarken(vec3 base, vec3 blend) {\n"
        "    return vec3(blendDarken(base.r,blend.r),blendDarken(base.g,blend.g),blendDarken(base.b,blend.b));\n"
        "}\n"
        "// difference"
        "vec3 blendDifference(vec3 base, vec3 blend) {\n"
        "    return abs(base-blend);\n"
        "}\n"
        "// exclusion"
        "vec3 blendExclusion(vec3 base, vec3 blend) {\n"
        "    return base+blend-2.0*base*blend;\n"
        "}\n"
        "// reflect"
        "float blendReflect(float base, float blend) {\n"
        "    return (blend==1.0)?blend:min(base*base/(1.0-blend),1.0);\n"
        "}\n"
        "vec3 blendReflect(vec3 base, vec3 blend) {\n"
        "    return vec3(blendReflect(base.r,blend.r),blendReflect(base.g,blend.g),blendReflect(base.b,blend.b));\n"
        "}\n"
        "// glow"
        "vec3 blendGlow(vec3 base, vec3 blend) {\n"
        "    return blendReflect(blend,base);\n"
        "}\n"
        "// hard light"
        "float blendHardLight(float base, float blend) {\n"
        "    return base<0.5?(2.0*base*blend):(1.0-2.0*(1.0-base)*(1.0-blend));\n"
        "}\n"
        "vec3 blendHardLight(vec3 base, vec3 blend) {\n"
        "    return vec3(blendHardLight(base.r,blend.r),blendHardLight(base.g,blend.g),blendHardLight(base.b,blend.b));\n" // NOLINT
        "}\n"
        "// hard mix"
        "float blendHardMix(float base, float blend) {\n"
        "    if(blend<0.5) {\n"
        "        float vividLight = blendColorBurn(base,(2.0*blend));\n"
        "        return (vividLight < 0.5 ) ? 0.0:1.0;\n"
        "    } else {\n"
        "        float vividLight = blendColorDodge(base,(2.0*(blend-0.5)));\n"
        "        return (vividLight < 0.5 ) ? 0.0:1.0;\n"
        "    }\n"
        "}\n"
        "vec3 blendHardMix(vec3 base, vec3 blend) {\n"
        "    return vec3(blendHardMix(base.r,blend.r),blendHardMix(base.g,blend.g),blendHardMix(base.b,blend.b));\n"
        "}\n"
        "// lighten"
        "float blendLighten(float base, float blend) {\n"
        "    return max(blend,base);\n"
        "}\n"
        "vec3 blendLighten(vec3 base, vec3 blend) {\n"
        "    return vec3(blendLighten(base.r,blend.r),blendLighten(base.g,blend.g),blendLighten(base.b,blend.b));\n"
        "}\n"
        "// linear burn"
        "float blendLinearBurn(float base, float blend) {\n"
        "    return max(base+blend-1.0,0.0);\n"
        "}\n"
        "vec3 blendLinearBurn(vec3 base, vec3 blend) {\n"
        "    return max(base+blend-vec3(1.0),vec3(0.0));\n"
        "}\n"
        "// linear dodge"
        "float blendLinearDodge(float base, float blend) {\n"
        "    return min(base+blend,1.0);\n"
        "}\n"
        "vec3 blendLinearDodge(vec3 base, vec3 blend) {\n"
        "    return min(base+blend,vec3(1.0));\n"
        "}\n"
        "// linear light"
        "float blendLinearLight(float base, float blend) {\n"
        "    return blend<0.5?blendLinearBurn(base,(2.0*blend)):blendLinearDodge(base,(2.0*(blend-0.5)));\n"
        "}\n"
        "vec3 blendLinearLight(vec3 base, vec3 blend) {\n"
        "    return vec3(blendLinearLight(base.r,blend.r),blendLinearLight(base.g,blend.g),blendLinearLight(base.b,blend.b));\n" // NOLINT
        "}\n"
        "// multiply"
        "vec3 blendMultiply(vec3 base, vec3 blend) {\n"
        "    return base*blend;\n"
        "}\n"
        "// negation"
        "vec3 blendNegation(vec3 base, vec3 blend) {\n"
        "    return vec3(1.0)-abs(vec3(1.0)-base-blend);\n"
        "}\n"
        "// overlay"
        "float blendOverlay(float base, float blend) {\n"
        "    return base<0.5?(2.0*base*blend):(1.0-2.0*(1.0-base)*(1.0-blend));\n"
        "}\n"
        "vec3 blendOverlay(vec3 base, vec3 blend) {\n"
        "    return vec3(blendOverlay(base.r,blend.r),blendOverlay(base.g,blend.g),blendOverlay(base.b,blend.b));\n"
        "}\n"
        "// phoenix"
        "vec3 blendPhoenix(vec3 base, vec3 blend) {\n"
        "    return min(base,blend)-max(base,blend)+vec3(1.0);\n"
        "}\n"
        "// pin light"
        "float blendPinLight(float base, float blend) {\n"
        "    return (blend<0.5)?blendDarken(base,(2.0*blend)):blendLighten(base,(2.0*(blend-0.5)));\n"
        "}\n"
        "vec3 blendPinLight(vec3 base, vec3 blend) {\n"
        "    return vec3(blendPinLight(base.r,blend.r),blendPinLight(base.g,blend.g),blendPinLight(base.b,blend.b));\n"
        "}\n"
        "// screen"
        "float blendScreen(float base, float blend) {\n"
        "    return 1.0-((1.0-base)*(1.0-blend));\n"
        "}\n"
        "vec3 blendScreen(vec3 base, vec3 blend) {\n"
        "    return vec3(blendScreen(base.r,blend.r),blendScreen(base.g,blend.g),blendScreen(base.b,blend.b));\n"
        "}\n"
        "// soft light"
        "float blendSoftLight(float base, float blend) {\n"
        "    return (blend<0.5)?(2.0*base*blend+base*base*(1.0-2.0*blend)):(sqrt(base)*(2.0*blend-1.0)+2.0*base*(1.0-blend));\n" // NOLINT
        "}\n"
        "vec3 blendSoftLight(vec3 base, vec3 blend) {\n"
        "    return vec3(blendSoftLight(base.r,blend.r),blendSoftLight(base.g,blend.g),blendSoftLight(base.b,blend.b));\n" // NOLINT
        "}\n"
        "// substract"
        "float blendSubstract(float base, float blend) {\n"
        "    return max(base+blend-1.0,0.0);\n"
        "}\n"
        "vec3 blendSubstract(vec3 base, vec3 blend) {\n"
        "    return max(base+blend-vec3(1.0),vec3(0.0));\n"
        "}\n"
        "// vivid light"
        "float blendVividLight(float base, float blend) {\n"
        "    return (blend<0.5)?blendColorBurn(base,(2.0*blend)):blendColorDodge(base,(2.0*(blend-0.5)));\n"
        "}\n"
        "vec3 blendVividLight(vec3 base, vec3 blend) {\n"
        "    return vec3(blendVividLight(base.r,blend.r),blendVividLight(base.g,blend.g),blendVividLight(base.b,blend.b));\n" // NOLINT
        "}\n"
        "// snow color"
        "vec3 RGBToHSL(vec3 color){\n"
        "    vec3 hsl;\n"
        "    float fmin = min(min(color.r, color.g), color.b);\n"
        "    float fmax = max(max(color.r, color.g), color.b);\n"
        "    float delta = fmax - fmin;\n"
        "    hsl.z = (fmax fmin) / 2.0;\n"
        "    if (delta == 0.0) {\n"
        "        hsl.x = 0.0;\n"
        "        hsl.y = 0.0;\n"
        "    } else {\n"
        "        if (hsl.z < 0.5) {\n"
        "            hsl.y = delta / (fmax fmin);\n"
        "        } else {\n"
        "            hsl.y = delta / (2.0 - fmax - fmin);\n"
        "        }\n"
        "        float deltaR = (((fmax - color.r) / 6.0) (delta / 2.0)) / delta;\n"
        "        float deltaG = (((fmax - color.g) / 6.0) (delta / 2.0)) / delta;\n"
        "        float deltaB = (((fmax - color.b) / 6.0) (delta / 2.0)) / delta;\n"
        "        if (color.r == fmax ) {\n"
        "            hsl.x = deltaB - deltaG;\n"
        "        } else if (color.g == fmax) {\n"
        "            hsl.x = (1.0 / 3.0) deltaR - deltaB;\n"
        "        } else if (color.b == fmax) {\n"
        "            hsl.x = (2.0 / 3.0) deltaG - deltaR;\n"
        "        }\n"
        "        if (hsl.x < 0.0) {\n"
        "            hsl.x= 1.0;\n"
        "        } else if (hsl.x > 1.0) {\n"
        "            hsl.x -= 1.0;\n"
        "        }\n"
        "    }\n"
        "    return hsl;\n"
        "}\n"
        "float HueToRGB(float f1, float f2, float hue){\n"
        "    if (hue < 0.0)\n"
        "        hue= 1.0;\n"
        "    else if (hue > 1.0)\n"
        "        hue -= 1.0;\n"
        "    float res;\n"
        "    if ((6.0 * hue) < 1.0)\n"
        "        res = f1 (f2 - f1) * 6.0 * hue;\n"
        "    else if ((2.0 * hue) < 1.0)\n"
        "        res = f2;\n"
        "    else if ((3.0 * hue) < 2.0)\n"
        "        res = f1 (f2 - f1) * ((2.0 / 3.0) - hue) * 6.0;\n"
        "    else\n"
        "        res = f1;\n"
        "    return res;\n"
        "}\n"
        "vec3 HSLToRGB(vec3 hsl){\n"
        "    vec3 rgb;\n"
        "    if (hsl.y == 0.0)\n"
        "        rgb = vec3(hsl.z);\n"
        "    else {\n"
        "        float f2;\n"
        "        if (hsl.z < 0.5) {\n"
        "            f2 = hsl.z * (1.0 hsl.y);\n"
        "        } else {\n"
        "            f2 = (hsl.z hsl.y) - (hsl.y * hsl.z);\n"
        "        } \n"
        "        float f1 = 2.0 * hsl.z - f2;\n"
        "        rgb.r = HueToRGB(f1, f2, hsl.x (1.0/3.0));\n"
        "        rgb.g = HueToRGB(f1, f2, hsl.x);\n"
        "        rgb.b= HueToRGB(f1, f2, hsl.x - (1.0/3.0));\n"
        "    }\n"
        "    return rgb;\n"
        "}\n"
        "vec3 blendSnowColor(vec3 blend, vec3 bgColor) {\n"
        "    vec3 blendHSL = RGBToHSL(blend);\n"
        "    vec3 hsl = RGBToHSL(bgColor);\n"
        "    return HSLToRGB(vec3(blendHSL.r, blendHSL.g, hsl.b));\n"
        "}\n"
        "// snow hue"
        "vec3 blendSnowHue(vec3 blend, vec3 bgColor) {\n"
        "    vec3 baseHSL = RGBToHSL(bgColor.rgb);\n"
        "    return HSLToRGB(vec3(RGBToHSL(blend.rgb).r, baseHSL.g, baseHSL.b));\n"
        "}\n"
        "vec3 blendFunc(vec3 base, vec3 blend, float opacity,int blendMode) {\n"
        "    if (blendMode == 0)\n"
        "        return (blendNormal(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 1)\n"
        "       return (blendAdd(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 2)\n"
        "       return (blendAverage(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 3)\n"
        "       return (blendColorBurn(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 4)\n"
        "       return (blendColorDodge(base, blend) * opacity base * (1.0 - opacity));   \n"
        "    else if (blendMode == 5)\n"
        "       return (blendDarken(base, blend) * opacity base * (1.0 - opacity)); \n"
        "    else if (blendMode == 6)\n"
        "       return (blendDifference(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 7)\n"
        "       return (blendExclusion(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 8)\n"
        "       return (blendGlow(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 9)\n"
        "       return (blendHardLight(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 10)\n"
        "       return (blendHardMix(base, blend) * opacity base * (1.0 - opacity));\t\n"
        "    else if (blendMode == 11)\n"
        "       return (blendLighten(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 12)\n"
        "       return (blendLinearBurn(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 13)\n"
        "       return (blendLinearDodge(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 14)\n"
        "       return (blendLinearLight(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 15)\n"
        "       return (blendMultiply(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 16)\n"
        "        return (blendNegation(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 17)\n"
        "        return (blendOverlay(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 18)\n"
        "        return (blendPhoenix(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 19)\n"
        "        return (blendPinLight(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 20)\n"
        "        return (blendReflect(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 21)\n"
        "        return (blendScreen(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 22)\n"
        "        return (blendSoftLight(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 23)\n"
        "        return (blendSubstract(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 24)\n"
        "        return (blendVividLight(base, blend) * opacity base * (1.0 - opacity));\n"
        "    else if (blendMode == 25)\n"
        "        return blendSnowColor(blend, blend);\n"
        "    else if (blendMode == 26)\n"
        "        return blendSnowHue(blend, blend);\n"
        "    else \n"
        "        return base;\n"
        "}\n"
        "vec2 sucaiAlign(vec2 videoUV,vec2 videoSize,vec2 sucaiSize,vec2 anchorImageCoord,float sucaiScale) {\n"
        "    vec2 videoImageCoord = videoUV * videoSize;\n"
        "    vec2 sucaiUV= (videoImageCoord - anchorImageCoord)/(sucaiSize * sucaiScale) vec2(0.5);\n"
        "    return sucaiUV;\n"
        "}\n"
        "vec4 blendColor(sampler2D sucai, vec4 baseColor,vec2 videoSize,vec2 sucaiSize,vec2 anchorImageCoord,float sucaiScale) {\n" // NOLINT
        "    lowp vec4 resultColor = baseColor;  \n"
        "    vec2 sucaiUV = sucaiAlign(textureCoordinate,videoSize,sucaiSize,anchorImageCoord,sucaiScale);\n"
        "    lowp vec4 fgColor = baseColor;\n"
        "     if(sucaiUV.x >= 0.0 && sucaiUV.x <= 1.0 && sucaiUV.y >= 0.0 && sucaiUV.y <= 1.0 ) {\n"
        "        // sucaiUV.y = 1.0 - sucaiUV.y;\n"
        "        fgColor = texture2D(sucai,sucaiUV);\n"
        "    } else {\n"
        "        return baseColor;\n"
        "    }\n"
        "    fgColor = fgColor * alphaFactor;\n"
        "    int newBlendMode = blendMode; \n"
        "    if (newBlendMode <= 0) {\n"
        "        return fgColor;\n"
        "    }\n"
        "    if (newBlendMode >= 1000) {\n"
        "        newBlendMode = newBlendMode - 1000;\n"
        "        if (fgColor.a == 0.0) {\n"
        "            // resultColor = fgColor;\n"
        "            return fgColor;\n"
        "        } \n"
        "    }\n"
        "    lowp vec3 color = blendFunc(baseColor.rgb, clamp(fgColor.rgb * (1.0 / fgColor.a), 0.0, 1.0), 1.0,newBlendMode);\n" // NOLINT
        "    resultColor.rgb = baseColor.rgb * (1.0 - fgColor.a) color.rgb * fgColor.a;  \n"
        "    resultColor.a = 1.0;    \n"
        "    return resultColor;\n"
        "}\n"
        "void main(void) { \n"
        "    vec2 baseTexureSize = vec2(baseTexWidth,baseTexHeight);\n"
        "    vec2 fullBlendAnchor = baseTexureSize * 0.5;\n"
        "    float scale = 1.0;\n"
        "    //外居中对齐\n"
        "    float baseAspectRatio = baseTexureSize.y/baseTexureSize.x;\n"
        "    float blendAspectRatio = fullBlendTexSize.y/fullBlendTexSize.x;\n"
        "    if(baseAspectRatio >= blendAspectRatio) {\n"
        "        scale = baseTexureSize.y / fullBlendTexSize.y;   \n"
        "    } else {\n"
        "        scale = baseTexureSize.x / fullBlendTexSize.x; \n"
        "    }\n"
        "    lowp vec4 baseColor = texture2D(inputImageTexture,textureCoordinate);\n"
        "    lowp vec4 fullblendColor = blendColor(blendTexture,baseColor,baseTexureSize,fullBlendTexSize,fullBlendAnchor,scale);\n" // NOLINT
        "    gl_FragColor = fullblendColor;\n"
        "}\n";

class Blend {
 public:
  explicit Blend(const char* fragment_shader);
  virtual ~Blend();
  int ProcessImage(int texture_id, int sticker_texture_id, int frame_width, int frame_height,
                   GLfloat* matrix, float alpha_factor);
  int ProcessImage(int texture_id, int sticker_texture_id, int frame_width, int frame_height, GLfloat* matrix,
                   GLfloat* texture_coordinate, float alpha_factor);
  int ProcessImage(int texture_id, int sticker_texture_id, int frame_width, int frame_height,
                   int target_width, int target_height, GLfloat* matrix,
                   GLfloat* vertex_coordinate, GLfloat* texture_coordinate, float alpha_factor);

 private:
  void CreateFrameBuffer(int width, int height);
  void DeleteFrameBuffer();

 private:
  int program_;
  int second_program_;
  GLuint frame_buffer_id_;
  GLuint frame_buffer_texture_id_;
  GLfloat* default_vertex_coordinates_;
  GLfloat* default_texture_coordinates_;
  int source_width_;
  int source_height_;
  FrameBuffer* frame_buffer_;
};

class NormalBlend : public Blend {
 public:
  NormalBlend() : Blend(BLEND_FRAGMENT_SHADER) {}
};

class AddBlend : public Blend {
 public:
  AddBlend() : Blend(ADD_BLEND_FRAGMENT_SHADER) {}
};

class OverlayBlend : public Blend {
 public:
  OverlayBlend() : Blend(OVERLAY_BLEND_FRAGMENT_SHADER) {}
};

class ColorDodgeBlend : public Blend {
 public:
  ColorDodgeBlend() : Blend(COLOR_DODGE_BLEND_FRAGMENT_SHADER) {}
};

class HardLightBlend : public Blend {
 public:
  HardLightBlend() : Blend(HARD_LIGHT_BLEND_FRAGMENT_SHADER) {}
};

class MultiplyBlend : public Blend {
 public:
  MultiplyBlend() : Blend(MULTIPLY_BLEND_FRAGMENT_SHADER) {}
};

class PinLightBlend : public Blend {
 public:
  PinLightBlend() : Blend(PIN_LIGHT_BLEND_FRAGMENT_SHADER) {}
};

class ScreenBlend : public Blend {
 public:
  ScreenBlend() : Blend(SCREEN_BLEND_FRAGMENT_SHADER) {}
};

class SoftLightBlend : public Blend {
 public:
  SoftLightBlend() : Blend(SOFT_LIGHT_BLEND_FRAGMENT_SHADER) {}
};

class BlendFactory {
 public:
  static Blend* CreateBlend(int blend_type) {
    switch (blend_type) {
      case NORMAL_BLEND:
        return new NormalBlend();
      case ADD_BLEND:
      case ADD2_BLEND:
        return new AddBlend();
      case OVERLAY_BLEND:
      case OVERLAY2_BLEND:
        return new OverlayBlend();
      case COLOR_DODGE_BLEND:
        return new ColorDodgeBlend();
      case HARD_LIGHT_BLEND:
        return new HardLightBlend();
      case MULTIPLY_BLEND:
        return new MultiplyBlend();
      case PIN_LIGHT_BLEND:
        return new PinLightBlend();
      case SCREEN_BLEND:
        return new ScreenBlend();
      case SOFT_LIGHT_BLEND:
        return new SoftLightBlend();
      default:
        return new NormalBlend();
    }
  }
};

}  // namespace effect

#endif  // OPENGL_GL_BLEND_H_