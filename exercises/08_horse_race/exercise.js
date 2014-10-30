'use strict';

 var expected = [  
    'Horse piscisaureus finished in place 1'
  , 'Horse indutny      finished in place 2'
  , 'Horse bnoordhuis   finished in place 3'
  , 'Horse ry           finished in place 4'
  , 'Horse saghul       finished in place 5'
 ].join('\n')

function printExpected() {
  console.error('Expected something similar to:')
  console.error(expected);
}

function checkFinishes(expected, actual) {
  // ignoring expected here but keeping it checked in to serve as an example
  var lines = actual.split('\n').filter(function (x) { return x.trim().length });

  if (lines.length < 5) {
    console.error('Expected 5 horses to finish the race, you reported %d', lines.length);
    printExpected();
    return true;
  }

  for (var i = 0; i < lines.length; i++) {
    var line = lines[i]
    if ((/^Horse +(piscisaureus|indutny|bnoordhuis|ry|saghul) +finished in place \d/)
        .test(line)
    ) continue;

    printExpected();
    console.error('"%s" looks totally different.', line)
    return true;
  }
  return false
}

var exercise = require('workshopper-exercise')()
  , checkFileContent = require('../../workshopper/check-file-content')

exercise.detectDiff = checkFinishes;
exercise.requireSubmission = false;
exercise.addSetup(checkFileContent.bind(null, exercise));  

module.exports = exercise;
