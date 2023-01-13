
#ifndef OPENGL_GL_COMMON_H_
#define OPENGL_GL_COMMON_H_

/// 旋转角度为0的纹理坐标
static float TEXTURE_COORDINATE_0[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
};

/// 旋转角度为90的纹理坐标
static float TEXTURE_COORDINATE_90[] = {
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 1.0f
};

/// 旋转角度为180的纹理坐标
static float TEXTURE_COORDINATE_180[] = {
    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};

/// 旋转角度为270的纹理坐标
static float TEXTURE_COORDINATE_270[] = {
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
};

/// 左右翻转的纹理坐标
static float TEXTURE_COORDINATE_FLIP_LEFT_RIGHT[] = {
    1.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
};

/// 旋转90度，然后左右翻转
static float TEXTURE_COORDINATE_90_FLIP_LEFT_RIGHT[] = {
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 0.0f
};

/// 旋转270度，然后左右翻转
static float TEXTURE_COORDINATE_270_FLIP_LEFT_RIGHT[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
};

/// 上下翻转的纹理坐标
static float TEXTURE_COORDINATE_FLIP_UP_DOWN[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f
};

/// 旋转90度，然后上下翻转
static float TEXTURE_COORDINATE_90_FLIP_UP_DOWN[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
};

/// 旋转180度，然后上下翻转
static float TEXTURE_COORDINATE_180_FLIP_UP_DOWN[] = {
    1.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
};

/// 旋转270度，然后上下翻转
static float TEXTURE_COORDINATE_270_FLIP_UP_DOWN[] = {
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 0.0f
};

#endif  // OPENGL_GL_COMMON_H_