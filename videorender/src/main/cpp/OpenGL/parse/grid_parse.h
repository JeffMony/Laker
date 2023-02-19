//
// Created by jeff lee on 2023/2/19.
//

#ifndef LAKER_VIDEORENDER_SRC_MAIN_CPP_OPENGL_PARSE_GRID_PARSE_H_
#define LAKER_VIDEORENDER_SRC_MAIN_CPP_OPENGL_PARSE_GRID_PARSE_H_

#include "json/json.h"
#include "OpenGL/sub_effect/grid_sub_effect.h"

namespace effect {

class GridParse {

 public:
  static int Parse(
      const Json::Value &item,
      const char *resource_root_path,
      std::list<SubEffect *> &sub_effects,
      bool encrypt = false);

  static int ParseGridResource(
      const Json::Value &item,
      GridSubEffect *grid_sub_effect,
      bool encrypt = false);
};

}

#endif //LAKER_VIDEORENDER_SRC_MAIN_CPP_OPENGL_PARSE_GRID_PARSE_H_
