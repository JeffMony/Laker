

#ifndef OPENGL_PARSE_SHAPE_MASK_PARSE_H_
#define OPENGL_PARSE_SHAPE_MASK_PARSE_H_

#include "json/json.h"
#include "OpenGL/sub_effect/shape_mask_sub_effect.h"

namespace effect {

class ShapeMaskParse {
 public:
  static int Parse(const Json::Value &item,
                   const char *resource_root_path,
                   std::list<SubEffect *> &sub_effects,
                   bool encrypt = false);

 private:
  static int ParseShapeMaskResource(const Json::Value &item,
                                    ShapeMaskSubEffect *shape_mask,
                                    bool encrypt = false);
};

}

#endif  // OPENGL_PARSE_SHAPE_MASK_PARSE_H_