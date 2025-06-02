add_library(sub2_lib STATIC
  sub2.cc
)

target_link_libraries(sub2_lib PRIVATE
  utils
  ${LLVM_LIBRARIES}
)