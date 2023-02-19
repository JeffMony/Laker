//
// Created by jeff lee on 2023/2/19.
//

#include "black_white_parse.h"

namespace effect {

int BlackWhiteParse::Parse(
    const Json::Value &item,
    const char *resource_root_path,
    std::list<SubEffect *> &sub_effects,
    bool encrypt) {
  auto black_white_sub_effect = new BlackWhiteSubEffect();
  int ret = ParseBlackWhiteResource(item, black_white_sub_effect, encrypt);
  if (ret >= 0) {
    sub_effects.push_back(black_white_sub_effect);
  } else {
    delete black_white_sub_effect;
  }
  return ret;
}

int BlackWhiteParse::ParseBlackWhiteResource(
    const Json::Value &item,
    BlackWhiteSubEffect *black_white_sub_effect,
    bool encrypt) {
  /**
   * 黑白滤镜暂时只有一个字段
   */
  auto type = item["type"].asString();
  if (type.find("black_white") != std::string::npos) {
    return 0;
  }
  return -1;
}

}