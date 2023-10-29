cmake_minimum_required(VERSION 3.13.0)
project(learn_llvm_pass)

set(CMAKE_CXX_STANDARD 17)
set(LLVM_INSTALL_PATH "/opt/homebrew/opt/llvm" CACHE PATH "llvm install path") 
set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
set(DCMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
list(APPEND CMAKE_PREFIX_PATH "${LLVM_INSTALL_PATH}/lib/cmake/llvm/")
find_package(LLVM REQUIRED CONFIG)
message(STATUS "${LLVM_INCLUDE_DIRS}")
add_library(HelloWorld SHARED HelloWorld.cpp)

target_include_directories(HelloWorld PUBLIC "${LLVM_INCLUDE_DIRS}")
target_link_libraries(HelloWorld "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>")