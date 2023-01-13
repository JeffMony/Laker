

#ifndef OPENGL_PARSE_STICKER_PARSE_H_
#define OPENGL_PARSE_STICKER_PARSE_H_

#include "json/json.h"
#include "OpenGL/sub_effect/sticker_sub_effect.h"

namespace effect {

class StickerParse {
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


  static int ParseStickerResource(const Json::Value& item,
                                  StickerSubEffect* sticker_sub_effect,
                                  bool encrypt = false);
};

}

#endif  // OPENGL_PARSE_STICKER_PARSE_H_