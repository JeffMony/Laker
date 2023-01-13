
#ifndef COMMON_H_
#define COMMON_H_

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) { if (x) delete (x); (x) = nullptr;}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) { if (x) delete [] (x); (x) = nullptr;}
#endif
#ifndef SAFE_FREE
#define SAFE_FREE(p) if(p != NULL) {free(p); p = NULL;}
#endif

#ifndef SAFE_DELETE_TEXTURE
#define SAFE_DELETE_TEXTURE(x)                                                 \
  if (x > 0) {                                                                 \
    glDeleteTextures(1, &x);                                                   \
    x = 0;                                                                     \
  }
#endif

#ifndef SHADER_STRING
#define STRINGIZE(x) #x
#define STRINGIZE2(x) STRINGIZE(x)
#define SHADER_STRING(text) @ STRINGIZE2(text)
#endif

#ifdef USE_STB_IMAGE
#ifndef STBI_IMAGE
#define STBI_IMAGE
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_STATIC
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_image.h"
#include "stb_image_resize.h"
#endif
#endif

#endif  // COMMON_H_