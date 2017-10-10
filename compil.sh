#!/bin/sh
flex shell.lex;
mv lex.yy.c shell.lex.c;
yacc -d shell.y;
gcc -o shell.lex.o -c shell.lex.c;
gcc -o y.tab.o -c y.tab.c;
gcc -o shell shell.lex.o y.tab.o -ll;

rm shell.lex.c;
rm shell.lex.o;
rm y.tab.c;
rm y.tab.h;
rm y.tab.o;