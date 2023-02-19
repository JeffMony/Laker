
#ifndef OPENGL_PARSE_CONFIG_H_
#define OPENGL_PARSE_CONFIG_H_

#include <list>
#include "input_type.h"
#include "json/json.h"
#include "sub_effect/sub_effect.h"

namespace effect {
class ParseConfig {

 public:
  /**
   * 解析一个特效json
   * @param config_path config的相对路径 /sdcard/Android/data/com.a.b/data/ 内部会把config.json自动拼接
   * @param extra_path 当前特效的额外信息, 不需要可以传nullptr
   * @param type 当前特效的config类型, 文件或者buffer
   * @param sub_effects 当前特效的子特效集合
   * @param encrypt 输入的文件是否加密
   * @return 返回0成功, 其它解析失败
   */
  static int ParseEffectConfig(
      const int effect_id,
      const char* config_path,
      const char* extra_path,
      EffectInputType type,
      std::list<SubEffect*>& sub_effects,
      bool encrypt = false);

  /**
   * 解析一个特效json
   * @param config config的相对路径 /sdcard/Android/data/com.a.b/data/ 内部会把config.json自动拼接
   * @param extra_path 当前特效的额外信息, 不需要可以传nullptr
   * @param input_type 当前特效的config类型, 文件或者buffer
   * @param sub_effects 当前特效的子特效集合
   * @param encrypt 输入的文件是否加密
   * @return 返回0成功, 其它解析失败
   */
  static int UpdateEffectConfig(
      const int effect_id,
      const char* config,
      const char* extra_path,
      EffectInputType input_type,
      std::list<SubEffect*>& sub_effects,
      bool encrypt = false);

};

} // namespace effect

#endif  // OPENGL_PARSE_CONFIG_H_