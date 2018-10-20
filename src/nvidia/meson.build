
nvidia_inc = include_directories('libXNVCtrl/')
nvidia_flags = [ '-nostartfiles' ]
nvidia_lib = static_library('libXNVCtrl', [ 'libXNVCtrl/NVCtrl.c' ])
nvidia_dep = declare_dependency(
        link_with : [ nvidia_lib ],
        include_directories : nvidia_inc
)


xext_dep = dependency('xext')
x11_dep = dependency('x11')
cc = meson.get_compiler('c')

nv_sources = [ 'nvidia.cpp', 'test.cpp' ]
nv = executable(
        'nvidia-test', 
        nv_sources,
        dependencies: [ nvidia_dep, xext_dep, x11_dep],
)

test('nvidia', nv)

