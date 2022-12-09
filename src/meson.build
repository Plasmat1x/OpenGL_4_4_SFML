ogl_src = [
    'ogl_main.cpp',
]


exe = executable('ogl', 
    ogl_src,
    dependencies: [sfml_deps,GL_deps,sys_deps],
    install : true)

test('basic', exe)