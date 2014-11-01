#!/usr/bin/env node

var pck = require(process.argv[2]);

var deps = Object.keys(pck.dependencies)
pck.bundledDependencies = deps; 
pck.dependencies = {};
pck.scripts.preinstall = {}; // dependencies are now already installed
pck.scripts.postinstall = 'mv binding.gyp learnuv.gyp'; // npm renames our gyp file on install

require('fs').writeFileSync(process.argv[2], JSON.stringify(pck, null, 2), 'utf8')
