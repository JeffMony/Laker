
#include "parse_config.h"
#include <fstream>
#include <memory>
#include <string>
#include "tools.h"
#include "parse/background_parse.h"
#include "parse/sticker_parse.h"
#include "parse/color_adjust_parse.h"
#include "parse/shape_mask_parse.h"
#include "parse/gradual_blur_parse.h"

namespace effect {

int ParseConfig::ParseEffectConfig(const int effect_id,
                                   const char *config_path,
                                   const char *extra_path,
                                   EffectInputType type,
                                   std::list<SubEffect *> &sub_effects,
                                   bool encrypt) {
  Json::Value root;
  JSONCPP_STRING err;
  if (type == kEffectInputFile) {
    /// 文件类型, 读取文件, 解析json配置
    std::string config;
    config.append(config_path)
        .append("/")
        .append("config.json");
    std::string config_buffer;
    std::string buffer = media::read_file_to_string(config);
    if (buffer.empty()) {
      LOGE("%s %s %d decode decrypt error: %s", __FILE_NAME__, __func__ , __LINE__, config.c_str());
      return -1;
    }
    config_buffer.append(buffer);
    /// buffer类型, 直接解析
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(config_buffer.c_str(), config_buffer.c_str() + config_buffer.length(), &root,
                       &err)) {
      /// 解析失败
      LOGE("%s %s %d parse: %s error: %s", __FILE_NAME__, __func__ , __LINE__, config.c_str(), err.c_str());
      return -2;
    }
  } else if (type == kEffectInputBuffer) {
    /// buffer类型, 直接解析
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    std::string config(config_path);
    if (!reader->parse(config.c_str(), config.c_str() + config.length(), &root,
                       &err)) {
      /// 解析失败
      LOGE("%s %s %d parse: %s error: %s", __FILE_NAME__, __func__ , __LINE__, config_path, err.c_str());
      return -2;
    }
  }
  auto effect = root["effect"];
  for (auto& effect_item : effect) {
    auto effect_type = effect_item["type"].asString();
    if (effect_type.find("background") != std::string::npos) {
      /// 设置背景
      int ret = BackgroundParse::Parse(effect_item, config_path, sub_effects, encrypt);
      if (ret != 0) {
        return ret;
      }
    } else if (effect_type.find("sticker") != std::string::npos) {
      /// 设置贴纸
      int ret = StickerParse::Parse(effect_item, config_path, sub_effects, encrypt);
      if (ret != 0) {
        return ret;
      }
    } else if (effect_type.find("color_adjust") != std::string::npos) {
      /// 设置色彩调节
      int ret = ColorAdjustParse::Parse(effect_item, config_path, sub_effects, encrypt);
      if (ret != 0) {
        return ret;
      }
    } else if (effect_type.find("shape_mask") != std::string::npos) {
      /// 图片和视频蒙版
      int ret = ShapeMaskParse::Parse(effect_item, config_path, sub_effects, encrypt);
      if (ret != 0) {
        return ret;
      }
    } else if (effect_type.find("gradual_blur") != std::string::npos) {
      /// 过渡虚化效果
      int ret = GradualBlurParse::Parse(effect_item, config_path, sub_effects, encrypt);
      if (ret != 0) {
        return ret;
      }
    }
  }
  return 0;
}

int ParseConfig::UpdateEffectConfig(const int effect_id,
                                    const char *config,
                                    const char *extra_path,
                                    EffectInputType input_type,
                                    std::list<SubEffect *> &sub_effects,
                                    bool encrypt) {
  if (input_type == EffectInputType::kEffectInputBuffer) {
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING err;
    std::string update_config(config);
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(update_config.c_str(), update_config.c_str() + update_config.length(), &root, &err)) {
      return EXIT_FAILURE;
    }
    auto effect = root["effect"];
    for (auto effect_item : effect) {
      int start_time = -1;
      int end_time = -1;
      auto start_time_json = effect_item["startTime"];
      auto end_time_json = effect_item["endTime"];
      if (!start_time_json.isNull()) {
        start_time = start_time_json.asInt();
      }
      if (!end_time_json.isNull()) {
        end_time = end_time_json.asInt();
      }
      auto type = effect_item["type"].asString();
      auto name = effect_item["name"].asString();
      if (type == "background") {
        BackgroundSubEffect *background_sub_effect = nullptr;
        for (auto sub_effect : sub_effects) {
          if (name == sub_effect->name) {
            background_sub_effect = reinterpret_cast<BackgroundSubEffect *>(sub_effect);
          }
        }
        if (background_sub_effect == nullptr) {
          LOGE("%s %s %d find: %s background sub effect error.", __FILE_NAME__, __func__ , __LINE__, name.c_str());
          return -3;
        }
        int ret = BackgroundParse::ParseBackgroundResource(effect_item, background_sub_effect, encrypt);
        if (ret != 0) {
          return ret;
        }
      } else if (type == "color_adjust") {
        ColorAdjustSubEffect *color_adjust_sub_effect = nullptr;
        for (auto sub_effect : sub_effects) {
          if (type == sub_effect->type) {
            color_adjust_sub_effect = reinterpret_cast<ColorAdjustSubEffect *>(sub_effect);
            break;
          }
        }
        if (color_adjust_sub_effect == nullptr) {
          LOGE("%s %s %d find: %s color_adjust sub effect error.", __FILE_NAME__, __func__ , __LINE__, name.c_str());
          return -4;
        }
        int ret = ColorAdjustParse::ParseColorAdjustResource(effect_item, color_adjust_sub_effect, encrypt);
        if (ret != 0) {
          return ret;
        }
      }
    }
  }
  return 0;
}

}