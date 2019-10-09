## nrfdfu-node

A Node.js module (Linux-only) for performing Bluetooth DFU firmware upgrades for Nordic Semiconductor nRF52 and probably nRF51 using a regular BT interface.

### Installing

This package should be built on the machine it's going to run. It is only compatible with Linux systems.
Before compiling, install these dependencies:

```
sudo apt-get update
sudo apt-get install libzip-dev libjson-c-dev
```

### Author

The original C code is based on [https://gitlab.com/visti_how/nrfdfu_ng](https://gitlab.com/visti_how/nrfdfu_ng).
Little modifications were made to C code. The Node.js code was written by Daniel Sandoval for
[LoopKey](https://www.loopkey.com.br).

### License

This code is licensed under GNU GPLv3.
