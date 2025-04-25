cmake_minimum_required(VERSION 3.20)
project(GlobalValueHide
  LANGUAGES CXX
  VERSION 0.1
)

set(CMAKE_CXX_COMPILER "clang++-19" CACHE STRING "C++ compiler" FORCE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# LLVM configure
find_package(LLVM 19.1.7 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM include: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM library: ${LLVM_LIBRARY_DIR}")

include_directories(
  ${CMAKE_SOURCE_DIR}
)

add_subdirectory(utils)
add_subdirectory(pass)

add_compile_options(
  -fPIC
  -fno-rtti
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)