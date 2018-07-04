qt5 = import('qt5')
qt5_dep = dependency('qt5', modules : [ 'Core', 'Gui', 'Widgets' ])

qt_preproc = qt5.preprocess(
  qresources: 'dvc_toggler_linux.qrc',
  moc_headers: [ 'main_window.hpp','profile_selector.hpp' ])

thread_dep = dependency('threads')
x11_dep = dependency('x11')
xext_dep = dependency('xext')
xmu_dep = dependency('xmu')

sources = [ 'config.cpp','main.cpp','main_window.cpp','proc_watch.cpp','profile_selector.cpp','run_guard.cpp','nvidia/display.cpp','nvidia/nvidia.cpp', 'nvidia/libXNVCtrl/NVCtrl.c']

executable('dvc-toggler-linux',
  [ sources , qt_preproc ], 
  dependencies : [qt5_dep, thread_dep, x11_dep, xext_dep, xmu_dep])
