flex scanner.l
bison -d parser.y
gcc lex.yy.c parser.tab.c test.in
