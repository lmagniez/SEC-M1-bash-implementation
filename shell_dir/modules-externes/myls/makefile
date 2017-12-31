EXEC=./

OBJET=myls.o 
CC=gcc
CFLAGS=-Wall -g

myls : $(OBJET)
	@$(CC) -o $(EXEC)myls $(BINDIR)*.o
	@$(CC) -o ../../../myls $(BINDIR)*.o


myls.o: $(MYLS)myls.c
	@$(CC) $(CFLAGS) -c  -o $@ $(MYLS)myls.c

clean:
	@rm -rf *.o
	@rm -rf ./myls
	@rm -rf ../../../myls
	@echo "Cleaning done :)"
