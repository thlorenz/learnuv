'use strict';

var exercise = require('workshopper-exercise')()

function checkFun(mode, cb) {
  console.log('There is not much to verify here except:\n')
  console.log('   Did you have fun?\n   Did your horses run?\n   Did your friends have fun?')
  console.log('\nIf you answered NO to any of the above questions please hit Ctrl-C now!')

  setTimeout(cb, 5000);
}

exercise.requireSubmission = false;
exercise.addSetup(checkFun);  

module.exports = exercise;
