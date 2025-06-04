#ifndef BLOCK_H
#define BLOCK_H

unsigned char *bread(int blocknum, unsigned char *block);
void bwrite(int blocknum, unsigned char *block);
int alloc(void); // ← Add this line

#endif
