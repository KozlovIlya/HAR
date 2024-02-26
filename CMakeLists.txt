cmake_minimum_required(VERSION 3.28)

project(HAR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/third_party/entt/src)
include_directories(${PROJECT_SOURCE_DIR}/third_party/glm)
include_directories(${PROJECT_SOURCE_DIR}/third_party/navmesh/source)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/third_party/navmesh/build/lib/libgmock.a")
target_link_libraries(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/third_party/navmesh/build/source/libnavmesh.a")

set(CMAKE_EXECUTABLE_SUFFIX ".html")

if (EMSCRIPTEN)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif ()
endif ()

# Установка флагов компиляции и линковки для использования WebGL и DWARF отладочной информации
set_target_properties(${PROJECT_NAME} PROPERTIES
  LINK_FLAGS "--shell-file ${CMAKE_SOURCE_DIR}/res/shell.html -s USE_WEBGL2=1 -s FULL_ES2=1 -gsource-map -g -s ASSERTIONS=1 -s SAFE_HEAP=1"
)
