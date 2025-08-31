cmake_minimum_required(VERSION 3.15)
project(MinecraftClone)

set(CMAKE_CXX_STANDARD 17)

# vcpkg installed packages (use CONFIG mode!)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(main src/main.cpp)

# Link with correct vcpkg targets
target_link_libraries(main PRIVATE glfw glad::glad glm::glm)
