/* Basic cal util
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

#define _POSIX_SOURCE

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "coroutine.h"

static const char *weekdays[] = {
	"Sunday", "Monday", "Tuesday",
	"Wednesday", "Thursday", "Friday",
	"Saturday"
};

static const char *months[] = {
  "January", "February", "March",
  "April", "May", "June",
  "July", "August", "September",
  "October", "November", "December"
};

static const int lengths_of_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static const int months_offset[] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};

static const int months_offset_in_leap[] = {6, 2, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};

static int is_leap(const int year)
{
  return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

static int starting_day(const int month, const int year)
{
	int c = 2*(3-((year/100)%4));
	int y = year + (year/4);
	if(is_leap(year))
		return (c + y + months_offset_in_leap[month]) % 7;
	else
		return (c + y + months_offset[month]) % 7;
}

static void repeat_print(const char *s, int n)
{
	for(int i = 0; i != n; i++)
		printf("%s", s);
}

static void output_centered(const char *s, const size_t s_len, const int num_columns)
{
	repeat_print(" ", (num_columns - s_len)/2);
	printf("%s", s);
	repeat_print(" ", (num_columns - s_len)/2 + (num_columns - s_len)%2);
}

#define col_check {				\
		if(cont->cur_col == 7) {	\
			cont->cur_col = 0;	\
			ccrReturnV;		\
		}				\
		cont->cur_col++;		\
	}

static void print_cal(ccrContParam, const int month, const int year)
{
	ccrBeginContext;
	int i;
	int start;
	int cur_col;
	ccrEndContext(cont);
	
	ccrBegin(cont);
	
	printf("Su Mo Tu We Th Fr Sa");
	ccrReturnV;
	
	cont->start = starting_day(month, year);
	cont->cur_col = cont->start;
	repeat_print("   ", cont->start);

	for(cont->i = 1; cont->i <= lengths_of_month[month]; cont->i++) {
		col_check;
		printf("%2i ", cont->i);
	}
	if(is_leap(year) && month == 1) {
		col_check;
		printf("29 ");
	}
	repeat_print("   ", 7 - cont->cur_col);
	ccrFinishV;
}

static void print_single(const int month, const int year)
{
	ccrContext cont = 0;
	char title[21]; // Width of a single calendar
	int title_length = snprintf(title, 20, "%s %i", months[month], year);
	output_centered(title, title_length, 20);
	printf("\n");
	do {
		print_cal(&cont, month, year);
		printf("\n");
	} while(cont);
}

#define cal_header(x)						\
	output_centered(months[i+x], strlen(months[i+x]), 20);	\
	printf(x != 2 ? "   " : "\n");				\

		
#define init_cal(x)						\
	ccrContext cont ## x = 0;				\
	print_cal(&cont ## x, i + x, year);			\
	printf(x != 2 ? "   " : "\n");


#define put_cal(x)							\
	{								\
		if(cont ## x)						\
			print_cal(&cont ## x, i + x, year);		\
		else if(!cont ## x)					\
			if(x != 2)					\
				repeat_print(" ", 21);			\
		if(x == 2)						\
			printf("\n");					\
		else							\
			printf("  ");					\
	}

static void print_year(const int year)
{
	char title[5]; // 4 digits + NULL
	int title_length = snprintf(title, 5, "%i", year);
	output_centered(title, title_length, 66); // 66 = width of 3 calendars
	printf("\n");
	for(int i = 0; i < 11; i += 3) {
		cal_header(0); cal_header(1); cal_header(2);
		init_cal(0);   init_cal(1);   init_cal(2);
		while(cont0 || cont1 || cont2) {
			put_cal(0);  put_cal(1);  put_cal(2);
		}
	}
}

int main(int argc, char **argv)
{
	if(argc == 1) {
		struct tm cur_date;
		time_t cur_time = time(NULL);
		if(localtime_r(&cur_time, &cur_date) == NULL) {
			perror(argv[0]);
			return 1;
		}
		print_single(cur_date.tm_mon, cur_date.tm_year + 1900);
	} else if(argc == 2)
		print_year(atoi(argv[1]));
	else
		print_single(atoi(argv[1])-1, atoi(argv[2]));
	printf("\n");
	return 0;
}
