Blaze (formerly Trappist)
=========================

Blaze is a UCI compatible chess engine written in C

## Build
Setting the `DEBUG` variable allows for a custom debug build with debugging symbols

```
# Release build
make -j4

# Run
./build/release/blaze

# Debug build
make -j4 DEBUG=1
```

## Technical details
Blaze is inspired by the didactic engine QBBEngine by Fabio Gobbato
- Quad bitboards (Using 4 bitboards to represent the entire chess board)
- Using the [o^(o-2r)-trick](https://www.chessprogramming.org/Hyperbola_Quintessence) to generate sliding piece attacks
- Make/Unmake (Rather than Copy/Make)

## Test

```
make check
```

## Benchmark

## License
Blaze is licensed under the MIT license, a copy of this license can be found in `LICENSE`
