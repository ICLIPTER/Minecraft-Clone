# Minecraft Clone

A simple 3D Minecraft-inspired voxel game built from scratch using C++ and OpenGL.

---

## Features

- **Voxel-based world**: Procedurally generated terrain made of cubes.
- **Basic player movement**: Walk, jump, and look around in first-person view.
- **Block manipulation**: Place and destroy blocks in the world.
- **Simple lighting**: Dynamic lighting to enhance the 3D experience.
- **Build system**: Uses CMake for easy cross-platform building.
- **Dependencies managed with `vcpkg`** for easy setup.

---

## Screenshots

*Add some cool screenshots here!*

---

## Getting Started

### Prerequisites

- C++17 compatible compiler
- [CMake](https://cmake.org/) (version 3.15 or higher)
- [vcpkg](https://github.com/microsoft/vcpkg) (for managing dependencies)

### Build Instructions

1. Clone the repository (with submodules):

```bash
git clone --recurse-submodules https://github.com/ICLIPTER/Minecraft-Clone.git
cd Minecraft-Clone

./vcpkg/bootstrap-vcpkg.sh   # Linux/macOS
.\vcpkg\bootstrap-vcpkg.bat  # Windows
./vcpkg/vcpkg install glfw3 glm

cmake -S . -B build
cmake --build build --config Release

./build/MinecraftClone.exe  # Or the equivalent on your OS

```

## Controls

| Action         | Key/Mouse          |
|----------------|--------------------|
| Move forward   | W                  |
| Move backward  | S                  |
| Move left      | A                  |
| Move right     | D                  |
| Jump           | Space              |
| Place block    | Left mouse button  |
| Destroy block  | Right mouse button |
| Look around    | Mouse movement     |




---

Would you like me to help you tailor this further — like adding more technical details, roadmap, or setup instructions?


