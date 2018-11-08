#define SYMBOL_H_ 

struct _entry;
typedef struct _entry entry;

typedef struct _entry {
	int is_const;
	int is_vec;
	int type_name;
	char *id;
	entry *next_scope;
	entry *this_scope;
};

void new_scope();
void new_entry();

