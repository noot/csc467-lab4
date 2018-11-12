#define SYMBOL_H_ 

struct _entry;
typedef struct _entry entry;

struct _entry {
	int is_const;
	int is_vec;
	int type_name;
	int is_init;
	char *id;
	entry *prev_scope;
	entry *this_scope;
	entry *next_scope;
};

void enter_scope(entry *e);
void exit_scope();
void new_entry(entry *e);
int in_scope(char *id);
entry* find_entry(char *id);
