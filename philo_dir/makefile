EXEC=../

OBJET=philo.o 
CC=gcc
CFLAGS=-Wall -g

philo : $(OBJET)
	@$(CC) -o $(EXEC)philo $(BINDIR)*.o -lpthread

philo.o: $(PHILO)*.c
	@$(CC) $(CFLAGS) -c  -o $@ $(PHILO)philo.c 

clean:
	@rm -rf *.o
	@rm -rf ../philo
	@echo "Cleaning done :)"
