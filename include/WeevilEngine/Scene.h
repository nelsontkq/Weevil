#pragma once
#include "weevil.h"
namespace wv{
class Scene {
 public:
  virtual ~Scene() = default;

  virtual void on_enter() {}
  virtual void on_exit() {}
};
}