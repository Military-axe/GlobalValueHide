# utils/CMakeLists.txt
add_library(utils STATIC
  utils.cc
)

target_include_directories(utils PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

if(LLVM_FOUND)
  target_include_directories(utils SYSTEM PUBLIC
    ${LLVM_INCLUDE_DIRS}
  )
endif()