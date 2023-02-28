#pragma once
#include <pthread.h>
#include <stdio.h>
// #include <threads.h>
#include <signal.h>
#include <stdio.h>
// #include <stdatomic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct wallet_resource_
  {
    const char *resource_name;
    sig_atomic_t amount;
    struct wallet_resource_ *next;
  } wallet_resource;

typedef struct wallet_t_ {
  pthread_mutex_t lock;
  pthread_cond_t conditional;
  wallet_resource *head;
} wallet_t;

void wallet_init(wallet_t *wallet);
int wallet_get(wallet_t *wallet, const char *resource);
int wallet_change_resource(wallet_t *wallet, const char *resource, const int delta);
void wallet_destroy(wallet_t *wallet);

#ifdef __cplusplus
}
#endif