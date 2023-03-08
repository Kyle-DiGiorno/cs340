#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <netinet/in.h>
#include "http.h"

#include <pthread.h>


/**
 * httprequest_parse_headers
 *
 * Populate a `req` with the contents of `buffer`, returning the number of bytes used from `buf`.
 */
ssize_t httprequest_parse_headers(HTTPRequest *req, char *buffer, ssize_t buffer_len)
{
  if (buffer_len == 0)
  {
    req = NULL;
    return 0;
  }
  int has_payload = 0;
  int count = 0;
  req->action = malloc(sizeof(char));
  req->path = malloc(sizeof(char));
  req->version = malloc(sizeof(char));
  //req->c_len = 0;
  // char* out[3]; //{'\0', '\0', '\0', '\0', '\0'};
  // for (int i = 0; i < 3; i++)
  // {
  //   out[i] = malloc(buffer_len * sizeof(char));
  //   //*out[i] = '\0';
  // }

  req->key = calloc(1,sizeof(char *));
  req->value = calloc(1,sizeof(char *));
  int switch_delim = 0;
  int i = 0;

  for (i = 0; i < buffer_len; i++)
  {
    // if (count >= 6)
    //{
    //   break;
    // }
    if (strncmp(buffer + i, " ", 1) == 0 && count < 3)
    {
      count++;
      continue;
    }
    else if (strncmp(buffer + i, "\r\n\r\n", 4) == 0)
    {
      break;
    }
    else if (strncmp(buffer + i, "\r\n", 2) == 0 && count > 1)
    {
      // strncat(req->key[count-2], (buffer + i), 1)
      switch_delim = 0;
      count++;
      i++;

      req->key = realloc(req->key, buffer_len * sizeof(char *) * (count));
      req->key[count - 3] = malloc(buffer_len - i);
      strcpy(req->key[count - 3], "\0");
      continue;
    }
    else if (strncmp(buffer + i, ": ", 2) == 0 && count > 2)
    {
      // count++;
      i++;
      switch_delim = 1;

      req->value = realloc(req->value, buffer_len * sizeof(char *) * (count));
      req->value[count - 3] = malloc(buffer_len - i);
      strcpy(req->value[count - 3], "\0");
      continue;
    }

    /*if (out[count] == NULL)
    {
      strncpy(out[count], buffer, 1);
      continue;
    }*/
    if (count < 3)
    {
      if(count == 0){
        realloc(req->action, sizeof(req->action)+sizeof(char));
        strncat( req->action, (buffer + i), 1);
      }
      if(count == 1){
        realloc(req->path, sizeof(req->path)+sizeof(char));
        strncat( req->path, (buffer + i), 1);
      }
      if(count == 2){
        realloc(req->version, sizeof(req->version)+sizeof(char));
        strncat( req->version, (buffer + i), 1);
      }
      //strncat(out[count], (buffer + i), 1);
    }
    else
    {
      if (switch_delim)
      {
        strncat(req->value[count - 3], (buffer + i), 1);
      }
      else
      {
        strncat(req->key[count - 3], (buffer + i), 1);
      }
    }
  }
  // req->key = out[3];
  // req->value = out[4];
  // req->key_size = malloc(sizeof(size_t));
  req->key_size = count - 2;
  // req->payload = malloc(buffer_len);
  //req->payload = malloc(buffer_len);
  //
  return i + 4;
}
size_t TimeoutRead (int port, void*buf, size_t size, int mlsec_timeout)
{
    struct pollfd fd = { .fd = port, .events = POLLIN };

    size_t      bytesread = 0;

    while (poll (&fd, 1, mlsec_timeout) == 1)
    {
      printf("Gune");
        int chunksize = read (port, buf + bytesread, size);
        if (chunksize == -1)
            return -1;

        bytesread += chunksize;
        size -= chunksize;

        if (size == 0)
            return bytesread;
    }

    // TODO: IsTimeout = true;
    return bytesread;
}
/**
 * httprequest_read
 *
 * Populate a `req` from the socket `sockfd`, returning the number of bytes read to populate `req`.
 */
ssize_t httprequest_read(HTTPRequest *req, int sockfd)
{
  printf("READING 77\n");
  // send(sockfd, 'a', sizeof('a'), 0);
  int flip = 0;
  void *in_str = malloc(sizeof(char));
  // strcpy(in_str, "\0");
  char *to_add = malloc(sizeof(char));
  printf("READING 66\n");
  strcpy(to_add, "\0");
  int i = 0;
  int precamp = 0;
  int plen = 0;
  int c_len = 0;
  printf("READING 55\n");
  // if(req->key_size > 0){
  //   printf("Here0\n");
  //   char* ip = httprequest_get_header(req,"Content-Length");
  //   printf("Here\n");
  //   c_len = atoi(ip);
  // }
  struct timeval tv = {
        .tv_sec = 1};
    // printf("2\n");
    // setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(tv));

  while (read(sockfd, to_add, 1) >= 1)
  {
    //printf("YHJK\n");
    if(precamp){
      plen++;
    }
    // i++;
    in_str = realloc(in_str, (i + 1) * sizeof(char));
    /* if (*to_add != NULL)
     {
       strncat(in_str, to_add, 1);
     }
     else
     {*/
    if (i % 98 == 0)
    {
      int y = 0;
    }
    memcpy(in_str + i, to_add, 1);
    //}
    if (in_str && i > 3)
    {
      int k = memcmp(in_str + i - 3, "\r\n\r\n", 4);
      if (memcmp(in_str + i - 3, "\r\n\r\n", 4) == 0)
      {
        // send(sockfd, 'a', sizeof('a'), 0); // int ur = read(sockfd, to_add, 1);
        //  in_str = realloc(in_str, (i + 5) * sizeof(char));
        //  memcpy(in_str + i, "\r\n\r\n", 4);
        //  break;
        precamp = 1;
        int t = 0;
        break;
      }
      if (flip > 1)
      {
        break;
      }
    }
    /*if (in_str && sizeof(in_str) > 4 && memcmp(in_str + sizeof(in_str) - 4, "\r\n\r\n", 4) == 0)
    {
      break;
    }*/
    i++;
    //free(in_str);
  }
  // in_str = realloc(sizeof(char)*i);
  // read()
  //  HTTPRequest *request = (HTTPRequest *)malloc(sizeof(HTTPRequest));
  // if (i > 0)
  //{
  // req = NULL;
  //printf(req->key);
  // for(int t = 0; t < req->key_size;t++){
  //   // char* k = (req->key[t]);
  //   printf(req->key[t]);
  //   if(strcmp(req->key[t],"Content-Length:")==0){
  //     req->c_len = req->value[t];
  //   }
  // }
  int con_len = 0;
  httprequest_parse_headers(req, in_str, i);
  printf("start packing\n");
  char* ip = httprequest_get_header(req,"Content-Length");
  printf("WHAT\n");
  printf(ip);
  printf("\n");
  if(ip){
    printf("HOW%d\n",con_len);
    con_len = atoi(ip);
    printf("HOW%d\n",con_len);
    printf("atoi\n");
    if(con_len > 0){ 
    printf("LEFT\n"); 
     req->payload = malloc(con_len*sizeof(char));
     printf("malloced\n");
     int pcl = 0;
     while(con_len > 0){
      ssize_t tr = read(sockfd, req->payload+pcl, con_len);
      printf("size:%d\n",tr);
      printf("%.2s",req->payload);
      con_len-=tr;
      pcl += tr;
     }
     
     printf("read\n");
    } else{
      req->payload = NULL;
    }
  } else{
    //
    req->payload = NULL;
  }
  printf("LEFT 7 \n");
  //}
  //close(sockfd);
  free(to_add);
  free(in_str);
  return i+con_len;
}


/**
 * httprequest_get_action
 *
 * Returns the HTTP action verb for a given `req`.
 */
const char *httprequest_get_action(HTTPRequest *req)
{
  return req->action;
}

/**
 * httprequest_get_header
 *
 * Returns the value of the HTTP header `key` for a given `req`.
 */
const char *httprequest_get_header(HTTPRequest *req, const char *key)
{
  for (int i = 0; i < req->key_size; i++)
  {
    printf(req->key[i]);
    printf("\n");
    if (strcmp(req->key[i], key) == 0)
    {
      printf("gonna return\n");
      return req->value[i];
    }
  }
  return NULL;
}

/*
 * httprequest_get_path
 *
 * Returns the requested path for a given `req`.
 */
const char *httprequest_get_path(HTTPRequest *req)
{
  return req->path;
}

/**
 * httprequest_destroy
 *
 * Destroys a `req`, freeing all associated memory.
 */
void httprequest_destroy(HTTPRequest *req)
{
  
  for (int i = 0; i < req->key_size; i++)
  {
    free(req->key[i]);
    free(req->value[i]);
  }
  free(req->key);
  free(req->value);
  free(req->payload);
  // free(req->payload_copy);
}