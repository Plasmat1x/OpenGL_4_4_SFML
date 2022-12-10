project('ogl', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

sfml_deps = [dependency('sfml-window'), dependency('sfml-system')]

GL_deps = [
  dependency('GL'),
  dependency('glew'),
  dependency('glm')
  ]

sys_deps = [
            #dependency('X11'),
            dependency('threads')
            #dependency('Xrandr'),
            #dependency('Xi'),
            #dependency('dl')
            ]

subdir('src')


