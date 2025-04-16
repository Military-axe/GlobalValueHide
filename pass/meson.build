src_files = files('collector.cc', 'encryptor.cc', 'gv_hide.cc', 'replacer.cc')

llvm_dep = dependency('llvm', version: '>=19.0')

shared_library(
    'gvHide',
    sources : src_files,
    dependencies : [llvm_dep, utils_dep],
    cpp_args : ['-std=c++20'],
    include_directories: include_directories('.'),
)