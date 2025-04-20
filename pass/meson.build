src_files = ['collector.cc', 'encryptor.cc', 'gv_hide.cc', 'replacer.cc', 'pass.cc']

llvm_dep = dependency('llvm', version: '>=19.0')

subdir('algebraic_substitution')

shared_library(
    'gvHide',
    sources : src_files,
    dependencies : [llvm_dep, utils_dep, substitution_dep],
    cpp_args : ['-std=c++20'],
    include_directories: include_directories('.'),
)