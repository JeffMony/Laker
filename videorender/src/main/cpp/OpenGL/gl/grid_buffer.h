//
// Created by jeff lee on 2023/2/19.
//

#ifndef LAKER_VIDEORENDER_SRC_MAIN_CPP_OPENGL_GL_GRID_BUFFER_H_
#define LAKER_VIDEORENDER_SRC_MAIN_CPP_OPENGL_GL_GRID_BUFFER_H_

#include "frame_buffer.h"

namespace effect {

class GridBuffer : public FrameBuffer{
 public:
  GridBuffer(int width, int height, const char *vertex_shader, const char *fragment_shader);

  virtual ~GridBuffer();

 protected:
  void RunOnDrawTasks() override;
};

}

#endif //LAKER_VIDEORENDER_SRC_MAIN_CPP_OPENGL_GL_GRID_BUFFER_H_
