# libmempool

[![Build Status](https://travis-ci.org/jonas-fan/libmempool.svg?branch=master)](https://travis-ci.org/jonas-fan/libmempool)

Memory pool implementation in C.

- Fixed-size memory pool
- Variable-size memory pool

## Building

```bash
$ cd libmempool/
$ mkdir build/
$ cd build/
$ cmake ..
$ make -j4
```

## Test

```bash
$ cd build/
$ ./tests/mempool_test
```

## License

The MIT License (MIT)