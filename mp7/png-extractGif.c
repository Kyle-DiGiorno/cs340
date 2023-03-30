#include <stdio.h>
#include <stdlib.h>
#include "lib/png.h"
#include <string.h>
#include <errno.h>

int png_extractGIF(const char *png_filename, const char *gif_filename)
{
  PNG *inp = PNG_open(png_filename, "r");
  // PNG* out = PNG_open(gif_filename, "w");
  //PNG_Chunk *chunk = malloc(10000000);
  FILE *fout = fopen(gif_filename, "a");
  fseek(fout, 0, SEEK_SET);
  fprintf(stderr, "b\n");
  while (1)
  {
    PNG_Chunk chunk;
    if (PNG_read(inp, &chunk) == 0) {
      PNG_close(inp);
      return ERROR_INVALID_CHUNK_DATA;
    }
    printf("Chunk: %s (%d bytes of data)\n", chunk.type, chunk.len);
    if ( strcmp(chunk.type, "IEND") == 0 ) {
      PNG_free_chunk(&chunk);
      break;  
    }
    if ( strcmp(chunk.type, "UIUC") == 0 || strcmp(chunk.type, "uiuc") == 0 ) {
      printf("Chunk: %s (%d bytes of data)\n", chunk.type, chunk.len);
      printf(chunk.type);
      printf("length:%d\n",chunk.len);
      printf("data:\n");
      printf("%d\n",chunk.data);
      printf("\nendData\n");
      //unsigned char* out_mem= malloc(sizeof(char*)*chunk.len);
      //memcpy(out_mem,chunk.data,chunk.len); 
      int count = 0;
     // printf("String length%d\n",strlen(chunk.data));
      // for(int u = 0; u < chunk.len; u++){
      //  // printf("%d\n",u);
      //   if(count < 30 && *(chunk.data+u)>0){
      //     printf("nz:%u,%d\n",*(chunk.data+u),u);
      //     count++;
      //   }
      //  //fwrite(out_mem[u], 1,1, fout);
      // }
      // unsigned char* end = chunk.data+chunk.len ;
      // unsigned char* iter = chunk.data;
      // int u = 0;
      
         
          //fwrite(iter, 1,1, fout);
          // if(iter == 0){
            
          // }
      int k = fwrite(chunk.data,1,chunk.len,fout);
      // printf("length on write: %d\n",k);
      // while( u < chunk.len )
      // {     char* out = iter;//fgetc(iter);
      //     printf("u:%d,out%c\n",u,out);
      //     unsigned char ch = fgetc(fout);
      //     if(ch == EOF){
      //       break;
      //     }
      //     printf("%u\n", ch);
      //     iter++ ;
      //     u++;
      // }
      //printf("om_%s\n",out_mem);
      
      break;  
    }
    
    // fprintf(stderr, "c\n");
    // PNG_read(inp, chunk);
    // fprintf(stderr, "d\n");
    // if (chunk->type[0] == 'u' && chunk->type[1] == 'i' && chunk->type[2] == 'u' && chunk->type[3] == 'c')
    // {
    //   fprintf(stderr, "%s\n", gif_filename);
    //   fwrite(chunk->data, chunk->len, 1, fout);
    //   fprintf(stderr, "f\n");
    // }
    // fprintf(stderr, "a\n");
  }
  // fseek(fout, 0, SEEK_SET);
  // char ch = fgetc(fout);

  //   while (ch != EOF) {
  //       ch = fgetc(fout);
  //       printf("%c\n", ch);
  //   } 
  PNG_close(inp);
  return 0; // Change the to a zero to indicate success, when your implementaiton is complete.
}

