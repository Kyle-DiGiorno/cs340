#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "lib/wallet.h"

wallet_t wallet;


void create_wallet_server(int port) {
}

int main(int argc, char* argv[]) {
  int c;
  int local_port = 34000;
  while((c = getopt(argc, argv, "p:")) != -1) {
    switch(c) {
      case 'p':
        if(optarg != NULL) {
          local_port = atoi(optarg);
        }
        break;
    }
  }

  wallet_init(&wallet);
  printf("Launching wallet server on :%d\n", local_port);
  create_wallet_server(local_port);
}
