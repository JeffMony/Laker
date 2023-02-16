
#include "sticker_parse.h"

namespace effect {

int StickerParse::Parse(const Json::Value &item,
                        const char *resource_root_path,
                        std::list<SubEffect *> &sub_effects,
                        bool encrypt) {
  auto sticker = new StickerSubEffect();
  int ret = ParseStickerResource(item, sticker, encrypt);
  if (ret >= 0) {
    sub_effects.push_back(sticker);
  } else {
    delete sticker;
  }
  return ret;
}

/**
 * center_x/center_y
 * scale
 * rotate
 * z_order
 * startTime/endTime
 * path : 贴纸路径
 *
 * @param item
 * @param sticker_sub_effect
 * @param encrypt
 * @return
 */
int StickerParse::ParseStickerResource(const Json::Value &item,
                                       StickerSubEffect *sticker_sub_effect,
                                       bool encrypt) {
  auto path_json = item["path"];
  if (!path_json.isNull()) {
    auto path = path_json.asString();
    int ret = sticker_sub_effect->SetStickerInfo(path);
    if (ret != 0) {
      return ret;
    }
  }
  sticker_sub_effect->type.append(item["type"].asString());
  auto start_time = item["startTime"].asInt();
  int end_time = INT32_MAX;
  auto end_time_json = item["endTime"];
  if (!end_time_json.isNull()) {
    end_time = end_time_json.asInt();
  }
  sticker_sub_effect->start_time = start_time;
  sticker_sub_effect->end_time = end_time;
  float center_x = 0.0f;
  float center_y = 0.0f;
  auto center_x_json = item["center_x"];
  auto center_y_json = item["center_y"];
  if (!center_x_json.isNull()) {
    center_x = center_x_json.asFloat();
  }
  if (!center_y_json.isNull()) {
    center_y = center_y_json.asFloat();
  }
  sticker_sub_effect->SetCenter(center_x, center_y);
  auto scale_json = item["scale"];
  auto rotate_json = item["rotate"];
  if (!scale_json.isNull()) {
    sticker_sub_effect->SetScale(scale_json.asFloat());
  }
  if (!rotate_json.isNull()) {
    sticker_sub_effect->SetRotate(rotate_json.asFloat());
  }
  auto z_order_json = item["z_order"];
  if (!z_order_json.isNull()) {
    sticker_sub_effect->z_order = z_order_json.asInt();
  }
  return 0;
}

}