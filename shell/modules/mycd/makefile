EXEC=./

OBJET=mycd.o 
CC=gcc
CFLAGS=-Wall -g

mycd : $(OBJET)
	@$(CC) -o $(EXEC)mycd  $(BINDIR)*.o


mycd.o: mycd.c 
	@$(CC) $(CFLAGS) -c  -o $@ mycd.c

clean:
	@rm -rf $(BINDIR)*.o
	@rm -rf ./mycd
	@echo "Cleaning done :)"
