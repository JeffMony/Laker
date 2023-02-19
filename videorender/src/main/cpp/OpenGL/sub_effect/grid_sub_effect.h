//
// Created by jeff lee on 2023/2/19.
//

#ifndef LAKER_VIDEORENDER_SRC_MAIN_CPP_OPENGL_SUB_EFFECT_GRID_SUB_EFFECT_H_
#define LAKER_VIDEORENDER_SRC_MAIN_CPP_OPENGL_SUB_EFFECT_GRID_SUB_EFFECT_H_

#include "sub_effect.h"
#include "gl/grid_buffer.h"

namespace effect {

class GridSubEffect : public SubEffect {
 public:
  GridSubEffect();

  virtual ~GridSubEffect();

  GLuint ProcessImage(
      const std::list<SubEffect *> &sub_effects,
      int origin_texture_id,
      int texture_id,
      int frame_width,
      int frame_height,
      int target_width,
      int target_height,
      int64_t current_time,
      ImageResult *result) override;

 private:
  GridBuffer *grid_buffer_;

};

}

#endif //LAKER_VIDEORENDER_SRC_MAIN_CPP_OPENGL_SUB_EFFECT_GRID_SUB_EFFECT_H_
