#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "image.h"
#include "free.h"
#include "block.h"


#define BLOCK_SIZE 4096

unsigned char *bread(int block_num, unsigned char *block) {
    off_t offset = block_num * BLOCK_SIZE;
    if (lseek(image_fd, offset, SEEK_SET) == -1) return NULL;
    if (read(image_fd, block, BLOCK_SIZE) != BLOCK_SIZE) return NULL;
    return block;
}

void bwrite(int block_num, unsigned char *block) {
    off_t offset = block_num * BLOCK_SIZE;
    if (lseek(image_fd, offset, SEEK_SET) == -1) return;
    write(image_fd, block, BLOCK_SIZE);
}

int alloc(void) {
    unsigned char block[4096];
    bread(2, block);                 // load block map
    int block_num = find_free(block);
    if (block_num == -1)
        return -1;

    set_free(block, block_num, 1);  // mark as allocated
    bwrite(2, block);               // save block map
    return block_num;
}
