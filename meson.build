project('GLESFunctions', 'c', version: '0.0.1')
version = '0.0.1'
app_name = 'GLESFunctions'
config_data = configuration_data()
config_data.set('version', version)

is_windows = host_machine.system() == 'windows'
is_darwin = host_machine.system() == 'darwin'
is_linux = host_machine.system() == 'linux'

# Define the resource directory based on the platform
if is_windows
  localappdata = run_command('cmd', '/c', 'echo', '%LOCALAPPDATA%').stdout().strip()
  resources_path = join_paths(localappdata, app_name)
elif is_darwin
  home = run_command('sh', '-c', 'echo $HOME').stdout().strip()
  resources_path = join_paths(home, 'Library', 'Application Support', app_name)
else
  xdg_data_home = run_command('sh', '-c', 'echo ${XDG_DATA_HOME:-$HOME/.local/share}').stdout().strip()
  resources_path = join_paths(xdg_data_home, app_name)
endif

resources_dir = join_paths(resources_path, 'resources')

#TODO: add windows and macos commands here
resources_target = custom_target('resources',
  output: 'resources',
  command: [
    'sh', '-c', 
    'rm -rf "@0@" && mkdir -p "@0@" && cp -r "@1@" "@0@"'.format(
      resources_path, 
      meson.current_source_dir() / 'resources'
    )
  ],
  build_by_default: true
)

# Create the resource directory if it doesn't exist
install_subdir('resources', install_dir: resources_path)


config_data.set('resources_dir', resources_dir)

configure_file(
    input: 'config.h.in',
    output: 'config.h',
    configuration: config_data
)

inc_root = include_directories('.')
inc = include_directories('include')

dep_glfw = dependency('glfw3', required: true)
dep_opengl = dependency('opengl', required: true)
dep_png = dependency('libpng', required: true)

source_files = files(
  'main.c',
  'src/examples/simpleTriangle.c',
  'src/examples/activeTextureExample.c',
  'src/utils/programUtils.c',
  'src/utils/resourceUtils.c'
)

program = executable('example',
  source_files,
  include_directories: [inc, inc_root],
  dependencies: [
    dep_glfw,
    dep_opengl,
    dep_png
  ]
)