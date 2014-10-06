{
  'target_defaults': { 
    'type'         : 'executable',
    'include_dirs' : [ './deps/log', './src' ],
    'sources'      : [ './deps/log/log.h', './src/learnuv.h' ],
    'dependencies' : [ './deps/libuv/uv.gyp:libuv' ],
  },
  'targets': [ 
    { 'target_name': '01_system_info', 'sources': [ './src/01_system_info.c' ] },
    { 'target_name': '02_idle', 'sources': [ './src/02_idle.c' ] },
  ]
}
