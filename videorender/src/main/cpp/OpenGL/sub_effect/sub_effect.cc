
#include "sub_effect.h"

namespace effect {

SubEffect::SubEffect()
  : z_order(0)
  , start_time(0)
  , end_time(INT32_MAX)
  , enable(true)
  , duration(0)
  , effect_id(0)
  , effect_param_callback_(nullptr) {

}

SubEffect::~SubEffect() {

}

}