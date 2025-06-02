#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "image.h"
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
