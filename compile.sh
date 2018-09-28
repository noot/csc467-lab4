flex scanner.l
bison -d parser.y
gcc lex.yy.c parser.tab.c -lfl -Tn -I test.in
