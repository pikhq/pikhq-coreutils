#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int all_slashes(const char *string, const size_t string_len);
static int any_slashes(const char *string, const size_t string_len);

static void basename(char *string, const char *suffix)
{
  if(!strcmp("", string)) {
    printf(".");
    return;
  }

  size_t string_len = strlen(string);
  if(all_slashes(string, string_len)) {
    printf("/");
    return;
  }
  
  // Strip off trailing slashes
  for(int i = string_len - 1; i != 0 && string[i] == '/'; i--)
    string[i] = 0;

  // Strip off leading slashes
  char *new_string = NULL;
  while((new_string = strchr(string, '/')) != NULL)
    string = new_string + 1;

  // Strip off the suffix, if any.
  if(strcmp(string, suffix)) {
    char *suffix_end = strchr(suffix, 0);
    char *string_end = strchr(string, 0);
    while(suffix_end != suffix && string_end != string && *string_end-- == *suffix_end--);
    if(suffix_end == suffix)
      *string_end = 0;
  }
  printf("%s", string);
}

static int all_slashes(const char *string, const size_t string_len)
{
  for(int i = 0; i != string_len; i++)
    if(string[i] != '/')
      return 0;
  return 1;
}

static int any_slashes(const char *string, const size_t string_len)
{
  for(int i = 0; i!= string_len; i++)
    if(string[i] == '/')
      return 1;
  return 0;
}

int main(int argc, char **argv)
{
  if(argc <= 1) {
    fprintf(stderr, "%s: Too few arguments.\n", argv[0]);
    return 1;
  } else if(argc == 2)
    basename(argv[1], "");
  else
    basename(argv[1], argv[2]);
  printf("\n");
}
