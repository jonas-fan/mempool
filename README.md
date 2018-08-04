# libmempool

[![Build Status](https://travis-ci.org/jonas-fan/libmempool.svg?branch=master)](https://travis-ci.org/jonas-fan/libmempool)

Header-only memory pool in C.

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
$ ./tests/stress_test
# Test 001 
os malloc/free ... elapsed 85.22ms
mempool alloc/free ... elapsed 1.92ms

# Test 002 
os malloc/free ... elapsed 87.92ms
mempool alloc/free ... elapsed 4.03ms

# Test 003 
os malloc/free ... elapsed 82.58ms
mempool alloc/free ... elapsed 4.11ms
```

## License

The MIT License (MIT)
