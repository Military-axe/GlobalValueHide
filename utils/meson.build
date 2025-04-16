utils_src = ['utils.cc']

utils_lib = static_library(
    'utils',
    sources: utils_src,
    include_directories: include_directories('.')
)

utils_dep = declare_dependency(
    include_directories: include_directories('..'),
    link_with: utils_lib
)
    