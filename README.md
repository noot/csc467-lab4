to compile scanner.l: `flex scanner.l`

to compile parser.y: `bison -d parser.y`

to compile generated c files: `gcc lex.yy.c parser.tab.c`

to make project: `make`

to run compiler467 with trace scanner and input file: `./compiler467 -Tn test.in`
