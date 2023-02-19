//
// Created by jeff lee on 2023/2/19.
//

#include "black_white_buffer.h"

namespace effect {

BlackWhiteBuffer::BlackWhiteBuffer(
    int width,
    int height,
    const char *vertex_shader,
    const char *fragment_shader)
    : FrameBuffer(width, height, vertex_shader, fragment_shader) {

}

BlackWhiteBuffer::~BlackWhiteBuffer() {

}

void BlackWhiteBuffer::RunOnDrawTasks() {
  OpenGL::RunOnDrawTasks();
}

}