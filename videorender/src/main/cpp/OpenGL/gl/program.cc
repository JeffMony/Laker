

#include "program.h"
#include <stdlib.h>
#include "log.h"

namespace effect {

int Program::CreateProgram(const char *vertex, const char *fragment) {
  auto program = glCreateProgram();
  auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  auto result = CompileShader(vertex, vertex_shader);
  if (result != 0) {
    glDeleteProgram(program);
    return 0;
  }
  result = CompileShader(fragment, fragment_shader);
  if (result != 0) {
    glDeleteProgram(program);
    return 0;
  }
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  result = Link(program);
  if (result != 0) {
    glDeleteProgram(program);
    return 0;
  }
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  return program;
}

int Program::CompileShader(const char *shader_string, GLuint shader) {
  glShaderSource(shader, 1, &shader_string, nullptr);
  glCompileShader(shader);
  GLint compiled = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    GLint infoLen;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen) {
      auto *buf = reinterpret_cast<char*>(malloc(static_cast<size_t>(infoLen)));
      if (buf) {
        LOGE("%s %s %d shader_string: %s", __FILE_NAME__, __func__ , __LINE__, shader_string);
        glGetShaderInfoLog(shader, infoLen, nullptr, buf);
        LOGE("%s %s %d Could not compile %d:\n%s\n", __FILE_NAME__, __func__ , __LINE__, shader, buf);
        free(buf);
      }
      glDeleteShader(shader);
    }
    return -1;
  }
  return 0;
}

int Program::Link(int program) {
  glLinkProgram(program);
  GLint linkStatus = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
  if (linkStatus == GL_FALSE) {
    GLint infoLen;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen) {
      auto *buf = reinterpret_cast<char*>(malloc(static_cast<size_t>(infoLen)));
      if (buf) {
        glGetProgramInfoLog(program, infoLen, nullptr, buf);
        LOGE("%s %s %d %s", __FILE_NAME__, __func__ , __LINE__, buf);
        free(buf);
      }
    }
    return -1;
  }
  return 0;
}

}  // namespace effect
