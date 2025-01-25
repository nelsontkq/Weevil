#include "WeevilEngine/Components.h"
#include "WeevilEngine/Game.h"
#include "brotato/player.h"

auto main() -> int {
  wv::Log::Init();
  wv::AppSettings settings("/home/nelson/Development/Weevil/config/weevil.toml");
  wv::Game game(settings);
//  game.system_manager().add_system<PlayerSystem>();
//  game.run();
  return 0;
}
