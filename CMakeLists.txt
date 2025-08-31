cmake_minimum_required(VERSION 3.10)
project(MinecraftClone)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(MinecraftClone src/main.cpp)

target_link_libraries(MinecraftClone PRIVATE glfw glad::glad glm::glm)
