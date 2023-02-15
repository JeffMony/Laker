
#include "color_adjust_parse.h"
#include "OpenGL/gl/color_adjust.h"

namespace effect {

int ColorAdjustParse::Parse(const Json::Value &item,
                            const char *resource_root_path,
                            std::list<SubEffect *> &sub_effects,
                            bool encrypt) {
  auto color_adjust = new ColorAdjustSubEffect();
  int ret = ParseColorAdjustResource(item, color_adjust, encrypt);
  if (ret >= 0) {
    sub_effects.push_back(color_adjust);
  } else {
    delete color_adjust;
  }
  return ret;
}

int ColorAdjustParse::ParseColorAdjustResource(const Json::Value &item,
                                               ColorAdjustSubEffect *color_adjust,
                                               bool encrypt) {
  color_adjust->type.clear();
  color_adjust->type.append(item["type"].asString());
  auto method_bit_json = item["method_bit"];
  if (!method_bit_json.isNull()) {
    auto method_bit = method_bit_json.asInt();
    color_adjust->SetMethodBit(method_bit);
  }

  auto brightness_level_json = item["brightness_level"];
  if (!brightness_level_json.isNull()) {
    auto brightness_level = brightness_level_json.asFloat();
    color_adjust->SetBrightnessLevel(brightness_level);
  }
  auto contrast_level_json = item["contrast_level"];
  if (!contrast_level_json.isNull()) {
    auto contrast_level = contrast_level_json.asFloat();
    color_adjust->SetContrastLevel(contrast_level);
  }
  auto temperature_level_json = item["temperature_level"];
  if (!temperature_level_json.isNull()) {
    auto temperature_level = temperature_level_json.asFloat();
    color_adjust->SetTemperatureLevel(temperature_level);
  }
  auto saturation_level_json = item["saturation_level"];
  if (!saturation_level_json.isNull()) {
    auto saturation_level = saturation_level_json.asFloat();
    color_adjust->SetSaturationLevel(saturation_level);
  }
  auto grain_level_json = item["grain_level"];
  if (!grain_level_json.isNull()) {
    auto grain_level = grain_level_json.asFloat();
    color_adjust->SetGrainLevel(grain_level);
  }
  auto sharpness_level_json =  item["sharpness_level"];
  if (!sharpness_level_json.isNull()) {
    auto sharpness_level = sharpness_level_json.asFloat();
    color_adjust->SetSharpnessLevel(sharpness_level);
  }

  auto background_image_path_json = item["background_path"];
  if (!background_image_path_json.isNull()) {
    color_adjust->SetBackgroundImagePath(background_image_path_json.asString());
  }
  return 0;
}

}