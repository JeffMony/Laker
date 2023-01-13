
#include "background_parse.h"
#include "log.h"

namespace effect {

int BackgroundParse::Parse(const Json::Value &item,
                           const char *resource_root_path,
                           std::list<SubEffect *> &sub_effects,
                           bool encrypt) {
  /// background类型
  auto background = new BackgroundSubEffect();
  int ret = ParseBackgroundResource(item, background, encrypt);
  if (ret >= 0) {
    sub_effects.push_back(background);
  } else {
    delete background;
  }
  return ret;
}

int BackgroundParse::ParseBackgroundResource(const Json::Value &item,
                                             BackgroundSubEffect* background,
                                             bool encrypt) {
  auto background_type = -1;
  auto background_type_json = item["backgroundType"];
  if (!background_type_json.isNull()) {
    background_type = background_type_json.asInt();
  }
  auto start_time = item["startTime"].asInt();
  int end_time = INT32_MAX;
  auto end_time_json = item["endTime"];
  if (!end_time_json.isNull()) {
    end_time = end_time_json.asInt();
  }
  background->name.clear();
  background->name.append(item["name"].asString());
  background->type.append(item["type"].asString());
  background->SetTextureWrap(item["textureWrap"].asInt());
  if (background_type == BackgroundType::kBackgroundColor) {
    auto red = item["red"].asInt();
    auto green = item["green"].asInt();
    auto blue = item["blue"].asInt();
    auto alpha = item["alpha"].asInt();
    background->SetColor(red, green, blue, alpha);
  } else if (background_type == BackgroundType::kBackgroundImage) {
    auto blur = item["blur"].asInt();
    background->SetBlur(blur);
    auto path_json = item["path"];
    if (path_json.isNull()) {
      LOGE("%s %s %d image path is null.", __FILE_NAME__, __func__ , __LINE__);
      delete background;
      return -1;
    }
    auto path = path_json.asString();
    int ret = background->SetImagePath(path);
    if (ret != 0) {
      delete background;
      return ret;
    }
  } else if (background_type == BackgroundType::kBackgroundOriginalScaleDisplay) {
    background->SetOriginalScaleDisplay();
    background->SetFrameType(item["renderFrameType"].asInt());
  } else if (background_type == BackgroundType::kBackgroundBlur) {
    auto blur = item["blur"].asInt();
    background->SetBlur(blur);
  }
  auto x_json = item["x"];
  auto y_json = item["y"];
  if (!x_json.isNull() && !y_json.isNull()) {
    background->SetTranslation(x_json.asFloat(), y_json.asFloat());
  }
  auto scale_json = item["scale"];
  if (!scale_json.isNull()) {
    background->SetScale(scale_json.asFloat());
  }
  auto rotate_json = item["rotate"];
  if (!rotate_json.isNull()) {
    background->SetRotate(rotate_json.asFloat());
  }
  background->start_time = start_time;
  background->end_time = end_time;
  auto z_order_json = item["z_order"];
  if (!z_order_json.isNull()) {
    background->z_order = z_order_json.asInt();
  }
  return 0;
}

}