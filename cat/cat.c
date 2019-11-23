/* Basic cat util
 * Copyright (c) 2010 Ada Worcester <oss@ada.pikhq.com>
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
#include <errno.h>
#include <getopt.h>

static void do_cat(FILE *f)
{
	int c;
	while((c = getc(f)) != EOF)
		putchar(c);
}

int main(int argc, char **argv)
{
	int arg;

	/* POSIX requires that "-u" copy bytes directly from file to stdout.
	 * As this is our normal behavior, we can just ignore this flag.
	 */
	while((arg = getopt(argc, argv, "u")) != -1)
	      if(arg == '?') // Unrecognised. Getopt already did an error message.
		      return 1;

	if(argc == optind)
		do_cat(stdin);
	else {
		for(int i = optind; i < argc; i++)
			if(!strcmp("-", argv[i]))
				do_cat(stdin);
			else {
				FILE *f = fopen(argv[i], "r");
				if(!f || errno) {
					perror(argv[0]);
					return 1;
				}
				do_cat(f);
				fclose(f);
			}
	}
	return 0;
}
