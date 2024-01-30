@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

:: Navigate to the third_party directory or create it if it doesn't exist
if not exist "third_party" mkdir third_party
cd third_party

:: Cloning and installing Emscripten
echo Cloning the Emscripten SDK...
if not exist "emsdk" git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
echo Installing and activating the latest version of Emscripten...
call emsdk.bat install latest
call emsdk.bat activate latest
call emsdk_env.bat
cd ..

:: Cloning and building static libraries for Box2D
if not exist "box2d" git clone https://github.com/erincatto/box2d.git
cd box2d
mkdir build
cd build
call emcmake cmake -DCMAKE_BUILD_TYPE=Release -DBOX2D_BUILD_DOCS=OFF -DBOX2D_BUILD_EXAMPLES=OFF -DBOX2D_BUILD_TESTBED=OFF -DCMAKE_TOOLCHAIN_FILE=%CD%../../../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake ..
call cmake --build . -- -j4
cd ../..

:: Cloning and building static libraries for Box2D
if not exist "navmesh" git clone https://github.com/ilyanikolaevsky/navmesh.git
cd navmesh
mkdir build
cd build
call emcmake cmake -DCMAKE_BUILD_TYPE=Release -DNAVMESH_BUILD_TESTS=OFF -DNAVMESH_BUILD_DOCS=OFF -DNAVMESH_BUILD_EXAMPLES=OFF -DNAVMESH_BUILD_TESTBED=OFF -DCMAKE_TOOLCHAIN_FILE=%CD%../../../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake ..
call cmake --build . -- -j4
cd ../..

:: Cloning header-only libraries
if not exist "entt" git clone https://github.com/skypjack/entt.git
if not exist "glm" git clone https://github.com/g-truc/glm.git

cd ..

echo Dependencies have been successfully installed in the third_party directory!
pause
