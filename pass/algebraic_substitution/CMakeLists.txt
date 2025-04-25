# pass/algebraic_substitution/CMakeLists.txt
add_library(substitution_lib STATIC
  sub1.cc
  substitutionChoose.cc
)

set_target_properties(substitution_lib PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)

target_include_directories(substitution_lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LLVM_INCLUDE_DIRS}
)

target_link_libraries(substitution_lib PUBLIC
  utils
  ${LLVM_LIBRARIES}
)

target_compile_options(substitution_lib PRIVATE
  -Wno-unused-parameter
)