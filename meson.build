project('toy-c', 'c')

incdir = include_directories('include')
sources = ['src/main.c', 'src/io.c', 'src/sparseSet.c']

glfwdep = dependency('glfw3')
glewdep = dependency('glew')
gldep = dependency('opengl')

executable('toy-c', sources, include_directories : incdir, dependencies: [glfwdep, glewdep, gldep])
