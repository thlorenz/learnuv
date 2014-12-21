'use strict';
var net = require('net')
  , colors = require('ansicolors')
  , format = require('util').format
  , table = require('text-table')

var HOST = '0.0.0.0'
  , PORT = 7000;

var exercise = require('workshopper-exercise')()

exercise.requireSubmission = false;
exercise.addSetup(checkClientResponse);  

module.exports = exercise;

function log() {
  console.log(colors.green('progress ') + colors.brightBlack(format.apply(this, arguments)));
}

function checkClientResponse(mode, cb) {
  var progress = {
      connected       : false
    , responded       : false
    , correctResponse : false
    , closed          : false
    , error           : false
  }

  var fail = false
    , quit = false
    , finished = false
    , msg = 'Hello Server can you echo?'
    , timeout

  function finish() {
    clearTimeout(timeout);
    if (finished) return;
    finished = true;

    var arr = Object.keys(progress)
      .reduce(function key(acc, k) {
        if (k === 'error') { fail = fail || !!progress[k]; return acc }
        fail = fail || !progress[k]
        var label = colors.blue(k[0].toUpperCase() + k.slice(1));
        acc.push([ label, progress[k] ? colors.green('OK') : colors.red('NOT OK') ])
        return acc
      }, [])

    console.log('\n' + table(arr));
    
    if (progress.error) console.error('\nEncountered an error', progress.error);
    cb(fail && mode === 'verify' ? new Error('Sorry something was not quite right yet :(') : null);
  }

  // give up after 3s, i.e. if server is irresponsive or doesn't quit
  timeout = setTimeout(finish, 3000);

  var client = new net.Socket();
  log('Client connecting to server');
  client
    .connect(PORT, HOST, function onconnection() {
      progress.connected = true;
      log('Client sending "%s"', msg);
      client.write(msg);
    }) 
    .on('error', function onerror(err) {
      progress.error = err;
      finish();
    })
    .on('data', function ondata(data) {
      progress.responded = true
      var res = data.toString();
      log('Server responded with "%s"', msg);

      progress.correctResponse = (res === msg)

      log('Telling server to quit');
      client.write('QUIT');
    })
    .on('close', function onclose() {
      progress.closed = true;
      log('Server closed connection')
      finish()
    })
}
