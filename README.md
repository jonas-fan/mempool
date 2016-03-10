# mempool
[![Build Status](https://travis-ci.org/yjfan/mempool.svg?branch=master)](https://travis-ci.org/yjfan/mempool)

Simple memory pool implementation in C.
- Fixed-size memory pool
- Variable-size memory pool

## Build
```
$ git clone https://github.com/yjfan/mempool.git
$ cd mempool/
$ mkdir build/
$ cd build/
$ cmake ..
$ make -j4
$ make install
```

## Test
```
$ cd build/
$ ./test/mempool_test
```

Macbook Pro 13" (Intel core i5 2.6GHz) + Vitualbox + Ubuntu-15.10-x86_64 + CPU * 2 + 2G DRAM

Test case: (Allocate 256bytes * 4000times) * 1000times

|                           |  Case 1  |   Case 2  |  Case 3  |
|:-------------------------:|:--------:|:---------:|:--------:|
|        malloc/free        | 149.81ms |  419.6ms  | 970.16ms |
|   Fixed-size memory pool  |  19.16ms |  32.66ms  |  26.82ms |
| Variable-size memory pool |  47.77ms | 15595.9ms |  55.72ms |

## License
The MIT License (MIT)
