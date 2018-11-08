#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"
#include "common.h"

entry* table = NULL;
entry* stack = NULL;

void enter_scope(entry *e) {
	e->this_scope = table;
	//table->next_scope = e;
	e->prev_scope = stack;
	table = e;
	stack->next_scope = table;
}

void exit_scope() {
	stack = stack->prev_scope;
	table = stack->this_scope;
}

void new_entry(entry *e) {
	if(in_scope(e->id) == 1) {
		break;
	}
	if(table == NULL) {
		table = (entry *)malloc(sizeof(entry));
	} else {
		table->this_scope = e;
	}
	table = e;
}

int in_scope(char *id) {
	entry *curr = table;
	while(curr) {
		if(strcmp(curr->id, id) == 0) {
			return 1;
		}
		curr = curr->this_scope;
	}
	return 0;
}