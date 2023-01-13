
#ifndef TOOLS_H_
#define TOOLS_H_

#include <dirent.h>
#include <unistd.h>
#include <sys/time.h>
#include <cstring>
#include <string>
#include <fstream>
#include <algorithm>
#include "log.h"

namespace media {

/**
 * 读取文件内容到buffer
 */
static int read_file_to_buffer(char *path, char **buffer) {
  FILE *file = fopen(path, "r");
  if (file == nullptr) {
    return -1;
  }
  fseek(file, 0, SEEK_END);
  auto file_size = ftell(file);
  rewind(file);
  char *data = new char[sizeof(char) * file_size + 1];
  memset(data, 0, sizeof(char) * file_size);
  data[file_size] = '\0';
  size_t read_size = fread(data, 1, file_size, file);
  if (read_size != file_size) {
    fclose(file);
    delete[] data;
    return -3;
  }
  fclose(file);
  *buffer = data;
  return 0;
}

static std::string read_file_to_string(const std::string &path) {
  std::ifstream input_stream(path);
  if (!input_stream.is_open()) {
    /// 文件打开失败, 返回一个空串
    LOGE("%s %s %d input file: %s is not open.", __FILE_NAME__, __func__ , __LINE__, path.c_str());
    return "";
  }
  /// 读取文件中的全部内容
  std::string input_buffer((std::istreambuf_iterator<char>(input_stream)), std::istreambuf_iterator<char>());
  input_stream.close();
  return input_buffer;
}

static int check_file(const char *path) {
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    return -1;
  }
  return 0;
}

/**
 * 获取文件后缀名，最后统一转化为小写
 * @param path
 * @return
 */
static inline std::string get_file_suffix(std::string path) {
  if (path.empty()) {
    return "";
  }
  auto position = path.rfind(".");
  if (position != path.npos) {
    std::string result = path.substr(position + 1, path.length());
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
  }
  return "";
}

/// 检查文件类型, 读取文件头12个字节
/// 100为图片类型, 101为mp4类型
static int check_file_type(const char *path, int *type) {
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    return -1;
  }
  auto buffer = new uint8_t[12];
  fread(buffer, sizeof(uint8_t), sizeof(uint8_t) * 12, file);
//  for (int i=0; i < 12; i++) {
//    LOGE("file buffer %c", buffer[i]);
//  }
  fclose(file);
  int ret = 0;
  if (!memcmp("GIF", buffer, 3)) {
    /**
     * GIF目前有两种类型
     * GIF89a
     * GIF87a
     */
    *type = 103;
  } else if (!memcmp("JFIF", buffer + 6, 4) ||
      !memcmp("Exif", buffer + 6, 4) ||
      !memcmp("Lavc", buffer + 6, 4) ||
      !memcmp("XICC", buffer + 5, 4) ||
      !memcmp("http", buffer + 6, 4)) {
    // JPEG
    *type = 100;
  } else if (!memcmp("PNG", buffer + 1, 3)) {
    // PNG
    *type = 100;
  } else if (!memcmp("WEBP", buffer + 8,  4)) {
    /// webp格式
    *type = 100;
  } else if (!memcmp("AVI", buffer + 8, 3)) {
    *type = 101;
  } else if (!memcmp("ftyp", buffer + 4, 4)) {
    // mp4
    *type = 101;

    /// 增加heic的识别, heic是图片的一种, 不过压缩率更好/能存储的数据类型更多
    if ((
        !memcmp("heic", buffer + 8, 4) ||
            !memcmp("msf1", buffer + 8, 4) ||
            !memcmp("mif1", buffer + 8, 4) ||
            !memcmp("heix", buffer + 8, 4) ||
            !memcmp("hevc", buffer + 8, 4) ||
            !memcmp("hevx", buffer + 8, 4)
    ) &&
        (
            strcmp(get_file_suffix(std::string(path)).c_str(), "heic") == 0 ||
                strcmp(get_file_suffix(std::string(path)).c_str(), "heif") == 0
        )
        ) {
      *type = 102;
    }
  } else if (!memcmp("IMKH", buffer, 4) || !memcmp("Sofdec", buffer, 6)) {
    /**
     * 具体见ffmpeg源码中mpeg.c中的mpegps_read_header
    if (!memcmp("IMKH", buffer, 4)) {
        m->imkh_cctv = 1;
    } else if (!memcmp("Sofdec", buffer, 6)) {
        m->sofdec = 1;
    } else
       avio_seek(s->pb, last_pos, SEEK_SET);
    }
     */
    *type = 101;
  } else {
    if (strcmp(get_file_suffix(std::string(path)).c_str(), "mp4") == 0) {
      *type = 101;
    } else {
      ret = -2;
    }
  }
  delete[] buffer;
  return ret;
}

static void vertex_coordinate_scale(int source_width, int source_height,
                                    int target_width, int target_height,
                                    int frame_type, float *vertex_coordinate) {
  float target_ratio = target_width * 1.f / target_height;
  float scale_width = 1.0F;
  float scale_height = 1.0F;
  if (frame_type == 0) {
    /// 上下或者左右留黑
    float source_ratio = source_width * 1.f / source_height;
    if (source_ratio > target_ratio) {
      scale_width = 1.f;
      scale_height = target_ratio / source_ratio;
    } else {
      scale_width = source_ratio / target_ratio;
      scale_height = 1.f;
    }
  } else if (frame_type == 1) {
    /// 铺满target但是内容会被裁剪掉
    float source_ratio = source_width * 1.f / source_height;
    if (source_ratio > target_ratio) {
      scale_width = source_ratio / target_ratio;
      scale_height = 1.f;
    } else {
      scale_width = 1.f;
      scale_height = target_ratio / source_ratio;
    }
  }
  vertex_coordinate[0] = -scale_width;
  vertex_coordinate[1] = -scale_height;
  vertex_coordinate[2] = scale_width;
  vertex_coordinate[3] = -scale_height;
  vertex_coordinate[4] = -scale_width;
  vertex_coordinate[5] = scale_height;
  vertex_coordinate[6] = scale_width;
  vertex_coordinate[7] = scale_height;
}

static inline std::string shader_header() {
  std::string header;
  header.append("#ifdef GL_ES\n")
      .append("precision highp float;\n")
      .append("#else\n")
      .append("#define highp\n")
      .append("#define mediump\n")
      .append("#define lowp\n")
      .append("#endif\n");
  return header;
}

static inline int64_t get_current_time() {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static inline int64_t get_current_time_mills() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static inline int64_t get_current_time_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec;
}

static inline int64_t current_time_mills() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return static_cast<int64_t>(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

static void delete_directory(const std::string &directory) {
  if (access(directory.c_str(), 0) != 0) {
    return;
  }
  auto dir = opendir(directory.c_str());
  if (dir == nullptr) {
    return;
  }
  struct dirent *file;
  while ((file = readdir(dir)) != nullptr) {
    if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
      continue;
    }
    std::string path;
    path.append(directory).append("/").append(file->d_name);
    remove(path.c_str());
  }
  rmdir(directory.c_str());
}

/**
 * 获取文件名(带后缀名)
 * @param path
 * @return
 */
static inline std::string get_file_name(std::string path) {
  if (path.empty()) {
    return "";
  }
  auto position = path.rfind("/");
  if (position != path.npos) {
    return path.substr(position + 1, path.length());
  }
  return "";
}

/**
 * 获取文件名(不带后缀名)
 * @param path
 * @return
 */
static inline std::string get_file_name_without_suffix(std::string path) {
  if (path.empty()) {
    return "";
  }
  auto start = path.rfind("/");
  auto end = path.rfind(".");
  if (start != path.npos && end != path.npos) {
    return path.substr(start + 1, end - start - 1);
  }
  return "";
}

static inline std::string get_file_dir(std::string path) {
  if (path.empty()) {
    return "";
  }
  auto position = path.rfind("/");
  if (position != path.npos) {
    return path.substr(0, position + 1);
  }
  return "";
}

/**
 * 删除文件,首先是确保文件已经存在的情况下
 * @param path
 */
static inline void delete_file(const char* path) {
  int ret = remove(path);
  if (ret == 0) {
    LOGI("%s %s %d Delete file %s success", __FILE_NAME__, __func__ , __LINE__, path);
  } else {
    LOGE("%s %s %d Delete file %s failed, ret=%d", __FILE_NAME__, __func__ , __LINE__, path, ret);
  }
}

static inline void delete_file_str(const std::string& path) {
  if (path.empty()) {
    return;
  }
  if (check_file(path.c_str()) == 0) {
    delete_file(path.c_str());
  }
}

}  // namespace effect

#endif  // TOOLS_H_