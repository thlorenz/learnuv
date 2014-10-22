{
  'target_defaults': { 
    'type'         : 'executable',
    'include_dirs' : [ './deps/log', './src' ],
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
  ]
}
