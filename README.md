# learnuv

Learn uv for fun and profit, a self guided workshop to [the library](https://github.com/joyent/libuv) that powers Node.js.

## Status

Currently under construction, but **ready for alpha testers** (a few exercises are implemented already).

## Installation

Requires [Python](https://www.python.org/downloads/) 2.6 or 2.7 to be installed.
If python is not in your path set the environment variable `PYTHON` to its
location. For example on Windows: `set PYTHON=C:\Python27\python.exe`

```sh
git clone https://github.com/thlorenz/learnuv && cd learnuv
npm install
```

## Building

Ninja and Make do not work on windows except via [cygwin](https://www.cygwin.com/).

### Ninja

Highly recommended since it builds faster than any of the other options, so [get ninja](http://martine.github.io/ninja/)
and do:

```sh
./gyp_learnuv.py -f ninja
ninja -C out/Debug
./out//Debug/learnuv_ex01
```

### Make

Works on Linux and OSX.

```sh
./gyp_learnuv.py -f make
make -C out
./out/Debug/learnuv_ex01
```

### Xcode

Works on OSX only. Highly recommended to ease debugging and code navigation.

```sh
./gyp_learnuv.py -f xcode
open learnuv.xcodeproj
```

### Visual Studio

Works on Windows only. **TODO** need to adapt [vcbuild.bat](https://github.com/joyent/libuv/blob/master/vcbuild.bat).

## License

MIT
