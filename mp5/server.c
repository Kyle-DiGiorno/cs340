#include "http.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/time.h>

int globe = 0;
int globe_c = 0;
void read_s(int sockfd)
{
  char *to_add = malloc(sizeof(char));
  while (read(sockfd, to_add, 1) >= 1)
  {
    printf("h");
  }
  printf("done");
}
void *client_thread(void *vptr)
{
  printf("HELP 99\n");
  int fd = *((int *)vptr);
  // printf("init %d", fd);
  globe++;
  // char *ni = malloc(5);
  HTTPRequest *rq = malloc(sizeof(HTTPRequest));
  globe_c++;
  httprequest_read(rq, fd);
printf("new\n");
  if ((rq == NULL))
  {
    printf("null\n");
    close(fd);
    // printf("close %d", fd);
    // globe_c++;
    return;
  }
  else if (rq->path == NULL || strlen(rq->path) < 1)
  {
    printf("no path\n");
    close(fd);
    // printf("close %d", fd);
    // globe_c++;
    return;
  }
  // globe_c++;
  char *index_out;

  /*while (rq == NULL || rq->path == NULL)
  {
    sleep(1);
  }*/

  // void *k = rq->path;
  printf("comparing\n");
  if (strcmp(rq->path, "/") == 0)
  {
    printf("sust /\n");
    index_out = malloc(strlen("static/index.html") + 3);
    strcpy(index_out, "static/index.html");
  }
  else
  {
    printf("path static\n");
    index_out = malloc(strlen(rq->path) + strlen("static") + 2);
    strcpy(index_out, "static");
    strcat(index_out, rq->path);
  }
  printf("staticed\n");
  FILE *dp = fopen(index_out, "r");
  if ((dp) == NULL)
  {
    printf("couldnt open\n");
    void *response = malloc(strlen(rq->version) + strlen("404 Not Found") + 2);
    memcpy(response, rq->version, strlen(rq->version));
    memcpy(response + strlen(rq->version), " ", 1);
    memcpy(response + strlen(rq->version) + 1, "404 Not Found", strlen("404 Not Found"));
    close(fd);
    // printf("close %d", fd);
    globe_c++;
    return response;
  }
  printf("opened\n");
  char *type = malloc(strlen("image/png") + 2);
  // char *path = malloc(strlen(rq->path));
  // strcpy(path, rq->path);
  const char ch = '.';
  char *p = strrchr(index_out, ch);
  if (strcmp(p, ".png") == 0)
  {
    strcpy(type, "image/png");
  }
  else if (strcmp(p, ".html") == 0)
  {
    strcpy(type, "text/html");
  }
  printf("filetyped\n");
  void *content = malloc(sizeof(char));
  int read_char = 0; // malloc(sizeof(int));
  int j = 0;
  while (read_char != EOF)
  {
    read_char = fgetc(dp);
    content = realloc(content, ((j + 1) * sizeof(char)));
    memcpy(content + (j * sizeof(char)), (char *)&read_char, sizeof(char));
    j++;
  }
  int ti = 0;
  printf("WE ARE\n");
  int u = strlen(rq->version) + strlen("400 Bad R equest") + 2 + strlen("Content-Type") + 2 + strlen(type) + 4 + sizeof(content) + 100 + j;
  char *response = malloc(u);
  memcpy(response, rq->version, strlen(rq->version));
  int op = strlen(rq->version);
  memcpy(response + op, " ", 1);
  op++;
  memcpy(response + op, "200 OK", strlen("200 OK"));
  op += strlen("200 OK");
  memcpy(response + op, "\r\n", 2);
  op += 2;
  memcpy(response + op, "Content-Type", strlen("Content-Type"));
  op += strlen("Content-Type");
  memcpy(response + op, ": ", 2);
  op += 2;
  memcpy(response + op, type, strlen(type));
  op += strlen(type);
  memcpy(response + op, "\r\n\r\n", 4);
  op += 4;
  // if(rq->c_len = 0){
  //   rq->c_len = j-1;
  // }
  // memcpy(response + op, content, rq->c_len);
  // op += rq->c_len;
  // if(rq->c_len > 0){
  //   memcpy(response + op, content, rq->c_len);
  //   op += rq->c_len;
  // } else {
  printf("the only explanation\n");
    char* ip = httprequest_get_header(rq,"Content-Length");
    if(ip){
      int c_len = atoi(ip);
      memcpy(response + op, content, c_len);
      op += c_len;
    } else {
      memcpy(response + op, content, j-1);
      op += j-1;
    }
  //}
  // memcpy(response + op, "\r\n\r\n", 4);
  // op += 4;

  // void *response = malloc(strlen(index_out));

  // close(fd);
  //  printf("close %d", fd);
  globe_c++;
  httprequest_destroy(rq);
  write(fd, response, op);
  //  read_s(fd);
  close(fd);
  return response;
  // return -1;
  // return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    return 1;
  }
  int port = atoi(argv[1]);
  printf("Binding to port %d. Visit http://localhost:%d/ to interact with your server!\n", port, port);

  // socket:
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

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
  while (1) {
    int *fd = malloc(sizeof(int));
    client_addr_len = sizeof(struct sockaddr_in);
    *fd = accept(sockfd, (struct sockaddr *)&client_address, &client_addr_len);
    printf("Client connected (fd=%d)\n", *fd);

    pthread_t tid;
    pthread_create(&tid, NULL, client_thread, fd);
    pthread_detach(tid);
  }
  
  return 0;
}