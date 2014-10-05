{
  'target_defaults': { 
    'include_dirs': [ './deps/log', './src' ],
    'sources': [ './deps/log/log.h', './src/learnuv.h' ],
  },
  'targets': [
    {
      'target_name': 'learnuv_ex01',
      'type': 'executable',
      'dependencies': [ './deps/libuv/uv.gyp:libuv' ], 
      'include_dirs': [ './src/ex01' ],
      'sources' : [ './src/ex01/ex01_main.c' ]
    }
  ]
}
