
#include "shape_mask_parse.h"
#include "OpenGL/gl/shape_mask.h"

namespace effect {

int effect::ShapeMaskParse::Parse(const Json::Value &item, const char *resource_root_path,
                                  std::list<SubEffect *> &sub_effects, bool encrypt) {
  auto shape_mask = new ShapeMaskSubEffect();
  int ret = ParseShapeMaskResource(item, shape_mask, encrypt);
  if (ret >= 0) {
    sub_effects.push_back(shape_mask);
  } else {
    delete shape_mask;
  }
  return ret;
}

int ShapeMaskParse::ParseShapeMaskResource(const Json::Value &item, ShapeMaskSubEffect *shape_mask,
                                           bool encrypt) {
  auto shape_json = item["shape"];
  if (!shape_json.isNull()) {
    ShapeType shape_type = ShapeType::NONE_SHAPE;
    auto shape_str = shape_json.asCString();
    if (strcmp(shape_str, "rectangle") == 0) {
      shape_type = ShapeType::RECTANGLE;
    } else if (strcmp(shape_str, "star") == 0) {
      shape_type = ShapeType::STAR;
    } else if (strcmp(shape_str, "round") == 0) {
      shape_type = ShapeType::ROUND;
    } else if (strcmp(shape_str, "heart") == 0) {
      shape_type = ShapeType::HEART;
    }
    shape_mask->SetShapeType(shape_type);

    if (shape_type == ShapeType::ROUND) {
      auto center_x_json = item["center_x"];
      auto center_y_json = item["center_y"];
      auto radius_json = item["radius"];
      if (!center_x_json.isNull() && !center_y_json.isNull() && !radius_json.isNull()) {
        shape_mask->SetRoundArgument(center_x_json.asFloat(), center_y_json.asFloat(), radius_json.asFloat());
      }
    } else if (shape_type == ShapeType::STAR) {
      auto center_x_json = item["center_x"];
      auto center_y_json = item["center_y"];
      auto r_min_json = item["r_min"];
      auto r_max_json = item["r_max"];
      if (!center_x_json.isNull() && !center_y_json.isNull() && !r_min_json.isNull() && !r_max_json.isNull()) {
        shape_mask->SetStarArgument(center_x_json.asFloat(), center_y_json.asFloat(), r_min_json.asFloat(), r_max_json.asFloat());
      }
    } else if (shape_type == ShapeType::HEART) {
      auto center_x_json = item["center_x"];
      auto center_y_json = item["center_y"];
      auto scale_x_json = item["scale_x"];
      auto scale_y_json = item["scale_y"];
      if (!center_x_json.isNull() && !center_y_json.isNull() && !scale_x_json.isNull() && !scale_y_json.isNull()) {
        shape_mask->SetHeartArgument(center_x_json.asFloat(), center_y_json.asFloat(), scale_x_json.asFloat(), scale_y_json.asFloat());
      }
    } else if (shape_type == ShapeType::RECTANGLE) {
      auto x_json = item["x"];
      auto y_json = item["y"];
      auto w_json = item["w"];
      auto h_json = item["h"];
      if (!x_json.isNull() && !y_json.isNull() && !w_json.isNull() && !h_json.isNull()) {
        shape_mask->SetRectArgument(x_json.asFloat(), y_json.asFloat(), w_json.asFloat(), h_json.asFloat());
      }
    }
  }
  auto background_image_path_json = item["background_path"];
  if (!background_image_path_json.isNull()) {
    shape_mask->SetBackgroundImagePath(background_image_path_json.asString());
  }
  return 0;
}

}