//
// Created by jeffli on 2022/7/29.
//

#ifndef YUANPLAYER_GLPROCESS_SRC_MAIN_CPP_RENDER_MESSAGE_H_
#define YUANPLAYER_GLPROCESS_SRC_MAIN_CPP_RENDER_MESSAGE_H_

namespace media {

enum EGLMessage {
  /// opengl 环境创建
  kCreateEGL = 0,
  /// 创建OpenGL Surface
  kCreateEGLSurface,
  /// Surface宽高改变
  kSurfaceChanged,
  /// 手动改变surface大小
  kSetSurfaceSize,
  /// 销毁OpenGL Surface
  kDestroyEGLSurface,
  /// 销毁OpenGL 环境
  kDestroyEGL
};

enum PlayerMessage {
  kRenderFrame = 2000,
  kSetVideoSize,
  kAddEffect,
  kUpdateEffect,
  kDeleteEffect,
  kAddFilter,
  kUpdateFilter,
  kUpdateFilterIntensity,
  kDeleteFilter,
  kCaptureFrame,
};

enum MirrorType {
  NONE_MIRROR = 0,
  VERTICAL_MIRROR,
  HORIZONTAL_MIRROR,
};

}

#endif //YUANPLAYER_GLPROCESS_SRC_MAIN_CPP_RENDER_MESSAGE_H_
