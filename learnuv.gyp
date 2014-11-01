{
  'target_defaults': { 
    'conditions': [
      ['OS != "win"', {
        'conditions': [
          ['OS=="solaris"', {
            'cflags': [ '-pthreads' ],
          }],
          ['OS not in "solaris android"', {
            'cflags': [ '-pthread' ],
          }],
        ],
      }],
    ],
    'xcode_settings': {
      'MACOSX_DEPLOYMENT_TARGET': '10.7',
    },
    'type'         : 'executable',
    'include_dirs' : [ './deps/log', './src', './deps/libuv/test' ],
    'sources'      : [ './deps/log/log.h', './src/learnuv.h' ],
    'dependencies' : [ './deps/libuv/uv.gyp:libuv' ],
    'defines'      : [ 
      '__ROOT__="<(root)"',
      '__MAGIC_FILE__="<(magic_file)"',
      '__HOME__="<(home)"',
      '__LEARNUV_CONFIG__="<(learnuv_config)"'
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
        ['OS in "freebsd openbsd solaris android linux mac"', {
          'ldflags': [ '-lncurses' ],
          'cflags': [ '--std=gnu99' ],
        }],
        ['OS in "linux"', {
          'libraries': [ '-lncurses' ]
        }]
      ],
      'xcode_settings': {
        'OTHER_LDFLAGS': [
          '-lncurses'
        ],
      }
    },
    { 'target_name': 'interactive_horse_race', 
      'include_dirs': [ './src/interactive_horse_race/' ],
      'sources': [ 
        './src/interactive_horse_race/interactive_horse_race.h',
        './src/interactive_horse_race/interactive_horse_race.c',
        './src/interactive_horse_race/tcp_server.c',
        './src/interactive_horse_race/track.c',
        './src/interactive_horse_race/questions.c',
      ],
      'conditions': [ 
        ['OS in "freebsd openbsd solaris android linux mac"', {
          'ldflags': [ '-lncurses' ],
          'cflags': [ '--std=gnu99' ],
        }],
        ['OS in "linux"', {
          'libraries': [ '-lncurses' ]
        }]
      ],
      'xcode_settings': {
        'OTHER_LDFLAGS': [
          '-lncurses'
        ],
      }
    },
  ]
}
