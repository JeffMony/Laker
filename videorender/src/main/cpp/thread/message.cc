//
// Created by jeffli on 2022/1/13.
//

#include "message.h"

#include "log.h"

namespace thread {

Message::Message()
  : what(-1)
  , arg1(-1)
  , arg2(-1)
  , arg3(-1)
  , arg4(-1)
  , arg5(-1)
  , arg6(-1)
  , arg7(-1)
  , arg8(0.0f)
  , arg9(0.0f)
  , flag1(false)
  , obj1(nullptr)
  , obj2(nullptr)
  , str1()
  , str2()
  , target(nullptr) {

}

Message::~Message() {
  /**
   * obj1
   * obj2
   * target
   * 不应该在Message析构函数中销毁, 应该由开发者决定是否销毁
   */
}

}