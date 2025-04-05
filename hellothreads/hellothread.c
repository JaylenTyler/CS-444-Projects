#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define COUNT 5

void* print_numbers(void* arg) {
    char* thread_name = (char*)arg;

    for (int i = 0; i < COUNT; i++) {
        printf("%s: %d\n", thread_name, i);
    }

    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    char* name1 = "thread 1";
    char* name2 = "thread 2";

    printf("Launching threads\n");

    if (pthread_create(&t1, NULL, print_numbers, name1) != 0) {
        fprintf(stderr, "Error creating thread 1\n");
        return 1;
    }

    if (pthread_create(&t2, NULL, print_numbers, name2) != 0) {
        fprintf(stderr, "Error creating thread 2\n");
        return 1;
    }

    if (pthread_join(t1, NULL) != 0) {
        fprintf(stderr, "Error joining thread 1\n");
        return 1;
    }

    if (pthread_join(t2, NULL) != 0) {
        fprintf(stderr, "Error joining thread 2\n");
        return 1;
    }

    printf("Threads complete!\n");
    return 0;
}
