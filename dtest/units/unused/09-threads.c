// -lm
// 0

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define SIZE 1000000000
#define THREADS_NUM 8

unsigned int long *shared;

struct TArgs {
  unsigned long int start;
  unsigned long int finish;
};

void setup_shared() {
  shared = calloc(SIZE, sizeof(unsigned long int));
  if (shared == NULL)
    printf("No memory");
  for (unsigned long int i = 0; i < SIZE; i++)
    shared[i] = 0;
}

void process_shared(unsigned int long start,unsigned int long finish) {
  for (unsigned int long j = 0; j < 10; j++)
    for (unsigned int long i = start; i < finish; i++)
      shared[i] = (unsigned long int) sqrt((double)i);
}

void *my_thread(void *args) {
  struct TArgs *a;
  a = (struct TArgs *) args;
  unsigned long int start = a->start;
  unsigned long int finish = a->finish;
  process_shared(start, finish);
  return NULL;
}

void printout_shared() {
  printf("%lu %lu %lu\n", shared[0], shared[SIZE / 2], shared[SIZE - 1]);
}

void use_threads() {
  pthread_t threads[THREADS_NUM];
  for (int i = 0; i < THREADS_NUM; i++) {
    unsigned long int chunk_size = SIZE / (unsigned long int) THREADS_NUM;
    struct TArgs *args = calloc(1, sizeof(struct TArgs));
    *args = (struct TArgs) {
      .start = chunk_size * (unsigned long int) i,
      .finish = chunk_size * (unsigned long int) i + chunk_size
    };
    pthread_create(&threads[i], NULL, my_thread, (void *) args);
  }
  for (int i = 0; i < THREADS_NUM; i++)
    pthread_join(threads[i], NULL);
}

int main() {
  DTEST_UNIT_START("Threads");

  DTEST_EVAL_TIME(setup_shared());
  DTEST_EVAL_TIME(process_shared(0, SIZE));
  printout_shared();
  free(shared);

  DTEST_EVAL_TIME(setup_shared());
  DTEST_EVAL_TIME(use_threads());
  printout_shared();
  free(shared);

  DTEST_UNIT_END;
}
