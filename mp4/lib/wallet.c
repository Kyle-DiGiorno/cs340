#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include "wallet.h"

/**
 * Initializes an empty wallet.
 */
void wallet_init(wallet_t *wallet) {
  // Implement `wallet_init`
  pthread_cond_init(&wallet->conditional, NULL);
  pthread_mutex_init(&wallet->lock, NULL);
  wallet->head = NULL;
}

/**
 * Returns the amount of a given `resource` in the given `wallet`.
 */
int wallet_get(wallet_t *wallet, const char *resource) {
  // Implement `wallet_get`
  //pthread_mutex_lock(&wallet->lock);
  printf("in wallet$------^&*(O)\n");
  int amt = 0;
  wallet_resource *nxt = wallet->head;
  while (nxt)
  {
    if (strcmp(nxt->resource_name, resource) == 0)
    {
      amt = nxt->amount;
    }
    nxt = nxt->next;
  }
  //pthread_mutex_unlock(&wallet->lock);
  return (amt);
}
int cnt = 0;
/**
 * Modifies the amount of a given `resource` in a given `wallet by `delta`.
 * - If `delta` is negative, this function MUST NOT RETURN until the resource can be satisfied.
 *   (Ths function MUST BLOCK until the wallet has enough resources to satisfy the request.)
 * - Returns the amount of resources in the wallet AFTER the change has been applied.
 */
int wallet_change_resource(wallet_t *wallet, const char *resource, const int delta) {
  pthread_cond_broadcast(&wallet->conditional);
  // Implement `wallet_change_resource`
  wallet_resource *nxt = wallet->head;

  wallet_resource *curr = wallet->head;
  if(strcmp("ping", resource) == 0){
    cnt +=delta;
    //printf("%d\n",cnt);
  }
  while (nxt)
  {
    if (strcmp(nxt->resource_name, resource) == 0)
    {
      break;
    }
    curr = nxt;
    nxt = nxt->next;
    //printf("\nt\n");
  }
  // if(nxt){
  //   printf("\nyeah\n");
  // }
  pthread_cond_broadcast(&wallet->conditional);
  printf("cr1\n");
  if (!nxt)
  {
    printf("cr2\n");
    //pthread_mutex_lock(&wallet->lock);
    //printf("\n0\n");
    wallet_resource *new_re = malloc(sizeof(wallet_resource));
    //printf("\ni\n");
    new_re->amount = 0;
    new_re->resource_name = resource;
    new_re->next = NULL;
    if (curr)
    {
      curr->next = new_re;
      nxt = curr->next;
    }
    else
    {
      wallet->head = new_re;
      nxt = wallet->head;
    }
    //printf("\n2\n");
    //pthread_mutex_unlock(&wallet->lock);
  }
  printf("cr10\n");
  /*if (!nxt)
  {
    pthread_mutex_unlock(&wallet->lock);
    return;
  }*/
  pthread_mutex_lock(&wallet->lock);
  //printf(nxt->resource_name);
  printf("\n%d\n",delta);
   if(delta < 0){
    while (nxt->amount + delta < 0)
     {
       printf("\nlast%d\n",delta);
       //printf(nxt->resource_name);
  //   //  printf("\nlyp%d\n",nxt->amount);
       //printf("\n%d\n",nxt->amount + delta);
       printf("cr12\n");
       pthread_cond_wait(&wallet->conditional, &wallet->lock);
  //     //break;
      }
  //   printf("\nhere 3\n");
  }
  printf("\nhere 2\n");
  nxt->amount += delta;
  //printf("\n%d\n", nxt->amount);
  pthread_mutex_unlock(&wallet->lock);
  printf("\nhere 3\n");
  pthread_cond_broadcast(&wallet->conditional);
  printf("\nhere 32\n");
  return nxt->amount;
}

/**
 * Destroys a wallet, freeing all associated memory.
 */
void wallet_destroy(wallet_t *wallet) {
  // Implement `wallet_destroy`
  printf("\nhere 3r\n");
  pthread_cond_destroy(&wallet->conditional);
  pthread_mutex_destroy(&wallet->lock);
  printf("\nhere 3r\n");
  wallet_resource *curr = wallet->head;
  wallet_resource *nxt = wallet->head;
  while (nxt)
  {
    nxt->amount = 0;
    nxt = nxt->next;
    free(curr);
    curr = nxt;
  }
}