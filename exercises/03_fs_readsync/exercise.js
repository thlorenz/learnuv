'use strict';
var exercise = require('workshopper-exercise')()
  , checkFileContent = require('../../workshopper/check-file-content')

exercise.requireSubmission = false;
exercise.addSetup(checkFileContent.bind(null, exercise));  

module.exports = exercise;
