cmake_minimum_required(VERSION 3.4.1)
enable_language(ASM)

set(-DCMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -s -Wdeprecated-declarations")
set(-DCMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -s -Wdeprecated-declarations")

add_definitions("
-DGL_GLEXT_PROTOTYPES
-DEGL_EGLEXT_PROTOTYPES
-DUSE_STB_IMAGE
")

set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})

include_directories(${CURRENT_DIR}/glm)
include_directories(${CURRENT_DIR}/stb)
include_directories(${CURRENT_DIR}/json)
include_directories(${CURRENT_DIR}/json/include)

include_directories(${CURRENT_DIR})
include_directories(${CURRENT_DIR}/OpenGL)

# json相关代码
file(
        GLOB
        json_source
        ${CURRENT_DIR}/json/json_reader.cpp
        ${CURRENT_DIR}/json/json_value.cpp
        ${CURRENT_DIR}/json/json_writer.cpp
        ${CURRENT_DIR}/json/json_valueiterator.inl
)

file(
        GLOB
        effect_source
        OpenGL/egl/egl_core.cc
        OpenGL/gl/background.cc
        OpenGL/gl/blend.cc
        OpenGL/gl/color_adjust.cc
        OpenGL/gl/filter.cc
        OpenGL/gl/frame_buffer.cc
        OpenGL/gl/gaussian_blur.cc
        OpenGL/gl/gradual_blur.cc
        OpenGL/gl/opengl.cc
        OpenGL/gl/process_buffer.cc
        OpenGL/gl/program.cc
        OpenGL/gl/shape_mask.cc
        OpenGL/parse/background_parse.cc
        OpenGL/parse/color_adjust_parse.cc
        OpenGL/parse/gradual_blur_parse.cc
        OpenGL/parse/shape_mask_parse.cc
        OpenGL/parse/sticker_parse.cc
        OpenGL/sub_effect/background_sub_effect.cc
        OpenGL/sub_effect/color_adjust_sub_effect.cc
        OpenGL/sub_effect/gradual_blur_sub_effect.cc
        OpenGL/sub_effect/shape_mask_sub_effect.cc
        OpenGL/sub_effect/sticker_sub_effect.cc
        OpenGL/sub_effect/sub_effect.cc
        OpenGL/effect.cc
        OpenGL/image_process.cc
        OpenGL/parse_config.cc
)

file(
        GLOB
        render_source
        thread/handler.cc
        thread/handler_thread.cc
        thread/looper.cc
        thread/message.cc
        thread/message_queue.cc
        thread/thread_utils.cc
        thread/time_utils.cc
        error.cc
        media_env.c
        render_jni.cc
        render_process.cc
)

add_library(
        rendersdk
        SHARED
        ${json_source}
        ${effect_source}
        ${render_source}
)

target_link_libraries(
        rendersdk
        android
        EGL
        GLESv2
        log
        jnigraphics
        z
)