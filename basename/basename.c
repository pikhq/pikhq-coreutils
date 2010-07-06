/* Basic basename util
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
#include <stdlib.h>
#include <string.h>

static int all_slashes(const char *string, const size_t string_len);
static char *remove_leading(char *string);
static void remove_suffix(char *string, const char *suffix);
static void remove_trailing(char *string, const size_t string_len);

static void basename(char *string, const char *suffix)
{
	size_t string_len = strlen(string);
	
	if(!strcmp("", string)) {
		printf(".\n");
		return;
	}
	
	if(all_slashes(string, string_len)) {
		printf("/\n");
		return;
	}
	
	remove_trailing(string, string_len);
	string = remove_leading(string);
	remove_suffix(string, suffix);
	printf("%s\n", string);
}

static int all_slashes(const char *string, const size_t string_len)
{
	for(int i = 0; i != string_len; i++)
		if(string[i] != '/')
			return 0;
	return 1;
}

static char *remove_leading(char *string)
{
	char *tmp = NULL;
	while((tmp = strchr(string, '/')) != NULL)
		string = tmp + 1;
	return string;
}

static void remove_trailing(char *string, const size_t string_len)
{
	for(int i = string_len - 1; i != 0 && string[i] == '/'; i--)
		string[i] = 0;
}

static void remove_suffix(char *string, const char *suffix)
{
	if(strcmp(string, suffix)) {
		char *suffix_end = strchr(suffix, 0);
		char *string_end = strchr(string, 0);
		while(suffix_end != suffix && string_end != string && *string_end-- == *suffix_end--);
		if(suffix_end == suffix)
			*string_end = 0;
	}
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
}
