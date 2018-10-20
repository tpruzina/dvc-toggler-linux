
xext_dep = dependency('xext')
x11_dep = dependency('x11')
xmu_dep = dependency('xmu')

xdisplay_sources = [ 'xdisplay.cpp', 'test.cpp' ]
xdisplay = executable(
        'xdisplay_test',
        xdisplay_sources,
        dependencies: [ xext_dep, x11_dep, xmu_dep ]
)

test('xdisplay', xdisplay)

