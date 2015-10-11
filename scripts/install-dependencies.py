#!/usr/bin/env python

import os

script_dir  = os.path.dirname(__file__)
root_dir    = os.path.join(os.path.abspath(script_dir), '..')
deps_dir    = os.path.join(root_dir, 'deps')
libuv_dir   = os.path.join(deps_dir, 'libuv')
logh_dir    = os.path.join(deps_dir, 'log')
build_dir   = os.path.join(root_dir, 'build')
gyp_dir     = os.path.join(build_dir, 'gyp')
home        = os.path.expanduser('~')
dot_learnuv = os.path.join(home, '.learnuv')

def log_info(msg):
    print '\033[0;32mlearnuv\033[0m ' + msg

def mkdirp(dir):
    try:
        os.mkdir(dir)
    except OSError:
        pass

def run(cmd):
    log_info('executing: ' + cmd)
    os.system(cmd)

# libuv
mkdirp(deps_dir)
## todo: This may not work with older git versions (http://stackoverflow.com/questions/20280726/how-to-git-clone-a-specific-tag)
run('git clone --depth 1 --branch v1.7.5 https://github.com/libuv/libuv ' + libuv_dir)

# gyp
mkdirp(build_dir)
run('git clone https://chromium.googlesource.com/external/gyp ' + gyp_dir)

# log.h
run ('git clone --depth 1 https://github.com/thlorenz/log.h ' + logh_dir) 

# create ~/.learnuv
log_info('Creating directory to store results at ' + dot_learnuv)
mkdirp(dot_learnuv)
