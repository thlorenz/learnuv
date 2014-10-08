'use strict';
var exercise = require('workshopper-exercise')()
  , checkFileContent = require('../../workshopper/check-file-content')

function checkUptimeRSS(expected, actual) {
  // ignoring expected here but keeping it checked in to serve as an example
  var lines = actual.split('\n')
  var l1 = /Uptime: \d+\.\d+/.test(lines[0])
    , l2 = /RSS: \d+/.test(lines[1])

  if (l1 && l2) return false;

  console.error('\nExpected something similar to:')
  console.error(expected)

  console.error('Your report contained:')
  console.error(actual)

  if (!l1 && !l2) console.error('Neither lines match')
  else if (!l1) console.error('Line 1 does not match, although Line 2 does.')
  else console.error('Line 1 does match, but Line 2 does not')
  return true
}

exercise.detectDiff = checkUptimeRSS;
exercise.requireSubmission = false;
exercise.addSetup(checkFileContent.bind(null, exercise));  

module.exports = exercise;
