{
  'target_defaults': { 
    'type'         : 'executable',
    'include_dirs' : [ './deps/log', './src', './deps/libuv/test' ],
    'sources'      : [ './deps/log/log.h', './src/learnuv.h' ],
    'dependencies' : [ './deps/libuv/uv.gyp:libuv' ],
    'defines'      : [ 
      '__ROOT__="<(root)"',
      '__MAGIC_FILE__="<(magic_file)"'
    ]
  },
  'targets': [ 
    { 'target_name': '01_system_info'          , 'sources': [ './src/01_system_info.c' ] }          ,
    { 'target_name': '02_idle'                 , 'sources': [ './src/02_idle.c' ] }                 ,
    { 'target_name': '03_fs_readsync'          , 'sources': [ './src/03_fs_readsync.c' ] }          ,
    { 'target_name': '04_fs_readasync'         , 'sources': [ './src/04_fs_readasync.c' ] }         ,
    { 'target_name': '05_fs_readasync_context' , 'sources': [ './src/05_fs_readasync_context.c' ] } ,
    { 'target_name': '06_fs_allasync'          , 'sources': [ './src/06_fs_allasync.c' ] }          ,
    { 'target_name': '07_tcp_echo_server'      , 'sources': [ './src/07_tcp_echo_server.c' ] }      ,
    { 'target_name': '08_horse_race',
      'sources': [ './src/08_horse_race.c' ],
      'conditions': [ 
        ['OS=="linux"', {
          'cflags': [ '-pthread, -lncurses' ],
          'ldflags': [ '-pthread, -lncurses' ],
        }],
        # TODO: why do these flags not get applied? For now we are fixing it in package.json by setting env var LDFLAGS
        ['OS!="linux"', {
          'cflags': [ '-lncurses' ],
          'ldflags': [ '-lncurses' ],
        }],
        ]
    }
  ]
}
