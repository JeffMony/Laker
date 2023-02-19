//
// Created by jeff lee on 2023/2/19.
//

#ifndef LAKER_VIDEORENDER_SRC_MAIN_CPP_OPENGL_PARSE_BLACK_WHITE_PARSE_H_
#define LAKER_VIDEORENDER_SRC_MAIN_CPP_OPENGL_PARSE_BLACK_WHITE_PARSE_H_

#include "json/json.h"
#include "OpenGL/sub_effect/black_white_sub_effect.h"

namespace effect {

class BlackWhiteParse {

 public:
  static int Parse(
      const Json::Value &item,
      const char *resource_root_path,
      std::list<SubEffect *> &sub_effects,
      bool encrypt = false);

  static int ParseBlackWhiteResource(
      const Json::Value &item,
      BlackWhiteSubEffect *black_white_sub_effect,
      bool encrypt = false);

};

}

#endif //LAKER_VIDEORENDER_SRC_MAIN_CPP_OPENGL_PARSE_BLACK_WHITE_PARSE_H_
