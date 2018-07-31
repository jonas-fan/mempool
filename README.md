# libmempool

[![Build Status](https://travis-ci.org/jonas-fan/libmempool.svg?branch=master)](https://travis-ci.org/jonas-fan/libmempool)

Single header, fixed-size memory pool implementation in C.

## Building

```bash
$ cd libmempool/
$ mkdir build/
$ cd build/
$ cmake ..
$ make -j4
```

## Testing

Note that the statistics will depend on the machine.

```bash
$ ./tests/stress
===== malloc/free =====
[Case 1]: 82.18ms
[Case 2]: 395.36ms
[Case 3]: 953.91ms

===== Fixed-size memroy pool =====
[Case 1]: 1.71ms
[Case 2]: 3.52ms
[Case 3]: 3.54ms
```

## License

The MIT License (MIT)