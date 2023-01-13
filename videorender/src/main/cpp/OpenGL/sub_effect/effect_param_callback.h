

#ifndef OPENGL_SUB_EFFECT_EFFECT_PARAM_CALLBACK_H_
#define OPENGL_SUB_EFFECT_EFFECT_PARAM_CALLBACK_H_

#include <string>
#include <map>

namespace effect {

class EffectParamCallback {
 public:
  virtual void EffectConfigParam(int effect_id, int type, int start_time, int end_time,
                                 std::string& effect_name, std::string& param_name, std::map<std::string, std::string>& extra) = 0;

  virtual void EffectLocationParam(const std::string& param) {}
};

}  // namespace effect

#endif  // OPENGL_SUB_EFFECT_EFFECT_PARAM_CALLBACK_H_