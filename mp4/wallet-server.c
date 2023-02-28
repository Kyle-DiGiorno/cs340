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
#include <fcntl.h>

#include "lib/wallet.h"

// Your server must use only one global wallet:

int setnonblock(int sock) {
   int flags;
   flags = fcntl(sock, F_GETFL, 0);
   if (-1 == flags)
      return -1;
   return fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}

wallet_t wallet;
void* parse_request(void* inp, int len){
  printf("BEGUN PARSING\n");
  printf(inp);
  printf("BEGUN PARSING2\n");
  char gt[3] = "GET";
  char md[3] = "MOD";
  // gt[0] = 'G';
  // gt[0] = 'E';
  // gt[0] = 'T';
  
  printf("%d\n",memcmp(inp,gt,2));
  if(memcmp(inp,gt,3)==0){
    printf("p1\n");
    void* buf[len - 4];
    printf("p2\n");
    memccpy(buf,inp+4,'\0', len-4);
    printf("p3\n");
    char ret_str[sizeof(int) + 1];
    printf("p4\n");
    sprintf(ret_str,"%d",wallet_get(&wallet, buf));
    printf("p5\n");
    strcat(ret_str, "\n");
    printf("p6\n");
    return(ret_str);
  }
   else if(memcmp(inp,md,3)==0){
    printf("in mod\n");
    void* resc_buf = malloc(sizeof(char)*len);
    int i = 0;
    for(i = 0; memcmp(inp+i,' ', 1) != 0; i++){
      if(i > len){
        return "EXIT";
      }
      //buf = realloc(sizeof(char)*i);
      memcpy(resc_buf + i, inp+4+i,1);
      printf(resc_buf);
    }

    memcpy(resc_buf + i + 1, '\0',1);
    void* delta_buf[len - i];
    memccpy(delta_buf,inp+4+i,'\0', len-4-i);
    char ret_str[sizeof(int) + 1];
    printf("m6\n");
    sprintf(ret_str,"%d",wallet_change_resource(&wallet, resc_buf,delta_buf));
    strcat(ret_str, "\n");
    return(ret_str);
  }
  else if(memcmp(inp,'EXIT',4)==0){
    return "EXIT";
  } else {
    return "EXIT";
  }
}
void* parse_input(void *vptr){
  char *to_add = malloc(sizeof(char));
  strcpy(to_add, "\0");
  int i = 0;
  void *in_str = malloc(sizeof(char));
  int sockfd = *((int *)vptr);
  setnonblock(sockfd);
  printf("in 0\n");
  int last_request = 0;
  int terminated = 0;
  while (read(sockfd, to_add, 1) >= 1)
  {
    // i++;
    printf("in 1\n");
    in_str = realloc(in_str, (i + 1) * sizeof(char));
    memcpy(in_str + i, to_add, 1);
    printf(in_str);
    if(memcmp(in_str + i - 1, "\n", 1) == 0){
      terminated = 1;
      printf("TTYN");
      printf(in_str);
      void* k = parse_request(in_str + last_request, i-last_request);
      if(memcmp(k,"EXIT",4)==0){
        close(sockfd);
      }
      else{
        return k;
      }
      last_request = i;
    }
    //printf("\n remain: %d",read(sockfd, to_add, 1));
    printf("\n");
    printf(in_str + i - 1);
    i++;
  }

  printf("\nto add:");
  printf(to_add);
  if(!terminated){
    void* k = parse_request(in_str, sizeof(in_str)/sizeof(char));
      if(memcmp(k,"EXIT",4)==0){
        close(sockfd);
      }
      else{
        return k;
      }
      last_request = i;
  }
}

void create_wallet_server(int port) {
  // Implement `create_wallet_server`
  // (You will need to use threads, which requires adding additional functions.)
  // char *to_add = malloc(sizeof(char));
  // int sockfd, bytes, sent, received, total;

  // sockfd = socket(AF_INET, SOCK_STREAM, 0);
  // if (sockfd < 0) error("ERROR opening socket");

  // while (read(sockfd, to_add, 1) >= 1)
  // {
  // }
  printf(stderr,"0\n");
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  printf(stderr,"1\n");
  // bind:
  struct sockaddr_in server_addr, client_address;
  memset(&server_addr, 0x00, sizeof(server_addr));
  printf(stderr,"2\n");
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);
  printf(stderr,"3\n");
  bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
  printf(stderr,"4\n");
  // listen:
  listen(sockfd, 10);
  printf(stderr,"5\n");
  // accept:
  socklen_t client_addr_len;
  printf(stderr,"6\n"); // = malloc(sizeof(client_addr_len));
  while (1)
  {
    printf(stderr,"7\n");
    int *fd = malloc(sizeof(int));
    //int status = fcntl(sockfd, F_SETFL, O_NONBLOCK);
    // struct timeval tv = {
    //     .tv_sec = 1};

    //setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(tv));
    // SetSocketBlockingEnabled(fd, 1);
    /*if (client_addr_len < 1)
    {
      continue;
    }*/
    *fd = accept(sockfd, (struct sockaddr *)&client_address, &client_addr_len);
    printf(stderr,"8\n");
    if (*fd < 0)
    {
      continue;
    }
    printf("Client connected (fd=%d)\n", *fd);

    pthread_t tid;
    printf(stderr,"9\n");
    void* k = pthread_create(&tid, NULL, parse_input(fd), fd);
    printf(stderr,"10\n");
    pthread_detach(tid);
    return k;
  }
}

int main(int argc, char* argv[]) {
  int c;
  int local_port = 34000;

  // Reads the (optional) command line argument:
  while((c = getopt(argc, argv, "p:")) != -1) {
    switch(c) {
      case 'p':
        if(optarg != NULL) {
          local_port = atoi(optarg);
        }
        break;
    }
  }

  // Calls `wallet_init`:
  wallet_init(&wallet);

  // Calls the `create_wallet_server` with the user-supplied port:
  printf("Launching wallet server on :%d\n", local_port);
  create_wallet_server(local_port);
}
