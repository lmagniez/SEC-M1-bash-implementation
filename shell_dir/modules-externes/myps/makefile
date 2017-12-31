EXEC=./
BINDIR=bin/
SRCDIR=src/

OBJET=$(BINDIR)main_ps_aux.o $(BINDIR)tools_psAux.o $(BINDIR)myPsaux.o 
CC=gcc
CFLAGS=-Wall -g

myPsaux : $(OBJET)
	@$(CC) -o $(EXEC)myps  $(BINDIR)*.o
	@$(CC) -o ../../../myps  $(BINDIR)*.o	

$(MYLS)myls.o: $(MYLS)myls.c
	@$(CC) $(CFLAGS) -c  -o $@ $(MYLS)myls.c

$(BINDIR)main_ps_aux.o: $(SRCDIR)*.c
	@$(CC) $(CFLAGS) -c  -o $@ $(SRCDIR)main_ps_aux.c

$(BINDIR)tools_psAux.o : $(SRCDIR)tools_psAux.c
	@$(CC) $(CFLAGS) -c  -o $@ $(SRCDIR)tools_psAux.c

$(BINDIR)myPsaux.o: $(SRCDIR)myPsaux.c $(SRCDIR)tools_psAux.c
	@$(CC) $(CFLAGS) -c  -o $@ $(SRCDIR)myPsaux.c

clean:
	@rm -rf $(BINDIR)*.o
	@rm -rf ./myps
	@rm -rf ../../../myps
	@echo "Cleaning done :)"
