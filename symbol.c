#include <stdio.h>
#include <stdlib.h>
#include "symbol.h"
#include "common.h"

entry* table = NULL;

void new_scope(entry *e) {
	table->next_scope = e;
	table = e;
}

void new_entry(entry *e) {
	if(table == NULL) {
		table = (entry *)malloc(sizeof(entry));
	} else {
		table->this_scope = e;
	}
	table = e;
}

