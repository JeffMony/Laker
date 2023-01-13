

#ifndef OPENGL_PARSE_COLOR_ADJUST_PARSE_H_
#define OPENGL_PARSE_COLOR_ADJUST_PARSE_H_

#include "json/json.h"
#include "OpenGL/sub_effect/color_adjust_sub_effect.h"

namespace effect {

class ColorAdjustParse {
 public:
  static int Parse(const Json::Value &item,
                   const char *resource_root_path,
                   std::list<SubEffect *> &sub_effects,
                   bool encrypt = false);

  static int ParseColorAdjustResource(const Json::Value &item,
                                      ColorAdjustSubEffect *color_adjust,
                                      bool encrypt = false);


};

}

#endif  // OPENGL_PARSE_COLOR_ADJUST_PARSE_H_