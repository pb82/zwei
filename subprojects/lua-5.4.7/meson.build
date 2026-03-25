project('lua', 'c', version: '5.4.7', default_options: ['c_std=c99'])

cc = meson.get_compiler('c')

# All core + lib sources, excluding lua.c (standalone interpreter) and luac.c (compiler)
lua_src = files(
  'src/lapi.c',
  'src/lauxlib.c',
  'src/lbaselib.c',
  'src/lcode.c',
  'src/lcorolib.c',
  'src/lctype.c',
  'src/ldblib.c',
  'src/ldebug.c',
  'src/ldo.c',
  'src/ldump.c',
  'src/lfunc.c',
  'src/lgc.c',
  'src/linit.c',
  'src/liolib.c',
  'src/llex.c',
  'src/lmathlib.c',
  'src/lmem.c',
  'src/loadlib.c',
  'src/lobject.c',
  'src/lopcodes.c',
  'src/loslib.c',
  'src/lparser.c',
  'src/lstate.c',
  'src/lstring.c',
  'src/lstrlib.c',
  'src/ltable.c',
  'src/ltablib.c',
  'src/ltm.c',
  'src/lundump.c',
  'src/lutf8lib.c',
  'src/lvm.c',
  'src/lzio.c',
)

lua_inc = include_directories('src')

lua_c_args = []
if host_machine.system() == 'linux'
  lua_c_args += '-DLUA_USE_LINUX'
elif host_machine.system() == 'darwin'
  lua_c_args += '-DLUA_USE_MACOSX'
endif

lua_lib = static_library('lua', lua_src,
  include_directories: lua_inc,
  c_args: lua_c_args,
)

m_dep = cc.find_library('m', required: false)
dl_dep = cc.find_library('dl', required: false)

lua_dep = declare_dependency(
  include_directories: lua_inc,
  link_with: lua_lib,
  dependencies: [m_dep, dl_dep],
)
