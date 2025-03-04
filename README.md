# Weevil Game Engine

Weevil is a modular, hot-reloadable game engine built with C++ and SDL, designed for rapid development and prototyping of 2D games.

## Features

- **Hot Module Reloading**: Make changes to your game code without restarting the application
- **Modular Architecture**: Separate gameplay mechanics into independent modules
- **Entity Component System**: Built on [EnTT](https://github.com/skypjack/entt) for efficient entity management
- **SDL Integration**: Hardware-accelerated rendering with support for adaptive VSync
- **TOML Configuration**: Simple, human-readable configuration files
- **Logging**: Configurable logging levels for debugging
- **RNG**: Easy-to-use RNG utilities for gameplay elements

## Getting Started

### Prerequisites

- CMake 3.30 or higher
- SDL3
- A C++20 compatible compiler (GCC, Clang)
- [toml++](https://github.com/marzer/tomlplusplus) library

### Building

1. Clone the repository
2. Configure the build using CMake presets:

```bash
cmake --preset linux-debug
```

3. Build the project:

```bash
cmake --build --preset linux-debug
```

### Configuration

The engine is configured through the `app_settings.toml` file, which controls window settings, file paths, and other important parameters:

```toml
[window]
title = "Weevil"
width = 1920
height = 1080
resizable = false
fullscreen = false

[general]
src_dir = "/path/to/your/project" # Path to your Weevil project directory
debug_preset = "linux-debug"      # CMake preset for hot reloading
```

## Creating Modules

Modules are the primary building blocks of your game. Each module has its own entity registry and lifecycle:

1. Create a new directory in the `modules` folder for your module
2. Create any amount of source files in the directory, but one of the sources must define one WV_MODULE
3. Implement your module class:

```cpp
#include <weevil/weevil_api.h>

class MyGameModule : public wv::IModule {
 public:
  void init(entt::dispatcher& dispatcher) override {
    // Initialize your module
  }
  
  void update(entt::dispatcher& dispatcher, float dt) override {
    // Update logic here - runs every frame
  }

  void shutdown(entt::dispatcher& dispatcher) override {
    // Cleanup resources
  }
};

// Register your module with the engine
WV_MODULE(MyGameModule)
```

### Component System

Weevil provides basic components for commonly used game objects:

- `wv::Rectangle`: Basic shape component
- `wv::Transform`: Position and size information
- `wv::Color`: RGBA color data

You can create entities and add components to them within your modules:

```cpp
auto entity = registry.create();
registry.emplace<wv::Rectangle>(entity);
registry.emplace<wv::Transform>(entity, x, y, width, height);
registry.emplace<wv::Color>(entity, r, g, b, a);
```

## Hot Reloading

The hot reloading system automatically monitors changes to your module source files and rebuilds the affected modules while the game is running. Changes are detected using Linux's inotify system, and modules are rebuilt using the CMake preset specified in your configuration file.

This allows for rapid iteration on your game without needing to restart the application.

## Asset Management

Assets should be placed in the appropriate directories under the `assets` folder:

- `assets/fonts`: Font files (TTF)
- `assets/music`: Background music tracks
- `assets/sounds`: Sound effects
- `assets/textures`: Image files
- `assets/ui`: User interface elements
- `assets/shaders`: Shader programs

## Architecture Overview

Weevil uses a modular architecture where:

1. `Application` manages the main game loop and coordinates other systems
2. `ModuleManager` loads and manages game modules
3. `Platform` handles rendering and platform-specific operations
4. Each `Module` contains game logic in self-contained units
5. `HotReloader` monitors file changes and triggers rebuilds

The event system uses EnTT's dispatcher to communicate between components.
