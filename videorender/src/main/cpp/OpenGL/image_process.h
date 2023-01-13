
#ifndef OPENGL_IMAGE_PROCESS_H_
#define OPENGL_IMAGE_PROCESS_H_

#include <string>
#include <map>

#include "effect.h"
#include "gl/opengl.h"
#include "gl/filter.h"
#include "sub_effect/effect_param_callback.h"

namespace effect {

/**
 * 图像处理中间类
 */
class ImageProcess {
 public:
  ImageProcess();
  ~ImageProcess();

  /**
   * 生成一个effect id
   * @return effect_id
   */
  int GenerateEffectId();

  /**
   * 处理一帧图像
   * @param texture_id 需要处理的纹理
   * @param frame_width 图像的宽
   * @param frame_height 图像的高
   * @param target_width 窗口的宽, 如果是视频导出时,应该是输出视频的宽
   * @param target_height 窗口的高, 如果是视频导出时,应该是输出视频的宽
   * @param current_time 当前处理的时间, 在处理多个我特效时,每个特效都会有一个显示时间区域,处理时会根据传进来的current_time来决定是否显示某个效果
   * @return 返回处理后的效果纹理id
   */
  GLuint ProcessImage(GLuint texture_id, int frame_width, int frame_height,
                      int target_width, int target_height,
                      int64_t current_time, ImageResult* result = nullptr);

  /**
   * 获取特效的参数json信息
   * @param effect_id 特效id
   * @param effect_name 特效名字
   * @return 一个特效的参数json
   */
  std::string GetEffectParam(int effect_id, const char* effect_name) const;

  /**
   * 添加一个特效
   * @param effect_id 创建时返回的特效id
   * @param config_path config的相对路径, buffer时为buffer内容
   * @param extra_config 特效的额外信息, 如果不需要可以为nullptr
   * @param type 特效配置的类型,分为文件和buffer两种
   * @param callback 外部纹理回调接口
   * @param encrypt 输入文件是否加密
   * @return 成功返回0, 其它为失败
   */
  int OnAddEffect(int effect_id, const char* config_path, const char* extra_config,
                  EffectInputType type = kEffectInputFile, bool encrypt = false);

  /**
   * 更新一个特效
   * @param effect_id 创建时返回的特效id
   * @param config config的相对路径, buffer时为buffer内容
   * @param type 特效配置的类型,分为文件和buffer两种
   * @return 成功返回0, 其它为失败
   */
  int OnUpdateEffect(int effect_id, const char* config, EffectInputType type = kEffectInputBuffer);

  /**
   * 更新一个特效的显示时间
   * @param effect_id 在创建特效时返回的id
   * @param start_time 特效的开始时间
   * @param end_time 特效的结束时间
   */
  void OnUpdateEffectTime(int effect_id, int start_time, int end_time);

  /**
   * 更新一个特效中子特效的显示时间
   * @param effect_id 在创建特效时返回的id
   * @param name 子特效的名字
   * @param start_time 特效的开始时间
   * @param end_time 特效的结束时间
   */
  void OnUpdateSubEffectTime(int effect_id, const char* name, int start_time, int end_time);

  /**
   * 添加子特效的显示时间, 一个子特效可以在不同时间显示
   * @param name 子特效的名字
   * @param start_time 开始时间
   * @param end_time 结束时间
   */
  void OnAddSubEffectTimeByName(int effect_id, const char* name, int start_time, int end_time);

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
   * @param effect_id 在创建特效时返回的id
   * @param effect_name 特效的名字
   * @param param_name 特效中shader的变量名
   * @param value 需要传入到shader变量的值
   */
  void OnUpdateEffectParamFloat(int effect_id, const std::string& effect_name,
                                const std::string& param_name, float value);

  /**
   * 更新一个特效中某个固定名字的值
   * 以上面一个特效示例, 特效名字为 0FE9EEA282D3E37E1AAC95D530D9D047
   * shader中的变量名 inputImageTexture
   * @param effect_id 在创建特效时返回的id
   * @param effect_name 特效的名字
   * @param param_name 特效中shader的变量名
   * @param value 需要传入到shader变量的值
   */
  void OnUpdateEffectParamInt(int effect_id, const std::string& effect_name,
                              const std::string& param_name, int value);

  /**
   * 根据特效id删除一个特效
   * @param effect_id 在创建特效时返回的id
   */
  void OnDeleteEffect(int effect_id);

  /**
   * 清除所有的特效
   */
  void ClearEffect();

  /**
   * 添加一个滤镜
   * @param effect_id 创建滤镜时返回的id
   * @param config_path 滤镜config的相对路径
   * @param encrypt 是否加密
   * @return 0成功, 其它失败
   */
  int OnAddFilter(int effect_id, const char* config_path, bool encrypt = false);

  /**
   * 添加一个滤镜
   * @param effect_id 创建滤镜时返回的id
   * @param config_path 滤镜config的相对路径
   * @param start_time 滤镜的开始时间
   * @param end_time 滤镜结束时间
   * @param encrypt 是否加密
   * @return 0成功, 其它失败
   */
  int OnAddFilter(int effect_id, const char* config_path, int start_time, int end_time, bool encrypt = false);

  /**
   * 更新一个滤镜的透明度
   * @param effect_id 创建滤镜时返回的id
   * @param intensity 滤镜的透明度 0 ~ 100
   */
  void OnUpdateFilterIntensity(int effect_id, int intensity);

  /**
   * 更新一个滤镜的显示时间
   * @param effect_id 创建滤镜时返回的id
   * @param start_time 滤镜的开始时间
   * @param end_time 滤镜结束时间
   */
  void OnUpdateFilterTime(int effect_id, int start_time, int end_time);

  /**
   * 删除一个滤镜
   * @param effect_id 创建滤镜时返回的id
   */
  void OnDeleteFilter(int effect_id);

  /**
   * 删除所有的滤镜
   */
  void ClearFilter();

  /**
   * 设置特效参数的回调接口
   * @param callback 特效参数接口的实现类
   */
  void SetEffectParamCallback(EffectParamCallback* callback);

 private:
  /**
   * 内部处理一帧的方法
   * @param texture_id 需要处理的纹理id
   * @param frame_width 纹理的宽
   * @param frame_height 纹理的高
   * @param target_width 窗口的宽, 如果是视频导出时,应该是输出视频的宽
   * @param target_height 窗口的高, 如果是视频导出时,应该是输出视频的宽
   * @param current_time 当前处理的时间, 在处理多个我特效时,每个特效都会有一个显示时间区域,处理时会根据传进来的current_time来决定是否显示某个效果
   * @param result 纹理的结果宽和高, 在有的特效中会改变纹理的宽和高, 比如 高斯模糊的背景
   */
  GLuint OnProcess(GLuint texture_id, int frame_width, int frame_height,
                   int target_width, int target_height,
                   int64_t current_time, ImageResult* result);

 private:
  /**
   * 特效的集合, 一个特效id, 对应一个config中一系列的特效组合
   */
  std::map<int, Effect*> effects_;
  /// 一个id, 对应一个滤镜
  std::map<int, Filter*> filters_;
  /// 自增的effect id
  /// 一个effect id 对应一组特效
  int effect_id_;
  /// 特效参数回调接口
  EffectParamCallback* effect_param_callback_;
};

}  // namespace effect


#endif  // OPENGL_IMAGE_PROCESS_H_