cmake_minimum_required(VERSION 3.15)
project(HAR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/third_party/box2d/include)
include_directories(${PROJECT_SOURCE_DIR}/third_party/entt/src)
include_directories(${PROJECT_SOURCE_DIR}/third_party/glm)
include_directories(${PROJECT_SOURCE_DIR}/third_party/navmesh/source)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/third_party/box2d/build/bin/libbox2d.a")
target_link_libraries(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/third_party/navmesh/build/lib/libgmock.a")
target_link_libraries(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/third_party/navmesh/build/source/libnavmesh.a")


set(CMAKE_EXECUTABLE_SUFFIX ".html")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--shell-file ${CMAKE_SOURCE_DIR}/res/shell.html")
