

#ifndef OPENGL_GL_PROGRAM_H_
#define OPENGL_GL_PROGRAM_H_

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace effect {

class Program {
 public:
  static int CreateProgram(const char* vertex, const char* fragment);

 private:
  static int CompileShader(const char* shader_string, GLuint shader);

  static int Link(int program);
};

}
#endif  // OPENGL_GL_PROGRAM_H_