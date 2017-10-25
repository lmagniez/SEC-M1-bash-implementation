EXEC=../

OBJET=myls.o 
CC=gcc
CFLAGS=-Wall -g

myLs : $(OBJET)
	@$(CC) -o $(EXEC)myLs $(BINDIR)*.o

myls.o: $(MYLS)myls.c
	@$(CC) $(CFLAGS) -c  -o $@ $(MYLS)myls.c

clean:
	@rm -rf *.o
	@rm -rf ../myLs
	@echo "Cleaning done :)"
