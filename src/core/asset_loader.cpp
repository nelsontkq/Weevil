#include "asset_loader.h"

#include <SDL3_image/SDL_image.h>

struct FontDeleter {
  void operator()(TTF_Font* f) const { TTF_CloseFont(f); }
};
wv::AssetLoader::AssetLoader(const std::string& asset_path) : asset_path_(asset_path) {}

wv::AssetLoader::~AssetLoader() {
  text_textures_.clear();
  sprite_textures_.clear();
  fonts_.clear();
  surfaces_.clear();
}
void wv::AssetLoader::load_font(wv::LoadFont& font_details) {
  auto p = asset_path_ / font_details.asset_path;
  WV_ASSERT(std::filesystem::exists(p), std::format("Asset not found: {}", font_details.asset_path));
  auto it = fonts_.find(font_details.asset_name);
  std::shared_ptr<TTF_Font> font;
  if (it != fonts_.end()) {
    font = it->second;
  } else {
    font = std::shared_ptr<TTF_Font>(TTF_OpenFont(p.c_str(), font_details.font_size), FontDeleter());
    if (!font) {
      LOG_ERROR("Failed to load font: {}", SDL_GetError());
      return;
    }
    fonts_[font_details.asset_name] = font;
  }

  const auto& [_, success] = text_textures_.emplace(font_details.asset_name, std::make_unique<wv::TextTexture>(font));
  WV_ASSERT(success, std::format("Texture for font {} being reinitialized", font_details.asset_name));

  LOG_INFO("Font loaded: {}", font_details.asset_name);
}

wv::TextTexture* wv::AssetLoader::get_text(std::string_view asset_name) {
  auto it = text_textures_.find(asset_name);
  WV_ASSERT(it != text_textures_.end(), "Attempted to get an unloaded text!");
  return it->second.get();
}

void wv::AssetLoader::register_dispatch_events(entt::dispatcher& dispatcher) {
  dispatcher.sink<wv::LoadFont>().connect<&AssetLoader::load_font>(*this);
  dispatcher.sink<wv::UnloadFont>().connect<&AssetLoader::unload_font>(*this);
  dispatcher.sink<wv::LoadSpriteSheet>().connect<&AssetLoader::load_sprite_sheet>(*this);
  dispatcher.sink<wv::UnloadSpriteSheet>().connect<&AssetLoader::unload_sprite_sheet>(*this);
}

void wv::AssetLoader::unload_font(wv::UnloadFont& asset) {
  auto it = text_textures_.find(asset.asset_name);
  if (it == text_textures_.end()) {
    return;
  }
  text_textures_.erase(it);
  auto font_it = fonts_.find(asset.asset_name);
  if (font_it->second.use_count() == 1) {
    fonts_.erase(font_it);
  }
}

wv::SpriteTexture* wv::AssetLoader::get_sprite(std::string_view asset_name) {
  auto it = sprite_textures_.find(asset_name);
  if (it == sprite_textures_.end()) {
    LOG_ERROR("Unable to load sprite!");
    return nullptr;
  }
  return it->second.get();
}
void wv::AssetLoader::load_sprite_sheet(wv::LoadSpriteSheet& asset) {
  auto p = asset_path_ / asset.asset_path;
  WV_ASSERT(std::filesystem::exists(p), std::format("Asset not found: {}", asset.asset_path));

  auto surface_it = surfaces_.find(asset.asset_name);
  std::shared_ptr<SDL_Surface> surface;

  if (surface_it != surfaces_.end()) {
    surface = surface_it->second;
  } else {
    surface = std::shared_ptr<SDL_Surface>(IMG_Load(p.c_str()), wv::SDL_Deleter());
    if (!surface) {
      LOG_ERROR("Failed to load sprite sheet: {}", SDL_GetError());
      return;
    }
    surfaces_[asset.asset_name] = surface;
  }

  auto sprite_texture = std::make_unique<wv::SpriteTexture>(surface);

  // Add all the frames from the sprite sheet
  for (const auto& frame : asset.sprites) {
    sprite_texture->add_frame(frame);
  }

  sprite_textures_[asset.asset_name] = std::move(sprite_texture);
  LOG_INFO("Sprite sheet loaded: {}", asset.asset_name);
}

void wv::AssetLoader::unload_sprite_sheet(wv::UnloadSpriteSheet& asset) {
  auto it = sprite_textures_.find(asset.asset_name);
  if (it == sprite_textures_.end()) {
    return;
  }
  sprite_textures_.erase(it);

  // Only remove the surface if no other textures are using it
  auto surface_it = surfaces_.find(asset.asset_name);
  if (surface_it != surfaces_.end() && surface_it->second.use_count() == 1) {
    surfaces_.erase(surface_it);
  }

  LOG_INFO("Sprite sheet unloaded: {}", asset.asset_name);
}
