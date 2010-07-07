static int base64_main(int argc, char **argv);
#define main base64_main
#include "base64/base64.c"
#undef main
static int basename_main(int argc, char **argv);
#define main basename_main
#include "basename/basename.c"
#undef main
static int cal_main(int argc, char **argv);
#define main cal_main
#include "cal/cal.c"
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
  while(argc != 0) {
    if(!strcmp(argv[0], "base64"))
      return base64_main(argc, argv);
    if(!strcmp(argv[0], "basename"))
      return basename_main(argc, argv);
    if(!strcmp(argv[0], "cal"))
      return cal_main(argc, argv);
    if(!strcmp(argv[0], "cat"))
      return cat_main(argc, argv);
    if(!strcmp(argv[0], "cksum"))
      return cksum_main(argc, argv);
    argv++;
    argc--;
  }
  return 1;
}
