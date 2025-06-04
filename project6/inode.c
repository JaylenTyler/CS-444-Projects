#include "inode.h"
#include "free.h"
#include "block.h"

int ialloc(void) {
    unsigned char block[4096];
    bread(1, block);                  // load inode map
    int inode_num = find_free(block);
    if (inode_num == -1)
        return -1;

    set_free(block, inode_num, 1);   // mark as allocated
    bwrite(1, block);                // save inode map
    return inode_num;
}
