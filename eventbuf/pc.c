#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "eventbuf.h"

#define SEM_NAME_MAX 64

struct eventbuf *queue;
int consumers_total;
int done_producing = 0;

sem_t *mutex;
sem_t *items;
sem_t *spaces;

sem_t *sem_open_temp(const char *name, int value) {
    sem_t *sem = sem_open(name, O_CREAT, 0600, value);
    if (sem == SEM_FAILED) return SEM_FAILED;
    sem_unlink(name);
    return sem;
}

void *producer(void *arg) {
    int id = *(int *)arg;
    extern int events_per_producer;

    for (int i = 0; i < events_per_producer; i++) {
        int event = id * 100 + i;
        sem_wait(spaces);
        sem_wait(mutex);
        eventbuf_add(queue, event);
        printf("P%d: adding event %d\n", id, event);
        sem_post(mutex);
        sem_post(items);
    }

    sem_wait(mutex);
    printf("P%d: exiting\n", id);
    sem_post(mutex);
    return NULL;
}

void *consumer(void *arg) {
    int id = *(int *)arg;

    while (1) {
        sem_wait(items);
        sem_wait(mutex);
        if (done_producing && eventbuf_empty(queue)) {
            sem_post(mutex);
            break;
        }
        if (eventbuf_empty(queue)) {
            sem_post(mutex);
            continue;
        }
        int event = eventbuf_get(queue);
        printf("C%d: got event %d\n", id, event);
        sem_post(mutex);
        sem_post(spaces);
    }

    printf("C%d: exiting\n", id);
    return NULL;
}

int events_per_producer;

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <producers> <consumers> <events per producer> <max outstanding>\n", argv[0]);
        exit(1);
    }

    int producers = atoi(argv[1]);
    int consumers = atoi(argv[2]);
    events_per_producer = atoi(argv[3]);
    int max_outstanding = atoi(argv[4]);

    consumers_total = consumers;

    queue = eventbuf_create();
    if (!queue) {
        perror("Failed to create event buffer");
        exit(1);
    }

    char mutex_name[SEM_NAME_MAX] = "/pc-mutex";
    char items_name[SEM_NAME_MAX] = "/pc-items";
    char spaces_name[SEM_NAME_MAX] = "/pc-spaces";

    mutex = sem_open_temp(mutex_name, 1);
    items = sem_open_temp(items_name, 0);
    spaces = sem_open_temp(spaces_name, max_outstanding);

    if (!mutex || !items || !spaces) {
        perror("sem_open_temp failed");
        exit(1);
    }

    pthread_t ptids[producers];
    pthread_t ctids[consumers];
    int pids[producers];
    int cids[consumers];

    for (int i = 0; i < producers; i++) {
        pids[i] = i;
        pthread_create(&ptids[i], NULL, producer, &pids[i]);
    }

    for (int i = 0; i < consumers; i++) {
        cids[i] = i;
        pthread_create(&ctids[i], NULL, consumer, &cids[i]);
    }

    for (int i = 0; i < producers; i++) {
        pthread_join(ptids[i], NULL);
    }

    sem_wait(mutex);
    done_producing = 1;
    sem_post(mutex);

    for (int i = 0; i < consumers; i++) {
        sem_post(items);
    }

    for (int i = 0; i < consumers; i++) {
        pthread_join(ctids[i], NULL);
    }

    eventbuf_free(queue);
    sem_close(mutex);
    sem_close(items);
    sem_close(spaces);
    return 0;
}
