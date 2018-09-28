to compile scanner.l: `flex scanner.l`

to compile parser.y: `bison -d parser.y`

to compile generated c files: `gcc lex.yy.c parser.tab.c`
