#include <stddef.h>     // for NULL
#include "block.h"      // for NUM_INODES
#include "inode.h"      // for struct inode and ALLOCATED

void set_free(unsigned char *block, int num, int set) {
    int byte_num = num / 8;
    int bit_num = num % 8;
    if (set)
        block[byte_num] |= (1 << bit_num);
    else
        block[byte_num] &= ~(1 << bit_num);
}

int find_low_clear_bit(unsigned char x) {
    for (int i = 0; i < 8; i++)
        if (!(x & (1 << i)))
            return i;
    return -1;
}

int find_free(unsigned char *block) {
    for (int i = 0; i < 4096; i++) {
        int bit = find_low_clear_bit(block[i]);
        if (bit != -1)
            return i * 8 + bit;
    }
    return -1;
}


struct inode *ialloc(void) {
    for (int i = 0; i < NUM_INODES; i++) {
        struct inode *in = iget(i);
        if (in == NULL) {
            continue;
        }

        if (in->flags == 0) { // Free inode
            in->flags = ALLOCATED;
            in->owner_id = 0;
            in->permissions = 0;
            in->link_count = 0;
            in->size = 0;

            for (int j = 0; j < INODE_PTR_COUNT; j++) {
                in->block_ptr[j] = 0;
            }

            write_inode(in);  // Commit changes to disk
            return in;
        }

        iput(in);  // Release in-core inode if not used
    }

    return NULL; // No free inodes found
}
