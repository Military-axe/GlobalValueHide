add_subdirectory(sub1)
add_subdirectory(sub2)

add_library(substitution_lib STATIC
  substitutionChoose.cc
)

set_target_properties(substitution_lib PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)

target_include_directories(substitution_lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LLVM_INCLUDE_DIRS}
)

target_link_libraries(substitution_lib PRIVATE
  sub1_lib
  sub2_lib
  utils
  ${LLVM_LIBRARIES}
)

target_compile_options(substitution_lib PRIVATE
  -Wno-unused-parameter
)