# pass/CMakeLists.txt
# 代数替换子模块
add_subdirectory(algebraic_substitution)

set(SRC_FILES
  collector.cc
  encryptor.cc
  gv_hide.cc
  replacer.cc
  pass.cc
)

add_library(gvHide SHARED ${SRC_FILES})

target_include_directories(gvHide PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(gvHide
  PRIVATE
  utils
  substitution_lib
  ${LLVM_LIBRARIES}
)
