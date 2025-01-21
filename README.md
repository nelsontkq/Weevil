# Weevil Engine

## Project Overview
Weevil is a game engine designed to provide a robust and flexible framework for game development. It offers a range of functionalities to manage game systems, assets, and configurations, making it easier for developers to create and manage game projects.

## Features
- **System Management**: Efficiently manage and update game systems.
- **Asset Management**: Load and unload game assets dynamically.
- **Configuration Management**: Easily configure game settings using TOML files.
- **Logging**: Integrated logging system for debugging and information tracking.

## Prerequisites
- C++20 compatible compiler
- CMake 3.27 or higher
- [SDL3](https://github.com/libsdl-org/SDL)
- [SDL_image](https://github.com/libsdl-org/SDL_image)
- [EnTT](https://github.com/skypjack/entt)
- [spdlog](https://github.com/gabime/spdlog)
- [toml++](https://github.com/marzer/tomlplusplus)

## Building the Project
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd Weevil
   ```
2. Configure and build the project using CMake:
   - Run `cmake --preset default` for a debug build.
   - Run `cmake --build build/default` to compile.

## Running the Project
- Execute the compiled binary located in the `build/default` directory.
- Ensure the `weevil.toml` configuration file is present in the `config/` directory.

## Configuration
- The `weevil.toml` file is used to configure application settings such as window title, width, height, fullscreen, and resizable options.

## Directory Structure
- `src/`: Source code files.
- `include/`: Header files.
- `assets/`: Assets such as fonts and textures.
- `config/`: Configuration files.
- `build/`: Build outputs.

## Usage Example
- Once the application is running, you can interact with the game engine through the provided interfaces and configurations.

## Contributing
- Contributions are welcome! Please follow the coding standards and submit pull requests with clear descriptions of changes.

## License
- This project is licensed under the MIT License. See the LICENSE file for details.

## Contact Information
- For issues or questions, please contact the project maintainers at [contact@example.com].
