
qt5 = import('qt5')
qt5_dep = dependency('qt5', modules : [ 'Core', 'Gui', 'Widgets' ])

sources = [ 'run_guard.cpp', 'test.cpp' ]
proc_watch = executable(
        'proc_watch_test',
        sources,
        dependencies: [ qt5_dep ]
)

test('proc_watch', proc_watch)

