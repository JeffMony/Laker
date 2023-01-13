
#ifndef OPENGL_EFFECT_H_
#define OPENGL_EFFECT_H_

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <list>
#include <map>
#include <string>

#include "input_type.h"
#include "sub_effect/sub_effect.h"
#include "gl/frame_buffer.h"
#include "sub_effect/effect_param_callback.h"

namespace effect {

/**
 * 一个config对应的一系列特效组合
 */
class Effect {
 public:
  Effect();
  ~Effect();

  /**
   * 设置特效参数回调接口
   */
  void SetEffectParamCallback(EffectParamCallback* callback);

  /**
   * 设置特效参数的回调接口
   * @param callBack 特效参数接口的实现类
   */
  void CallbackEffectParam(int effect_id, EffectParamCallback* callback);

  /**
   * 解析一个特效
   * @param effect_id 特效id
   * @param config 特效config的相对路径
   * @param extra_config 特效的额外信息, 没有的话可以传nullptr
   * @param type 特效config的数据类型, 文件还是buffer
   * @param callback 解析config.json 的回调数据
   * @param encrypt 输入的文件是否加密
   */
  int AddConfig(int effect_id, const char* config, const char* extra_config,
                EffectInputType type = kEffectInputFile, bool encrypt = false);

  /**
   * 更新一个特效的配置
   * @param config 需要更新特效的配置
   * @param type 特效config的数据类型, 文件还是buffer
   * @param callback 解析config.json 的回调数据
   * @param ae_callback ae 资源回调处理
   * @return 0成功
   */
  int UpdateConfig(int effect_id, const char* config, EffectInputType type = kEffectInputFile, bool encrypt = false);

  /**
   * 更新这一系列特效的显示时间
   * @param start_time 开始时间
   * @param end_time 结束时间
   */
  void UpdateTime(int start_time, int end_time);

  /**
   * 更新子特效特效的显示时间
   * @param name 子特效的名字
   * @param start_time 开始时间
   * @param end_time 结束时间
   */
  void UpdateTimeByName(const char* name, int start_time, int end_time);

  /**
   * 添加子特效的显示时间, 一个子特效可以在不同时间显示
   * @param name 子特效的名字
   * @param start_time 开始时间
   * @param end_time 结束时间
   */
  void AddSubEffectTimeByName(const char* name, int start_time, int end_time);

  // "effect": [
  //    {
  //        "type": "generalEffect",
  //        "name": "0FE9EEA282D3E37E1AAC95D530D9D047",
  //        "vertexShader": "resource/vshader.vs",
  //        "vertexUniforms": [],
  //        "fragmentShader": "resource/fshader.fs",
  //        "fragmentUniforms": [
  //            {
  //                "name": "inputImageTexture",
  //                "type": 100
  //            }
  //        ]
  //    }
  //]
  /**
   * 更新一个特效中某个固定名字的值
   * 以上面一个特效示例, 特效名字为 0FE9EEA282D3E37E1AAC95D530D9D047
   * shader中的变量名 inputImageTexture
   * @param effect_name 特效的名字
   * @param param_name 特效中shader的变量名
   * @param value 需要传入到shader变量的值
   */
  void UpdateParamFloat(const std::string& effect_name, const std::string& param_name, float value);

  /**
   * 更新一个特效中某个固定名字的值
   * 以上面一个特效示例, 特效名字为 0FE9EEA282D3E37E1AAC95D530D9D047
   * shader中的变量名 inputImageTexture
   * @param effect_name 特效的名字
   * @param param_name 特效中shader的变量名
   * @param value 需要传入到shader变量的值
   * @param length value的长度
   */
  void UpdateParamFloat(const std::string& effect_name, const std::string& param_name, float* value, int length);

  /**
   * 更新一个特效中某个固定名字的值
   * 以上面一个特效示例, 特效名字为 0FE9EEA282D3E37E1AAC95D530D9D047
   * shader中的变量名 inputImageTexture
   * @param effect_name 特效的名字
   * @param param_name 特效中shader的变量名
   * @param value 需要传入到shader变量的值
   */
  void UpdateParamInt(const std::string& effect_name, const std::string& param_name, int value);

  /**
   * 处理一帧特效
   * @param texture_id 待处理的纹理
   * @param frame_width 纹理的宽
   * @param frame_height 纹理的高
   * @param target_width 目标的宽
   * @param target_height 目标的高
   * @param current_time 当前的时间
   * @return 处理好的纹理
   */
  GLuint ProcessImage(GLuint texture_id, int frame_width, int frame_height,
                      int target_width, int target_height, int64_t current_time);

  /**
   * 处理一帧特效
   * @param texture_id 待处理的纹理
   * @param frame_width 纹理的宽
   * @param frame_height 纹理的高
   * @param target_width 目标的宽
   * @param target_height 目标的高
   * @param current_time 当前的时间
   * @param result 返回处理后的纹理size
   * @return 处理好的纹理
   */
  GLuint ProcessImage(GLuint texture_id, int frame_width, int frame_height,
                      int target_width, int target_height, int64_t current_time, ImageResult* result);

 private:
  /// 特效参数回调
  EffectParamCallback* effect_param_callback_;
  /// 当前特效中包含的所有子特效
  std::list<SubEffect*> sub_effects_;
  /// 暂时没啥用
  int start_time_;
  int end_time_;
};

}

#endif  // OPENGL_EFFECT_H_