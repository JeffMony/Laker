
#include "gradual_blur_parse.h"

namespace effect {

int GradualBlurParse::Parse(const Json::Value &item,
                            const char *resource_root_path,
                            std::list<SubEffect *> &sub_effects,
                            bool encrypt) {
  auto gradual_blur = new GradualBlurSubEffect();
  int ret = ParseGradualBlurResource(item, gradual_blur, encrypt);
  if (ret >= 0) {
    sub_effects.push_back(gradual_blur);
  } else {
    delete gradual_blur;
  }
  return ret;
}

int GradualBlurParse::ParseGradualBlurResource(const Json::Value &item,
                                               GradualBlurSubEffect *blur_effect,
                                               bool encrypt) {
  auto method_json = item["method"];
  if (!method_json.isNull()) {
    auto method = method_json.asCString();
    BlurType blur_type = GAUSSIAN;
    if (strcmp(method, "gaussian") == 0) {
      blur_type = GAUSSIAN;
    } else if (strcmp(method, "frost_glass") == 0) {
      blur_type = FROST_GLASS;
    }
    blur_effect->SetBlurType(blur_type);
  }
  auto gradient_json = item["gradient"];
  if (!gradient_json.isNull()) {
    auto gradient = gradient_json.asBool();
    blur_effect->SetGradient(gradient);
  }
  auto draw_line_json = item["draw_line"];
  if (!draw_line_json.isNull()) {
    auto draw_line = draw_line_json.asBool();
    blur_effect->SetDrawLine(draw_line);
  }
  int level = 0;
  auto level_json = item["blur_level"];
  if (!level_json.isNull()) {
    level = level_json.asInt();
  }
  int gradual_level = 0;
  auto gradual_level_json = item["gradual_blur_level"];
  if (!gradual_level_json.isNull()) {
    gradual_level = gradual_level_json.asInt();
  }
  blur_effect->SetLevel(level, gradual_level);
  auto gradual_shape_json = item["gradual_shape"];
  if (!gradual_shape_json.isNull()) {
    auto gradual_shape = gradual_shape_json.asCString();
    GradualShape shape;
    auto circle = item["circle"];
    auto rect = item["rect"];
    if (strcmp(gradual_shape, "circle") == 0 && !circle.isNull()) {
      shape = CIRCLE;
      auto center_x = circle["center_x"].asFloat();
      auto center_y = circle["center_y"].asFloat();
      auto in_radius = circle["in_radius"].asFloat();
      auto out_radius = circle["out_radius"].asFloat();
      blur_effect->SetCircleParams(center_x, center_y, in_radius, out_radius);
    } else if (strcmp(gradual_shape, "rect") == 0 && !rect.isNull()) {
      shape = RECT;
      auto out_start_y = rect["out_start_y"].asFloat();
      auto out_end_y = rect["out_end_y"].asFloat();
      auto in_start_y = rect["in_start_y"].asFloat();
      auto in_end_y = rect["in_end_y"].asFloat();
      blur_effect->SetRectParams(out_start_y, out_end_y, in_start_y, in_end_y);
    } else {
      shape = NONE;
    }
    blur_effect->SetGradualShape(shape);
  }
  auto background_image_path_json = item["background_path"];
  if (!background_image_path_json.isNull()) {
    blur_effect->SetBackgroundImagePath(background_image_path_json.asString());
  }

  auto foreground_image_path_json = item["foreground_path"];
  if (!foreground_image_path_json.isNull()) {
    blur_effect->SetForegroundImagePath(foreground_image_path_json.asString());
  }
  return 0;
}

}  // namespace effect