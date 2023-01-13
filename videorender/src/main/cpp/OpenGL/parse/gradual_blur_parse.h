
#ifndef OPENGL_PARSE_GRADUAL_BLUR_PARSE_H_
#define OPENGL_PARSE_GRADUAL_BLUR_PARSE_H_

#include "json/json.h"
#include "OpenGL/sub_effect/gradual_blur_sub_effect.h"

namespace effect {

class GradualBlurParse {
 public:
  static int Parse(const Json::Value &item,
                   const char *resource_root_path,
                   std::list<SubEffect *> &sub_effects,
                   bool encrypt = false);

 private:
  static int ParseGradualBlurResource(const Json::Value &item,
                                      GradualBlurSubEffect *blur_effect,
                                      bool encrypt = false);
};

}  // namespace effect


#endif  // OPENGL_PARSE_GRADUAL_BLUR_PARSE_H_