Chess Engine
====================

A chess engine powered by lookup tables and bad code.

## How this works ##
It doesn't. I'm still implementing vectors and maps.

Ultimately, this should comb thorugh lookup tables to generate all possible moves per piece.
Each player knows the check status of the king (none, single, double) to reduce the number of moves checked.
They also know which pieces can put the king in check and which ray they sit on.

Not sure how to implement stalemate. Part one is to understand what a stalemate is. I might just be lazy and pretend stalemates don't exist.

## Contributing ##
All help is appreciated.

Please do not directly modify chessconstants.c. It is comprised of five generators plus a wrapper for those generators. If there is a problem with any of the lookup tables, fix the associated generator file (or the wrapper). I'm alright if you regenerate chessconstants.c - just don't make manual changes to the file.

