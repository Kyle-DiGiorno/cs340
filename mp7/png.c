#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "crc32.h"
#include "png.h"

const int ERROR_INVALID_PARAMS = 1;
const int ERROR_INVALID_FILE = 2;
const int ERROR_INVALID_CHUNK_DATA = 3;
const int ERROR_NO_UIUC_CHUNK = 4;

/**
 * Opens a PNG file for reading (mode == "r" or mode == "r+") or writing (mode == "w").
 *
 * (Note: The function follows the same function prototype as `fopen`.)
 *
 * When the file is opened for reading this function must verify the PNG signature.  When opened for
 * writing, the file should write the PNG signature.
 *
 * This function must return NULL on any errors; otherwise, return a new PNG struct for use
 * with further fuctions in this library.
 */
PNG *PNG_open(const char *filename, const char *mode)
{
  FILE *op_return = fopen(filename, mode);
  if (op_return == NULL)
  {
    printf("hrm  here");
    return NULL;
  }

  char sig[] = {137, 80, 78, 71, 13, 10, 26, 10};
  // fseek(op_return, 0, SEEK_SET);
  char read_sig[sizeof(sig)];
  fread(read_sig, sizeof(sig), 1, op_return);

  if (*mode == 'r' || *mode == 'r+')
  {
    for (int i = 0; i < sizeof(sig); i++)
    {
      int p = 0;
      if (read_sig[i] != sig[i])
      {
        // free(out);
        return NULL;
      }
    }
  }
  else if (*mode == 'w')
  {
    fseek(op_return, 0, SEEK_SET);
    fwrite(sig, 8, 1, op_return);
    // fseek(op_return, 0, SEEK_SET);
    // fread(read_sig, sizeof(sig), 1, op_return);
    int b = 9;
  }
  fseek(op_return, 0L, SEEK_END);
  int size = ftell(op_return);
  PNG *out = malloc(100 * size + 16);

  out->file_c = op_return;
  out->chunk_arr = NULL;
  // fseek(op_return, 8, SEEK_END); // seek to end of file
  /*int size = ftell(op_return);   // get current file pointer
  fseek(op_return, 0, SEEK_SET);
  out->file_c = malloc(size);
  out->file_c = memcpy(out->filmake_c, op_return, size);*/

  out->chunk_count = 0;
  out->mem_c = 0;
  out->sec_p = 10;
  out->file_pos = 8;
  out->arr_last = NULL;
  return out;
}

/**
 * Reads the next PNG chunk from `png`.
 *
 * If a chunk exists, a the data in the chunk is populated in `chunk` and the
 * number of bytes read (the length of the chunk in the file) is returned.
 * Otherwise, a zero value is returned.
 *
 * Any memory allocated within `chunk` must be freed in `PNG_free_chunk`.
 * Users of the library must call `PNG_free_chunk` on all returned chunks.
 */
size_t PNG_read(PNG *png, PNG_Chunk *chunk)
{
  if (!png)
  {
    return 0;
  }
  int is_data = 0;
  // fseek(png->file_c, 4, SEEK_CUR);
  //   FILE *chunk_reader = fopen(png->file_c, "r");
  //    u_int32_t *len;
  fseek(png->file_c, png->file_pos, SEEK_SET);
  // chunk->len = malloc(5);

  // chunk->len = 0;
  chunk->len = getw(png->file_c);
  chunk->len = ntohl(chunk->len);
  /*if (chunk->len == NULL)
  {
    return 0;
  }*/
  // char *type;
  fgets(chunk->type, 5, png->file_c);
  png->file_pos += 8;
  if (chunk->type == NULL)
  {
    return 0;
  }
  chunk->data = malloc((chunk->len));
  if (chunk->len > 0)
  {
    is_data = 1;
    int was_s = ftell(png->file_c);
    //fseek(png->file_c, png->file_pos, SEEK_SET);
    
    // if(strcmp(chunk->type, "uiuc") == 0 ){
    //   unsigned char to_data[chunk->len];
    //   unsigned char* u = malloc(1*sizeof(char));
    //   for(int to_remove = 0; to_remove < chunk->len; to_remove++){
    //     fread(u, 1,1, png->file_c);
    //     // if(u[0]>0){
    //     printf("%u\n",u);
    //     //   printf("%d\n",to_remove);
    //     // }
    //     //to_data[to_remove] = u;
    //   }
    //   memcpy(chunk->data,to_data,chunk->len);
    // } else{
    int k = fread(chunk->data, 1,chunk->len, png->file_c);
    //}
    printf("%son read length:%d\n",chunk->type,k);

    //printf("on read data:%s\n",chunk->data);
    // fseek(png->file_c, was_s + chunk->len, SEEK_SET);
    png->file_pos += chunk->len;
    if (chunk->data == NULL)
    {
      return 0;
    }
  }
  // uint32_t *crc;
  chunk->crc = getw(png->file_c);
  png->file_pos += 4;
  if (chunk->crc == NULL)
  {
    return 0;
  }
  chunk->crc = ntohl(chunk->crc);

  // chunk = {len, type, data, crc};
  // chunk->len = len;

  if (png->chunk_arr == NULL)
  {
    png->chunk_arr = malloc(chunk->len);
    if (chunk != NULL)
    {
      png->chunk_arr = malloc(sizeof(chunk) + 800);
      png->chunk_arr->chunk = chunk;
      png->chunk_arr->next = NULL;
      png->arr_last = png->chunk_arr;
    }
    else
    {
      int y = 0;
    }
  }
  else
  {
    if (png->arr_last)
    {
      fprintf(stderr, "%d\n", sizeof(png->arr_last));
      png->arr_last->next = malloc(sizeof(chunk) + 800);
      fprintf(stderr, "lezre\n");
      png->arr_last->next->chunk = chunk;
      png->arr_last->next->next = NULL;
      png->arr_last = png->arr_last->next;
    }
    else
    {
      int y = 0;
      fprintf(stderr, "how");
    }
  }
  png->mem_c++;

  /*png->chunk_arr = realloc(png->chunk_arr, png->mem_c);
  png->chunk_arr[sizeof(png->chunk_arr) - 1].len = chunk->len;
  // png->chunk_arr[sizeof(png->chunk_arr)-1].type = malloc(5);
  memcpy(png->chunk_arr[sizeof(png->chunk_arr) - 1].type, chunk->type, 5);
  png->chunk_arr[sizeof(png->chunk_arr) - 1].data = malloc(chunk->len);
  memcpy(png->chunk_arr[sizeof(png->chunk_arr) - 1].data, chunk->data, chunk->len);
  // memcpy(png->chunk_arr, chunk->data, chunk->len);
  png->chunk_arr[sizeof(png->chunk_arr) - 1].crc = chunk->crc;
  png->chunk_count++;
  // free(chunk_out);
  png->sec_p += 4 * 3 + (chunk->len);*/
  // png->chunk_arr;
  fprintf(stderr, "yoyoyo\n");
  return 4 * 3 + (chunk->len);
}

/**
 * Writes a PNG chunk to `png`.
 *
 * Returns the number of bytes written.
 */
size_t PNG_write(PNG *png, PNG_Chunk *chunk)
{
  // FILE *chunk_writer = png->file_c, "a";
  //  int t = realloc(png->chunk_arr, png->chunk_count+ 4 * 3 + chunk->len);
  //  if (t)
  //{
  char png_sig[8] = {137,80,78,71,13,10,26,10};
  fseek(png->file_c, 3, SEEK_END);
  char nl_len[4] = {htonl(chunk->len)>>24,htonl(chunk->len)>>16,htonl(chunk->len)>>8,htonl(chunk->len)};
  // uint32_t nl_crc[1] = {htonl(chunk->crc)};
  
  uint32_t* crc = malloc(16);
  //crc = chunk->crc;
  //nl_crc = htonl(chunk->crc);
  crc32(chunk->data, chunk->len*sizeof(char), crc);
  //crc32(chunk->data, sizeof(chunk->data), crc);
  char nl_crc[4] = {htonl(crc)>>24,htonl(crc)>>16,htonl(crc)>>8,htonl(crc)};
  printf(nl_crc);
  //{htonl(chunk->crc)>>24,htonl(chunk->crc)>>16,htonl(chunk->crc)>>8,htonl(chunk->crc)};
  // uint32_t *ncrc = htonl(chunk->crc);
  //fprintf(png->file_c, png_sig);
  fprintf(png->file_c, nl_len);
  fprintf(png->file_c, chunk->type);
  fprintf(png->file_c, chunk->data);
  fprintf(png->file_c, nl_crc);
  // memcpy(png->chunk_arr, chunk->len,4);
  // memcpy(png->chunk_arr, chunk->type,4);
  // memcpy(png->chunk_arr, chunk->data,chunk->len);
  // memcpy(png->chunk_arr, chunk->crc,4);
  return 4 * 3 + chunk->len;
  //}
  // return 0;
}

/**
 * Frees all memory allocated by this library related to `chunk`.
 */
void PNG_free_chunk(PNG_Chunk *chunk)
{
  if (chunk->data != NULL)
    free(chunk->data);
}

/**
 * Closes the PNG file and frees all memory related to `png`.
 */
void preeFree(PNG *png, ChunkArr *p)
{
  if (p != png->arr_last)
  {
    preeFree(png, p->next);
  }
  free(p);
}
void PNG_close(PNG *png)
{
  fclose(png->file_c);
  png->file_pos = 0;
  png->chunk_count = 0;
  png->mem_c = 0;
  png->sec_p = 0;
  /*for (int i = 0; i < png->chunk_count; i++)
  {
    PNG_free_chunk(png->chunk_arr + i);
  }*/
  // preeFree(png, png->chunk_arr);
  free(png->chunk_arr);
}
