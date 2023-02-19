//
// Created by jeff lee on 2023/2/19.
//

#include "grid_parse.h"

namespace effect {

int GridParse::Parse(
    const Json::Value &item,
    const char *resource_root_path,
    std::list<SubEffect *> &sub_effects,
    bool encrypt) {
  auto grid_sub_effect = new GridSubEffect();
  int ret = ParseGridResource(item, grid_sub_effect, encrypt);
  if (ret >= 0) {
    sub_effects.push_back(grid_sub_effect);
  } else {
    delete grid_sub_effect;
  }
  return ret;
}

int GridParse::ParseGridResource(
    const Json::Value &item,
    GridSubEffect *grid_sub_effect,
    bool encrypt) {
  /**
   * 黑白滤镜暂时只有一个字段
   */
  auto type = item["type"].asString();
  if (type.find("grid") != std::string::npos) {
    return 0;
  }
  return -1;
}

}