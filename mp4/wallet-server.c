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
typedef struct socket_read
  {
    struct socket_read* nxt;
    int sockfd;
    sig_atomic_t done;
    sig_atomic_t exiting;
    pthread_mutex_t* socket_lock;
    pthread_cond_t* socket_conditional;
  } socket_read;
socket_read* sock_head = NULL;
socket_read* sock_tail = NULL;

socket_read* get_sock_read(int i){
  socket_read* curr = sock_head;
  while(curr){
    printf("%d\n",curr->sockfd);
    printf("%d\n",i);
    if(curr->sockfd == i){
      printf("%d\n",i);
      return curr;
    }
    curr = curr->nxt;
  }
  printf("OOPS\n");
  return NULL;
}
int set_fully_read(int i){
  socket_read* curr = sock_head;
  while(curr){
    if(curr->sockfd == i){
      curr->done = 1;
    }
    curr = curr->nxt;
  }
}
int send_data(int sockfd, char* data){
  printf("SOKET NUM:%d\n",sockfd);
  socket_read* sock = get_sock_read(sockfd);
  printf(data);
  printf("GOT SOCK\n");
  printf(sock);
  if(strcmp(data,"EXIT")!=0){
    printf("NOT EXIT\n");
    sock->exiting++;
    
  } else {
    printf("IS EXIT\n");
    set_fully_read(sockfd);
    pthread_cond_broadcast(&sock->socket_conditional);
  }
  pthread_mutex_lock(&sock->socket_lock);
  while( sock->done < 1){
    printf("WAITING FOR FULL READ\n");
    pthread_cond_wait(&sock->socket_conditional,&sock->socket_lock);
  }
  
  if(strcmp(data,"EXIT")==0 && sock->exiting > 1){
    printf("WAITING FOR FULL WRITE\n");
    pthread_cond_broadcast(&sock->socket_conditional);
    pthread_cond_wait(&sock->socket_conditional,&sock->socket_lock);
  }
  send(sockfd,data,sizeof(data),MSG_WAITFORONE);
  printf("SENT\n");
  pthread_mutex_unlock(&sock->socket_lock);
  pthread_cond_broadcast(&sock->socket_conditional);
  if(strcmp(data,"EXIT")==0){
    printf("CLOSING SOCKET\n");
    //close(sockfd);
  }

  
}
int setnonblock(int sock) {
   int flags;
   flags = fcntl(sock, F_GETFL, 0);
   if (-1 == flags)
      return -1;
   return fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}
void* timeout_read(int fdsock){
  sleep(100);
  //close(fdsock);
  return NULL;
}
wallet_t wallet;
void* parse_request(char* inp, int len, int sockfd){
  printf("BEGUN PARSING\n");
  printf(inp);
  printf("BEGUN PARSING2\n");
  char gt[3] = "GET";
  char md[3] = "MOD";
  char ext[4] = "EXIT";
  // gt[0] = 'G';
  // gt[0] = 'E';
  // gt[0] = 'T';
  printf("SOKET NUM:%d\n",sockfd);
  printf("%d\nt",strncmp(inp,"MOD",3));
  printf(inp);
  printf("t \n");
  if(strncmp(inp,"GET",3)==0){
    printf("p1\n");
    void* buf[len - 4];
    printf("p2\n");
    memccpy(buf,inp+4,'\0', len-4);
    printf("p3\n");
    int add_str = wallet_get(&wallet, buf);
    //char* ret_str = malloc(sizeof(int) + 1);
    printf("p4\n");
    int length = snprintf( NULL, 0, "%d", add_str );
    printf("m6.1\n");
    char ret_str[2*length + 1];
    printf("m6.11\n");
    snprintf( ret_str, length + 1, "%d", add_str );
    //sprintf(ret_str,"%d",wallet_get(&wallet, buf));
    printf("p5\n");
    strcat(ret_str, "\n");
    printf(ret_str);
    printf("p6\n");
    printf("SOKET NUM:%d\n",sockfd);
    pthread_t tid;
    // pthread_create(&tid, NULL, send_data(sockfd,ret_str), NULL);
    // pthread_detach(tid);
    printf("strlen:%d\n",strlen(ret_str));
    pthread_create(&tid, NULL, send(sockfd,ret_str,sizeof(ret_str),NULL), NULL);
    pthread_detach(tid);
    printf("p7\n");
    return (ret_str);
  } else if(strncmp(inp,"MOD",3)==0){
    printf("in mod\n");
    void* resc_buf = malloc(sizeof(char)*len);
    printf("malloc line\n");
    int i = 0;
    char spc[1] = " ";
    for(i = 0; strncmp(inp+4+i,spc, 1) != 0;){
      printf("\n%da\na",i);
      printf("wakku\n");
      if(i > len){
        printf("exiting\n");
        return "EXIT";
      }
      //buf = realloc(sizeof(char)*i);
      printf("resc_buf: \n");
      memcpy(resc_buf + i, inp+4+i,1);
      printf("resc_buf2: \n");
      printf(resc_buf);
      printf("resc_buf3: \n");
      i++;
      printf("\n%da\na",memcmp(inp+4+i,spc, 1));
    }
    printf("weve arrived\n");
    //memcpy(resc_buf + i + 1, '\0',1);
    printf("T1\n");
    char* delta_buf[len - i];
    
    strcpy(delta_buf,inp+4+i);
    printf(delta_buf);
    printf("T2\n");
    
    printf("m6\n");
    printf("SOKET NUM:%d\n",sockfd);
    int add_str = wallet_change_resource(&wallet, resc_buf,atoi(delta_buf));
    printf("m6.00\n");
    printf("%d\n",add_str);
    printf("m6.0\n");
    int length = snprintf( NULL, 0, "%d", add_str );
    printf("m6.1\n");
    char ret_str[2*length + 1];
    printf("m6.11\n");
    snprintf( ret_str, length + 1, "%d", add_str );
    //ret_str = itoa(add_str);
    printf("m6.5\n");
    printf(ret_str);
    strcat(ret_str, "\n");
    printf("m7\n");
    pthread_t tid;
    printf(ret_str);
    printf("tjij");
    // pthread_create(&tid, NULL, send_data(sockfd,ret_str), NULL);
    // pthread_detach(tid);
    printf("strlen:%d\n",strlen(ret_str));
    pthread_create(&tid, NULL, send(sockfd,ret_str,sizeof(ret_str),NULL), NULL);
    pthread_detach(tid);
    return (ret_str);
  }
  else if(strncmp(inp,ext,4)==0){
    printf("GOT HER\n");
    close(sockfd);
    return "EXIT";
  } else {
    return "EXIT";
  }
}
void* parse_input(int vptr){
  
  //strcpy(to_add, "\0");
  int i = 0;
  char *in_str = malloc(sizeof(char));
  int sockfd = vptr;
  pthread_mutex_t lock_read;
  pthread_mutex_init(&lock_read,NULL);
  printf("SOKET NUM:%d\n",sockfd);
  //setnonblock(sockfd);
  printf("in 0\n");
  int last_request = 0;
  int terminated = 0;
  int flag = 0;
  pthread_t tind;    // pthread_create(&tid, NULL, send_data(sockfd,ret_str), NULL);
    // pthread_detach(tid);
  //pthread_create(&tind, NULL, timeout_read(sockfd), NULL);
  //pthread_detach(tind);
  int full_count = 0;
  char* to_add = malloc(4096);
  //int j = 4096;
  while (1)
  {
    //j+=4*sizeof(char);
    //printf("LOOP START\n");
    //printf("sock %d\n",sockfd);
    //printf(to_add);
    // printf("a");
    if(read(sockfd, to_add, 0)){
      
      printf(": Peek value\n");
    }
    // printf("b");
    //read(sockfd, to_add, 1);
    // for(int u = 1; recv(sockfd, to_add, u,MSG_PEEK); u++){
    //   full_count = u;
    // }
    printf("d\n");
    //to_add = realloc(to_add,10*sizeof(char));
    //memcpy(to_add,"k",sizeof("k"));
    printf(to_add);
    printf(":-:%d\n",sizeof(to_add));
    //realloc(to_add, j);
    if(!recv(sockfd, to_add, 1,NULL)){
      
      printf("uh\n");
      continue;
    } else {
      printf("STARTED REAL\n");
    }
    // i++;
    //flag = 0;
    printf("in 1\n");
    in_str = realloc(in_str, (i + 1) * sizeof(char));
    memcpy(in_str + i, to_add, 1);
    printf(in_str);
    printf("\ncount: %d\n",memcmp(to_add, "\n", 1));
    if(memcmp(to_add, "\n", 1) == 0){
      flag = 1;
      terminated = 1;
      printf("TTYN");
      printf(in_str);
      printf("SOKET NUM:%d\n",sockfd);
      char* k = parse_request(in_str + last_request, i-last_request,sockfd);
      int p = 0;
      // while(recv(sockfd, to_add, 1,MSG_PEEK | O_NONBLOCK)){
      //   p++;
      //   if(p%100 == 0){
      //   printf(to_add);
      //   printf(" waiteing for empty\n");
      //   }
      // }
      // char buffer[4096];
      // int bytes_read = recv(sockfd, buffer, 4096, 0);
      // printf("BUFFER:");
      // printf(buffer);
      // char* k = malloc(sizeof(k)*2);
      // printf(k);
      // printf("\ntiy \n");
      // strcpy(k2,k);
      // printf(k);
      // printf("\ntiy2 \n");
      // strcat(k,"\n");
      printf("\ntoodles \n");
    //   if(strncmp(k,"EXIT",4)==0){
    //     close(sockfd);
    //     break;
    //  }
     printf("tre\n");
      // else{
      //   printf(k);
      //   printf("SOCKFDVAL: %d\n",sockfd);
      //   send(sockfd,k,sizeof(k),0);
      //   char buffer[4096];
      //   int bytes_read = recv(sockfd, buffer, 4096, 0);
      //   printf(buffer);
      //   printf(" sent\n");
      // }
      last_request = i;
    }
    //printf("\n remain: %d",read(sockfd, to_add, 1));
    printf("here\n");
    printf(in_str + i - 1);
    i++;
    printf("POSTED UP\n");
    // if(flag){
    //   printf(stderror(getsockopt (socket_fd, SOL_SOCKET, SO_ERROR, &error, &len)));
    // }
  
  }

  printf("\nto add:\n");
  // printf(to_add);
  // if(!terminated){
  //   char* k = parse_request(in_str, sizeof(in_str)/sizeof(char));
  //   printf("TYHJK\n");
  //   printf(k);
  //   printf("- k \n");
  //     if(memcmp(k,"EXIT",4)==0){
  //       close(sockfd);
  //     }
  //     else{

  //       return k;
  //     }
  //     last_request = i;
  // }
  return NULL;
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
  printf("0\n");
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  printf("1\n");
  // bind:
  struct sockaddr_in server_addr, client_address;
  memset(&server_addr, 0x00, sizeof(server_addr));
  
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);
  
  bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
 
  // listen:
  listen(sockfd, 10);
 
  // accept:
  socklen_t client_addr_len;
   // = malloc(sizeof(client_addr_len));
  while (1)
  {
   
    int *fd = malloc(sizeof(int));
    //int status = fcntl(sockfd, F_SETFL, O_NONBLOCK);
    // struct timeval tv = {
    //     .tv_sec = 1};

    //setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(tv));
    //SetSocketBlockingEnabled(fd, 1);
    /*if (client_addr_len < 1)
    {
      continue;
    }*/
    *fd = accept(sockfd, (struct sockaddr *)&client_address, &client_addr_len);
    printf("SOKET NUM:%d\n",fd);
  
    if (*fd < 0)
    {
      continue;
    }
    printf("start\n");
    if(!sock_head){
      printf("s null\n");
      socket_read n_sock = {.nxt = NULL,.sockfd = sockfd,.done = 0,.exiting = 0};
      printf("s2\n");
      printf("SOKET NUM:%d\n",fd);
      pthread_mutex_init(&n_sock.socket_lock, NULL);
      pthread_cond_init(&n_sock.socket_conditional, NULL);
      printf("s3\n");
      sock_head = &n_sock;
      sock_tail = &n_sock;
    } else {
      printf("s!null\n");
      socket_read n_sock = {.nxt = NULL,.sockfd = sockfd,.done = 0, .exiting = 0};
      printf("s2\n");
      pthread_cond_init(&n_sock.socket_conditional, NULL);
      pthread_mutex_init(&n_sock.socket_lock, NULL);
      sock_tail->nxt = &n_sock;
      sock_tail = &n_sock;
    }
    printf("Client connected (fd=%d)\n", *fd);

    pthread_t tid;
    
    //char* k = malloc(1000);
    //printf("%d\n",parse_input(fd));
    printf("SOKET NUM:%d\n",fd);
    pthread_create(&tid, NULL, parse_input(*fd), NULL);
    //pthread_join(&tid,k);
    //printf(k);
    printf(" 10\n");
    //pthread_join(tid,NULL);
    //return k;
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
