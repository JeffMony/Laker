
#ifndef OPENGL_PARSE_BACKGROUND_PARSE_H_
#define OPENGL_PARSE_BACKGROUND_PARSE_H_

#include "json/json.h"
#include "OpenGL/sub_effect/background_sub_effect.h"

namespace effect {

class BackgroundParse {
 public:
  /**
   * 解析background类型的信息,并添加到sub_effect list中
   * blend类型的一般会有素材资源进行融合, 会开启OpenGL的blend选项
   * @param item 当前的effect json item对象
   * @param resource_root_path 图片或者其它资源的相对路径
   * @param sub_effects 当前特效的子特效集合
   * @param encrypt 输入的文件是否加密
   * @return 返回0成功, 其它解析失败
   */
  static int Parse(const Json::Value& item,
                   const char* resource_root_path,
                   std::list<SubEffect*>& sub_effects,
                   bool encrypt = false);

  /**
   * 解析background信息
   * @param item 当前的effect json item对象
   * @param encrypt 输入的文件是否加密
   * @return 0成功
   */
  static int ParseBackgroundResource(const Json::Value& item,
                                     BackgroundSubEffect* background,
                                     bool encrypt = false);

};

}

#endif  // OPENGL_PARSE_BACKGROUND_PARSE_H_