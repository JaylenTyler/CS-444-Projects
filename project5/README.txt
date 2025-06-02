# Very, Very Simple File System (VVSFS)

## Overview
This project simulates a "Very, Very Simple File System operating at the block level using Unix system calls such as (`open()`, `read()`, `write()`, `lseek()`, etc.). 
It is built to follow a structure similar to a real file system, but using a single image file to store all data.

## Block Layout
The simulated file system reserves the first 7 blocks (4096 bytes each) as follows:

- Block 0: Superblock
- Block 1: Inode map
- Block 2: Block map
- Block 3-6: Inode blocks
- Block 7+: File data blocks

## Components
- `image.c/.h`: Manages opening and closing the image file that simulates the disk.
- `block.c/.h`: Provides `bread()` and `bwrite()` for reading/writing 4KB blocks within the image file.
- `testfs.c`: Manual tests that validate correct block read/write and image file operations using `ctest.h`.
- `ctest.h`: Beej’s minimal C test framework header (used in simple mode).

## Building

(optional) make clean
make

## Testing

make test

or

./testfs