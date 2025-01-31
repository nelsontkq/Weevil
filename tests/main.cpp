#include "WeevilEngine/Components.h"
#include "WeevilEngine/AppSettings.h"
#include "WeevilEngine/Game.h"


auto main() -> int {
  wv::Log::Init();
  wv::AppSettings settings("/home/nelson/Development/Weevil/config/weevil.toml");
  wv::Game game(settings);

  game.run();
  return 0;
}
