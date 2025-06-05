#include "free.h"
#include "inode.h"
#include "block.h"
#include "image.h"
#include "pack.h"
#include "dir.h"
#include "ctest.h"
#include <string.h>
#include <stdio.h>

void test_image_open_close() {
    image_open("test.img", 1);
    CTEST_ASSERT(image_fd != -1, "image_fd should not be -1 after open");
    image_close();
}

void test_block_read_write() {
    image_open("test.img", 1);

    unsigned char write_buf[4096];
    unsigned char read_buf[4096];
    memset(write_buf, 'X', sizeof(write_buf));

    bwrite(10, write_buf);
    bread(10, read_buf);

    CTEST_ASSERT(memcmp(write_buf, read_buf, sizeof(write_buf)) == 0, "Block read/write round-trip failed");

    image_close();
}

void test_free_functions() {
    unsigned char map[4096];
    memset(map, 0xFF, sizeof(map));     // All used
    set_free(map, 10, 0);               // Free bit 10
    CTEST_ASSERT(find_free(map) == 10, "find_free should return 10");

    set_free(map, 10, 1);               // Mark bit 10 used
    CTEST_ASSERT(find_free(map) == -1, "find_free should return -1 when full");
}

void test_ialloc() {
    unsigned char map[4096] = {0};
    image_open("test.img", 1);
    bwrite(1, map); // clear inode map

    struct inode *inode = ialloc();
    CTEST_ASSERT(inode != NULL, "ialloc should return a valid pointer");

    if (inode != NULL) {
        CTEST_ASSERT(inode->inode_num == 0, "First allocated inode should have inode_num == 0");
        CTEST_ASSERT(inode->flags == ALLOCATED, "Allocated inode should have ALLOCATED flag set");
    }

    image_close();
}

void test_alloc() {
    unsigned char map[4096] = {0};
    image_open("test.img", 1);
    bwrite(2, map); // clear block map

    int block = alloc();
    CTEST_ASSERT(block == 0, "alloc should return 0");

    image_close();
}


void test_directory_functions() {
    image_open("test.img", 1);
    mkfs();  // Create root directory with . and ..

    struct directory *dir = directory_open(0);
    CTEST_ASSERT(dir != NULL, "directory_open() should return non-NULL");

    struct directory_entry ent;
    int count = 0;

    printf("Root inode size: %u\n", dir->inode->size);

    while (directory_get(dir, &ent) != -1) {
        CTEST_ASSERT(ent.inode_num == 0, "inode_num should be 0 (root)");
        CTEST_ASSERT(
            strcmp(ent.name, ".") == 0 || strcmp(ent.name, "..") == 0,
            "name should be . or .."
        );
        count++;
    }

    printf("Root inode size: %u\n", dir->inode->size);


    CTEST_ASSERT(count == 2, "should read exactly two entries from root dir");

    directory_close(dir);
    image_close();
}

int main() {
    test_directory_functions();  // Must run BEFORE test_ialloc()

    test_image_open_close();
    test_block_read_write();
    test_free_functions();
    test_ialloc();  
    test_alloc();

    CTEST_RESULTS();
    return 0;
}
