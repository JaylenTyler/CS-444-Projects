#include <stdlib.h>
#include <string.h>
#include "dir.h"
#include "block.h"
#include "pack.h"

#define BLOCK_SIZE 4096
#define DIR_ENTRY_SIZE 32

struct directory *directory_open(int inode_num) {
    struct inode *inode = iget(inode_num);
    if (!inode) return NULL;

    struct directory *dir = malloc(sizeof(struct directory));
    if (!dir) {
        iput(inode);
        return NULL;
    }

    dir->inode = inode;
    dir->offset = 0;
    return dir;
}

int directory_get(struct directory *dir, struct directory_entry *ent) {
    if (dir->offset >= dir->inode->size) return -1;

    int block_index = dir->offset / BLOCK_SIZE;
    int offset_in_block = dir->offset % BLOCK_SIZE;

    unsigned char block[BLOCK_SIZE];
    int block_num = dir->inode->block_ptr[block_index];
    bread(block_num, block);

    ent->inode_num = read_u16(block + offset_in_block);
    strcpy(ent->name, (char *)(block + offset_in_block + 2));

    dir->offset += DIR_ENTRY_SIZE;
    return 0;
}

void directory_close(struct directory *d) {
    iput(d->inode);
    free(d);
}
