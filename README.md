# chatty

Logger for both C++ and Python.

[![Build Status](https://travis-ci.org/thorbenk/chatty.svg?branch=master)](https://travis-ci.org/thorbenk/chatty)

## Building

On Ubuntu 17.04:

```bash
sudo apt install libboost-dev libboost-python-dev libboost-system-dev
sudo apt install libboost-filesystem-dev libboost-serialization-dev
sudo apt install libboost-date-time-dev libboost-test-dev
sudo apt install libboost-regex-dev libboost-thread-dev ninja-build cmake

mkdir build && cd build
cmake -GNinja ..
ninja
ctest .
```
