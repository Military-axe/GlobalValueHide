add_library(sub1_lib STATIC
  sub1.cc
)

target_link_libraries(sub1_lib PRIVATE
  utils
  ${LLVM_LIBRARIES}
)