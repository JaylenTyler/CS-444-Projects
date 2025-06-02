#include <stdio.h>
#include <string.h>
#include "ctest.h"
#include "image.h"
#include "block.h"

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

int main() {
    test_image_open_close();
    test_block_read_write();
    CTEST_RESULTS();
    return 0;
}
