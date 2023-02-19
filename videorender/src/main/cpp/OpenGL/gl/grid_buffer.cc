//
// Created by jeff lee on 2023/2/19.
//

#include "grid_buffer.h"

namespace effect {

GridBuffer::GridBuffer(
    int width,
    int height,
    const char *vertex_shader,
    const char *fragment_shader)
    : FrameBuffer(width, height,vertex_shader,fragment_shader) {

}

GridBuffer::~GridBuffer() {

}

void GridBuffer::RunOnDrawTasks() {
  OpenGL::RunOnDrawTasks();
}

}