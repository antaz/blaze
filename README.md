Blaze (formerly Trappist)
=========================

Blaze is a UCI compatible chess engine written in C

## Build
Setting the `DEBUG` variable allows for a custom debug build with debugging symbols

```
# Release build
make -j4

# Run
./build/release/blaze/blaze

# Debug build
make -j4 DEBUG=1
```

## Test

```
make check
```

## Benchmark

## License
Blaze is licensed under the MIT license, a copy of this license can be found in `LICENSE`
