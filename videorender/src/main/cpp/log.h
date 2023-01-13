//
// Created by jeffli on 2022/7/28.
//

#ifndef YUANPLAYER_GLPROCESS_SRC_MAIN_CPP_LOG_H_
#define YUANPLAYER_GLPROCESS_SRC_MAIN_CPP_LOG_H_

#include <android/log.h>

/// 打印可变参数
#define FF_LOG_TAG "render_sdk:"
#define LOGE(format, ...)  __android_log_print(ANDROID_LOG_ERROR, FF_LOG_TAG, format, ##__VA_ARGS__)
#define LOGI(format, ...)  __android_log_print(ANDROID_LOG_INFO,  FF_LOG_TAG, format, ##__VA_ARGS__)
#define LOGV(format, ...)  __android_log_print(ANDROID_LOG_VERBOSE,  FF_LOG_TAG, format, ##__VA_ARGS__)

#endif //YUANPLAYER_GLPROCESS_SRC_MAIN_CPP_LOG_H_
