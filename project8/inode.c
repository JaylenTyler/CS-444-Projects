#include "inode.h"
#include "block.h"
#include "pack.h"
#include "free.h"
#include <string.h>
#include <stdio.h>

static struct inode incore[MAX_SYS_OPEN_FILES] = {0};

// Find a free in-core inode
struct inode *incore_find_free(void) {
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++) {
        if (incore[i].ref_count == 0) {
            return &incore[i];
        }
    }
    return NULL;
}

// Find an in-core inode by inode number
struct inode *incore_find(unsigned int inode_num) {
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++) {
        if (incore[i].ref_count > 0 && incore[i].inode_num == inode_num) {
            return &incore[i];
        }
    }
    return NULL;
}

// Set all in-core inode ref_counts to 0
void incore_free_all(void) {
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++) {
        incore[i].ref_count = 0;
    }
}

// Read inode from disk into in-core structure
void read_inode(struct inode *in, int inode_num) {
    unsigned char block[BLOCK_SIZE];

    int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    int inode_index = inode_num % INODES_PER_BLOCK;
    int offset = inode_index * INODE_SIZE;

    bread(block_num, block);

    in->size = read_u32(block + offset + 0);
    in->owner_id = read_u16(block + offset + 4);
    in->permissions = read_u8(block + offset + 6);
    in->flags = read_u8(block + offset + 7);
    in->link_count = read_u8(block + offset + 8);

    for (int i = 0; i < INODE_PTR_COUNT; i++) {
        in->block_ptr[i] = read_u16(block + offset + 9 + i * 2);
    }
}

// Write in-core inode to disk
void write_inode(struct inode *in) {
    unsigned char block[BLOCK_SIZE];

    int block_num = in->inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    int inode_index = in->inode_num % INODES_PER_BLOCK;
    int offset = inode_index * INODE_SIZE;

    bread(block_num, block);

    write_u32(block + offset + 0, in->size);
    write_u16(block + offset + 4, in->owner_id);
    write_u8(block + offset + 6, in->permissions);
    write_u8(block + offset + 7, in->flags);
    write_u8(block + offset + 8, in->link_count);

    for (int i = 0; i < INODE_PTR_COUNT; i++) {
        write_u16(block + offset + 9 + i * 2, in->block_ptr[i]);
    }

    printf("WRITE_INODE %d → size: %u, flags: %d\n", in->inode_num, in->size, in->flags);


    bwrite(block_num, block);
}

// Get an in-core inode, loading from disk if necessary
struct inode *iget(int inode_num) {
    struct inode *in = incore_find(inode_num);
    if (in != NULL) {
        in->ref_count++;
        return in;
    }

    in = incore_find_free();
    if (in == NULL) {
        return NULL;
    }

    read_inode(in, inode_num);
    in->ref_count = 1;
    in->inode_num = inode_num;
    return in;
}

// Decrement ref_count and write inode to disk if unused
void iput(struct inode *in) {
    if (in->ref_count == 0) {
        return;
    }

    in->ref_count--;

    if (in->ref_count == 0) {
        write_inode(in);
    }
}

void mkfs(void) {
    // Clear inode and block maps
    unsigned char zero_block[BLOCK_SIZE] = {0};
    bwrite(1, zero_block); // inode map
    bwrite(2, zero_block); // block map

    // Allocate inode for root directory
    struct inode *in = ialloc();
    int inode_num = in->inode_num;

    // Allocate block for root directory data
    int block_num = alloc();

    // Initialize root inode
    in->flags = 2; // directory
    in->size = 64;
    in->block_ptr[0] = block_num;

    // Fill directory data
    unsigned char block[BLOCK_SIZE] = {0};
    write_u16(block + 0, inode_num);
    strcpy((char *)(block + 2), ".");
    write_u16(block + 32, inode_num);
    strcpy((char *)(block + 34), "..");

    // Write directory block to disk
    bwrite(block_num, block);

    // Ensure inode changes are flushed to disk
    write_inode(in);
    iput(in);
}
