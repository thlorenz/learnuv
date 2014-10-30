#!/usr/bin/env bash

rm -rf ./gitbook
gitbookify slides.md -o gitbook      && \
  rm -rf ./book                      && \
  gitbook build ./gitbook -o  ./book && \
  cd ./book                          && \
  echo "http://localhost:3000" && http-server -p 3000 -c-1
