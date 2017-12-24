MYLS=myls/
PSAUX=psAux/ 
PHILO=philo_dir/

all:
	@(cd $(MYLS) && $(MAKE))
	@(cd $(PSAUX) && $(MAKE))	
	@(cd $(PHILO) && $(MAKE))

clean:
	@(cd $(MYLS) && $(MAKE) $@)
	@(cd $(PSAUX) && $(MAKE) $@)
	@(cd $(PHILO) && $(MAKE) $@)
	@echo "Cleaning done :)"
