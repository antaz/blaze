[![Build Status](https://travis-ci.org/flurt/Trappist.svg?branch=master)](https://travis-ci.org/flurt/Trappist)

# Trappist

This a little chess engine AI written in C, not the strongest, not the weakest.

## How it works?

Trappist uses a bruteforce algorithm for solving perfect information / zero-sum games called minimax with alphabeta prunning (kinda like an enhancement).

Trappist is also UCI compatible but probably not fully compatible yet

## Build

To build Trappist we recommend using `gcc` (a free an open source C compiler)

```
make
```

## Credits

This engine came into existence only because of the great help from [ZirconiumX](https://github.com/ZirconiumX) aka Dan Ravensloft and his engine [Dorpsgek](https://github.com/ZirconiumX/Dorpsgek), thank you Dan.

and i also thank all people in `##chessprogramming` community on IRC Freenode, and people who made [chessprogramming](https://www.chessprogramming.org/) it's a great resource as well as the video series on youtube [programming a chess engine in C](https://www.youtube.com/watch?v=bGAfaepBco4&list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg)
