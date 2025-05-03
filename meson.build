project('GLESFunctions', 'c')

inc_root = include_directories('include')
inc = include_directories('include')

dep_glfw = dependency('glfw3', required: true)
dep_opengl = dependency('opengl', required: true)

source_files = files(
  'main.c',
  'src/examples/simpleTriangle.c'
)

program = executable('example',
  source_files,
  include_directories: [inc, inc_root],
  dependencies: [
    dep_glfw,
    dep_opengl
  ]
)