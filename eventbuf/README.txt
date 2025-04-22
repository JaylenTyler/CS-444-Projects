# Producer-Consumer Project

## Building

Command line:

* `make` to build. An executable called `pc` will be produced.
* `make clean` to clean up all build products except the executable.
* `make pristine` to clean up all build products entirely.

VS Code:

* The default build task runs `make`.

## Files

* `pc.c`: Main source file that implements the producer-consumer logic
* `eventbuf.c`: Implementation of the non-thread-safe FIFO event buffer
* `eventbuf.h`: Header file describing the event buffer interface

## Data

The main shared data structure is a FIFO queue (`eventbuf`) that holds integer event values. It is not thread-safe and must be accessed only when protected by a mutex semaphore.

Other global data includes:

- `done_producing`: A flag to signal when all producers have finished
- POSIX named semaphores: `mutex`, `items`, `spaces`

## Functions

* `main()`
  * Parses arguments and sets up shared data
  * Creates producer and consumer threads
  * Waits for all producer threads to finish
  * Signals consumers to exit
  * Cleans up semaphores and memory

* `producer()`
  * Waits for a free space
  * Locks the queue
    * Adds an event to the buffer
    * Prints event message
  * Unlocks the queue
  * Posts to `items`

* `consumer()`
  * Waits on `items`
  * Locks the queue
    * If `done_producing` and buffer empty, exits
    * Otherwise, consumes an event and prints it
  * Unlocks the queue
  * Posts to `spaces`

* `sem_open_temp(name, value)`
  * Creates a named semaphore and unlinks it immediately to ensure cleanup

## Notes

* Uses POSIX named semaphores via `sem_open()` and `sem_unlink()`
* Avoids `sem_init()` and `pthread_mutex_t` as required
* Outputs are printed within the mutex to keep results consistent
* Use `^C` to manually terminate if needed during development

## Author

Jaylen Tyler
