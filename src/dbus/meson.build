dbus1_dep = dependency('dbus-1') 
thread_dep = dependency('threads')

dbus_sources = [ 'dbus_watch.cpp', 'test.cpp' ]
dbus = executable(
        'dbus_test',
        dbus_sources,
        dependencies: [ dbus1_dep, thread_dep ]
)

test('dbus', dbus)

