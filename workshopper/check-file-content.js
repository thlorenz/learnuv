'use strict';

var path = require('path')
  , fs   = require('fs')
  , diff = require('diff')

function logDiff(d) {
  var differs = false;
  d.forEach(function (part) {
    var color = part.added 
      ? 'green'   
      : part.removed
        ? 'red'
        : 'grey'
    process.stderr.write(part.value[color])

    if (part.added || part.removed) differs = true;
  })
  return differs;
}

function detectDiff(expected, actual) {
  var d = diff.diffLines(expected, actual) 
  return logDiff(d)
}

function diffFiles(expectedFile, actualFile, diffFn, cb) {
  fs.readFile(expectedFile, 'utf8', function onexpected(err, expected) {
    if (err) return cb(err);
    fs.readFile(actualFile, 'utf8', function onactual(err, actual) {
      if (err) return cb(err);
      var differs = diffFn(expected, actual)
      if (differs) return cb(new Error('Output not as expected.'))
      cb()
    })  
  })
}

module.exports = function checkFileContent(exercise, mode, cb) {
  if (mode !== 'verify') cb(new Error('You can only verify your excercise. Run it directly on the terminal or via Xcode.'));

  var dir      = exercise.workshopper.dataDir
    , filename = exercise.id + '.c'
    , fullPath = path.join(dir, filename)

  fs.exists(fullPath, function (itdoes) {
    if (!itdoes) return cb(
      new Error('It looks like you did not run the current exercise yet and/or forgot to write a report.\n                  ' + 
                'learnuv could not find a report file for ' + exercise.name + '.')
    )
    diffFiles(path.join(exercise.dir, 'expected.txt'), fullPath, exercise.detectDiff || detectDiff, cb); 
  })
}
