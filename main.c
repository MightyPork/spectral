#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct Counter_struct Counter;

struct Counter_struct {
	char *point;
	int count;
	Counter *next;
};

static Counter *head = NULL;
static int point_count = 0;

static char utf_buffer[5];
static int utf_buffer_len = 0;
static int utf_remain = 0;
static bool is_utf = false;

/** Free the list structure */
static void destroy_list(Counter *h)
{
	if (h == NULL) return;
	destroy_list(h->next);
	free(h->point);
	free(h);
}

/** Create a new counter (list node) */
static Counter *make_counter(const char *src_string)
{
	Counter *cntr = malloc(sizeof(Counter));
	cntr->point = strdup(src_string);
	cntr->count = 1;
	cntr->next = NULL;
	return cntr;
}

/** Char was collected, add to the list or increment it's counter */
static void process_new_char(void)
{
	/*
	printf("Unicode char = ");

	for (int i = 0; i < utf_buffer_len; i++) {
		printf("%02x, ", (uint8_t) utf_buffer[i], utf_buffer[i]);
	}

	printf("(%s)\n", utf_buffer);
	*/

	// Add to the list or increment
	if (head == NULL) {
		// no list yet
		head = make_counter(utf_buffer);
		point_count++;
	} else {
		Counter *ptr = head;
		bool found = false;

		while(1) {
			if (strcmp(ptr->point, utf_buffer) == 0) {
				ptr->count++;
				found = true;
				break;
			}

			if (ptr->next == NULL) break;
			ptr = ptr->next;
		}

		if (!found) {
			ptr->next = make_counter(utf_buffer);
			point_count++;
		}
	}

	utf_buffer_len = 0;
	utf_remain = 0;
	is_utf = false;
}

/** Start collecting a new utf-8 char */
static void start_utf_char(char c, int remain)
{
	memset(utf_buffer, 0, sizeof(utf_buffer));

	is_utf = true;
	utf_remain = remain;
	utf_buffer_len = 1;
	utf_buffer[0] = c;
}

/** Handle an incoming char */
static void incoming_byte(char c)
{
	if (!is_utf) {
		// plain ASCII or first of UTF-8
		if ((c & 0xE0) == 0xC0) {
			// 2-char utf8
			start_utf_char(c, 1);
		} else if ((c & 0xF0) == 0xE0) {
			// 3-char utf8
			start_utf_char(c, 2);
		} else if ((c & 0xF8) == 0xF0) {
			// 4-char utf8
			start_utf_char(c, 3);
		} else {
			// ASCII
			start_utf_char(c, 0);
			process_new_char();
		}
	} else {
		// continue a longer char
		utf_buffer[utf_buffer_len++] = c;
		utf_remain--;
		// We should check the starting 2 bits here

		if (utf_remain == 0) {
			process_new_char();
		}
	}
}

static int counter_cmpfunc_alpha(const void *a, const void *b)
{
	return strcmp((*((const Counter **)a))->point, (*((const Counter **)b))->point);
}

static int counter_cmpfunc_count(const void *a, const void *b)
{
	return -1 * ((*((const Counter **)a))->count - (*((const Counter **)b))->count);
}

int main(int argc, const char *argv)
{
	// TODO args

	// FIXME does not handle '\0' properly

	int c;
	while ((c =getchar()) != EOF) {
		incoming_byte((char) c);
	}

	Counter **list = malloc(sizeof(Counter*) * point_count);
	int cnt;

	Counter *ptr = head;
	while (ptr != NULL) {
		list[cnt++] = ptr;
		ptr = ptr->next;
	}

	// TODO let user choose how to sort
	qsort(list, point_count, sizeof(Counter*), counter_cmpfunc_count);

	for (int i = 0; i < point_count; i++) {
		const char *toprint = list[i]->point;

		if (strlen(toprint) == 1 && toprint[0] < ' ') {
			// TODO nicer formatting
			if (toprint[0] == '\n') toprint = "\\n";
			if (toprint[0] == '\r') toprint = "\\r";
			if (toprint[0] == '\t') toprint = "\\t";
		}

		printf("'%s'\t%d\n", toprint, list[i]->count);
	}

	free(list);
	destroy_list(head);

	return 0;
}
