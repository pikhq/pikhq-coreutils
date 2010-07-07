/* Basic base64 util
 * Copyright (c) 2010 Josiah Worcester <josiahw@gmail.com>
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

static const char b64enc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
// The inverse of the above table, with '!' added to make the entire table printable.
static const char b64dec[] = "a!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!_!!!`UVWXYZ[\\]^!!!!!!!!\"#$%&'()*+,-./0123456789:!!!!!!;<=>?@ABCDEFGHIJKLMNOPQRST";

static void wrap(char c);
static void encode_chars(char s[3], size_t len)
{
  char a = s[0];
  char b = len >= 2 ? s[1] : 0;
  char c = len == 3 ? s[2] : 0;
  wrap(           b64enc[ a >> 2           & 0x3F]);
  wrap(           b64enc[(a << 4 | b >> 4) & 0x3F]);
  wrap(len >= 2 ? b64enc[(b << 2 | c >> 6) & 0x3F] : '=');
  wrap(len == 3 ? b64enc[ c                & 0x3F] : '=');
}

static void decode_chars(char s[4])
{
  int num_to_output = (int)(strchr(s, '=') - s);

  for(int i = 0; i != 4; i++)
    s[i] = s[i] != '=' ? b64dec[s[i]] - '!' : 0;
  if(num_to_output > 0)
    putchar(s[0] << 2 | s[1] >> 4);
  if(num_to_output > 1)
    putchar(s[1] << 4 | s[2] >> 2);
  if(num_to_output > 2)
    putchar(s[2] << 6 | s[3]);
}

static void encode(FILE *f)
{
  size_t len;
  char buf[3];
  while((len = fread(buf, 1, 3, f)) > 0)
    encode_chars(buf, len);
}

static void decode(FILE *f)
{
  char buf [4];
  while(1) {
    for(int i = 0; i != 4; i++) {
      buf[i] = getc(f);
      if(buf[i] == EOF) {
	buf[i] = '=';
	decode_chars(buf);
	return;
      } else if((buf[i] < 'A' || buf[i] > 'z') && (buf[i] < '/' || buf[i] > '9') && buf[i] != '+')
	i--;
    }
    decode_chars(buf);
    memset(buf, '=', 4);
  }
}

static void wrap(char c)
{
  static int cur_col;
  if(cur_col++ == 76) {
    cur_col = 1;
    putchar('\n');
  }
  putchar(c);
}

int main(int argc, char **argv)
{
  int decode_flag = 0;
  int arg;
  while((arg = getopt(argc, argv, "d")) != -1) {
	  if(arg == '?')
		  return 1;
	  if(arg == 'd')
		  decode_flag = 1;
  }
  FILE *f = stdin;
  if(argc != optind) {
	  if(strcmp("-", argv[optind])) {
		  f = fopen(argv[optind], "r");
		  if(!f || errno) {
			  perror(argv[0]);
			  return 1;
		  }
	  }
  }
  if(decode_flag)
	  decode(f);
  else
	  encode(f);
  fclose(f);
  return 0;
}
