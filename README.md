# mempool

[![Build Status](https://travis-ci.org/jonas-fan/mempool.svg?branch=master)](https://travis-ci.org/jonas-fan/mempool)

Simple, header-only memory pool in C.

## Build

Since there is only a header, you can choose the way you like to use.

## Testing

Note that the statistics will depend on the machine.

```bash
$ mkdir build/
$ cd build/
$ cmake ..
$ make -j4
$ ./tests/mempool_stress_test
# Test 001
os malloc/free ... elapsed 429.94ms
mempool alloc/free ... elapsed 9.62ms
# Test 002
os malloc/free ... elapsed 423.46ms
mempool alloc/free ... elapsed 20.32ms
# Test 003
os malloc/free ... elapsed 409.97ms
mempool alloc/free ... elapsed 20.03ms
```

## License

The MIT License (MIT)
