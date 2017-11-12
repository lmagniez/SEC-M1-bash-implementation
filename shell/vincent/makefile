EXEC=./

OBJET=tools_psAux.o myjobs.o 
CC=gcc
CFLAGS=-Wall -g
FICHIER_PS=../../psAux/src/


myJobs : $(OBJET)
	@$(CC) -o $(EXEC)myJobs  $(BINDIR)*.o

myjobs.o: myjobs.c $(FICHIER_PS)tools_psAux.c
	@$(CC) $(CFLAGS) -c  -o $@ myjobs.c

tools_psAux.o : $(FICHIER_PS)tools_psAux.c
	@$(CC) $(CFLAGS) -c  -o $@ $(FICHIER_PS)tools_psAux.c

clean:
	@rm -rf $(BINDIR)*.o
	@rm -rf ../myPsaux
	@echo "Cleaning done :)"
