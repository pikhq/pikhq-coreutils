static int basename_main(int argc, char **argv);
#define main basename_main
#include "basename/basename.c"
#undef main
static int cat_main(int argc, char **argv);
#define main cat_main
#include "cat/cat.c"
#undef main
static int cksum_main(int argc, char **argv);
#define main cksum_main
#include "cksum/cksum.c"
#undef main

int main(int argc, char **argv)
{
 start:
  if(!strcmp(argv[0], "basename"))
    return basename_main(argc, argv);
  if(!strcmp(argv[0], "cat"))
    return cat_main(argc, argv);
  if(!strcmp(argv[0], "cksum"))
    return cksum_main(argc, argv);
  argv++;
  argc--;
  if(argc == 0)
    return 1;
  goto start;
}
