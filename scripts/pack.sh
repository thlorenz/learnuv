#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT=$DIR/..

cd $ROOT
cp package.json package.orig.json                && \
  $DIR/bundle-dependencies.js $ROOT/package.json && \
  npm pack                                       && \
  mv package.orig.json package.json 
