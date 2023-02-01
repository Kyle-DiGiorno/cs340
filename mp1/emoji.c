#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Return your favorite emoji.  Do not allocate new memory.
// (This should **really** be your favorite emoji, we plan to use this later in the semester. :))
char *emoji_favorite()
{
  return "👺";
}

// Count the number of emoji in the UTF-8 string `utf8str`, returning the count.  You should
// consider everything in the ranges starting from (and including) U+1F000 up to (and including) U+1FAFF.
int utf_charlen_helper(unsigned char utf8str)
{
  // printf("%d", utf8str);
  unsigned long p = (unsigned long)(utf8str);
  if (!p)
  {
    return 1;
  }
  // printf("num:%u", p);
  if (p && p > 127)
  {
    printf("start");
    if (p % 128 > 63)
    {
      if (p % 64 > 31)
      {
        if (p % 32 > 15)
        {
          printf("GOT HERE");
          return 4;
        }
        return 3;
      }
      return 2;
    }
    return 1;
  }
  printf("uuu");
  return 1;
}
int emoji_count(const unsigned char *utf8str)
{
  int tot_chars = 0;
  int skip_count = 1;
  // unsigned char *oo = malloc(1000);
  // strcpy(oo, &utf8str);
  // printf(oo);
  // printf(strlen(oo));
  if (!utf8str)
  {
    printf("bozo");
    return 0;
  }
  // printf(strtoul("🎉"));
  printf("%d", sizeof(utf8str));
  for (int i = 0; i < sizeof(utf8str) - 1; i += skip_count)
  {
    //
    printf(utf8str);
    skip_count = utf_charlen_helper(utf8str[i]);
    if (skip_count >= 4)
    {
      printf("SKIP:%d", skip_count);
      unsigned long p = 0;
      char start[4];
      char *stop = malloc(100);
      strncpy(start, &utf8str[i], skip_count);
      printf(start);
      // memcpy(p, &utf8str[i], skip_count);
      p = strtoul(start, &stop, 2);
      printf("\nlll");
      printf(p);
      printf("%lu", (unsigned long)p);
      printf("\nt");
      if ((unsigned long)(p) >= 0x3146303030 && (unsigned long)(p) <= 0x3146414646)
      { // 552b
        tot_chars++;
      }
    }
  }
  return tot_chars;
}

// Return a random emoji stored in new heap memory you have allocated.  Make sure what
// you return is a valid C-string that contains only one random emoji.
char *emoji_random_alloc()
{
  return (char *)(rand() % (0x3146414646 - 0x3146303030) + 0x3146303030);
}

// Modify the UTF-8 string `utf8str` to invert the FIRST character (which may be up to 4 bytes)
// in the string if it the first character is an emoji.  At a minimum:
// - Invert "😊" U+1F60A ("\xF0\x9F\x98\x8A") into ANY non-smiling face.
// - Choose at least five more emoji to invert.
void emoji_invertChar(unsigned char *utf8str)
{
  if (utf8str == '🙂')
  {
    utf8str = '🙃';
    return;
  }
  if (utf8str == '🙃')
  {
    utf8str = '🙂';
    return;
  }
  if (utf8str == '🥶')
  {
    utf8str = '🥵';
    return;
  }
  if (utf8str == '🥵')
  {
    utf8str = '🥶';
    return;
  }
  if (utf8str == '😎')
  {
    utf8str = '🤓';
    return;
  }
  if (utf8str == '😊')
  {
    utf8str = '🤕';
    return;
  }
}

// Modify the UTF-8 string `utf8str` to invert ALL of the character by calling your
// `emoji_invertChar` function on each character.
void emoji_invertAll(unsigned char *utf8str)
{
  int skip_count = 1;
  for (int i = 0; i < strlen(utf8str); i += skip_count)
  {
    skip_count = utf_charlen_helper(utf8str[i]);
    if (skip_count > 2)
    {
      char p[skip_count];
      memcpy(p, &utf8str[i], skip_count);
      emoji_invertChar(p);
      for (int j = 0; j < 4; j++)
      {
        utf8str[i + j] = p[j];
      }
    }
  }
}

// Reads the full contents of the file `fileName, inverts all emojis, and
// returns a newly allocated string with the inverted file's content.
unsigned char *emoji_invertFile_alloc(const char *fileName)
{
  // adapted from https://pubs.opengroup.org/onlinepubs/9699919799/functions/getdelim.html
  FILE *fp;
  char *line = NULL;
  char *pre_invert = NULL;
  size_t len = 0;
  ssize_t read;
  fp = fopen(fileName, "r");
  if (fp == NULL)
    exit(1);
  while ((read = getline(&line, &len, fp)) != -1)
  {
    pre_invert = strcat(pre_invert, line);
  }
  if (ferror(fp))
  {
    return NULL;
  }
  fclose(fp);
  free(line);
  emoji_invertAll(pre_invert);
  return pre_invert;
}
