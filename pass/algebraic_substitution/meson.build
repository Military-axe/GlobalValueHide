src_files = files(
  'substitutionChoose.cc',
  'sub1.cc',
)

llvm_dep = get_variable('llvm_dep')

substitution_lib = static_library(
  'substitution_lib',
  sources: src_files,
  dependencies: [llvm_dep, utils_dep],
  include_directories: include_directories('.'),
)

substitution_dep = declare_dependency(
  include_directories: include_directories('..'),
  link_with: substitution_lib
)