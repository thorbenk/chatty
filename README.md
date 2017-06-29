# chatty

Logger for both C++ and Python.

[![Build Status](https://travis-ci.org/thorbenk/chatty.svg?branch=master)](https://travis-ci.org/thorbenk/chatty)

![screencast](https://raw.githubusercontent.com/thorbenk/chatty/master/examples/chatty.gif)

Here is the python script that generates the above log:
```python
from chatty import *
import time

log = Logger.getInstance()
log.setLogfile('/tmp/chatty.log')

log.log(Format.header1, Color.green, 'Hello world!')
log.warn('warning: starting example')

example_timer = log.startNewTimer('run example')

with IndentationContextManager() as l:
    l.log(Color.yellow, "indented log")

with TimedLineContextManager('long operation'):
    time.sleep(0.7)

N = 10
with ProgressLoopContextManager('long loop', N) as l:
    for i in range(10):
        time.sleep(0.4)
        l.advanceProgressLoop(1)

log.endTimer(example_timer)

time.sleep(2)
```

A similar API can be used from C++. Both Python and C++ will share the
same logger instance.

## Building

On Ubuntu 17.04:

```bash
sudo apt install libboost-dev libboost-python-dev libboost-system-dev
sudo apt install libboost-filesystem-dev libboost-serialization-dev
sudo apt install libboost-date-time-dev libboost-test-dev
sudo apt install libboost-regex-dev libboost-thread-dev ninja-build cmake

mkdir build && cd build
cmake -GNinja \
  -DCMAKE_INSTALL_PREFIX=~/inst \
  -DPYTHON_SITE_PACKAGES=~/inst/lib/python-2.7/site-packages \
  -DCMAKE_BUILD_TYPE-Release ..
ninja
ctest .
```
