EXEC=../

OBJET=shell.lex.o y.tab.o ./modules/mypipe/mypipe.o ./modules/myjoker/myjoker.o ./modules/myjobs/myjobs.o ./modules/mycd/mycd.o launchManager.o ./modules/myredir/myredir.o
CC=gcc
CFLAGS=-Wall -g


shell : $(OBJET)
	@$(CC) -o $(EXEC)shell  $(OBJET) -ll
	@rm shell.lex.c;
	@rm shell.lex.o;
	@rm y.tab.c;
	@rm y.tab.h;
	@rm y.tab.o;
	@rm launchManager.o;

shell.lex.o:
	@flex shell.lex;
	@mv lex.yy.c shell.lex.c;
	@yacc -d shell.y;
	@$(CC) $(CFLAGS) -c  -o $@ shell.lex.c

y.tab.o: y.tab.c
	@$(CC) $(CFLAGS) -c  -o $@ y.tab.c

./modules/mypipe/mypipe.o : ./modules/mypipe/mypipe.c
	@$(CC) $(CFLAGS) -c  -o $@ ./modules/mypipe/mypipe.c

./modules/myjoker/myjoker.o: ./modules/myjoker/myjoker.c
	@$(CC) $(CFLAGS) -c  -o $@ ./modules/myjoker/myjoker.c

./modules/myredir/myredir.o: ./modules/myredir/myredir.c
	@$(CC) $(CFLAGS) -c  -o $@ ./modules/myredir/myredir.c

./modules/myjobs/myjobs.o: ./modules/myjobs/myjobs.c
	@$(CC) $(CFLAGS) -c  -o $@ ./modules/myjobs/myjobs.c

./modules/mycd/mycd.o: ./modules/mycd/mycd.c
	@$(CC) $(CFLAGS) -c  -o $@ ./modules/mycd/mycd.c

launchManager.o: launchManager.o
	@$(CC) $(CFLAGS) -c  -o $@ launchManager.c

clean:
	@rm -rf $(OBJET)
	@rm -rf $(EXEC)shell
	@echo "Cleaning done :)"
