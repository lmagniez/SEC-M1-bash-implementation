MYLS=shell_dir/modules-externes/myls/
PSAUX=shell_dir/modules-externes/myps/
PHILO=philo_dir/
BIG=shell_dir/

all:
	@(cd $(MYLS) && $(MAKE))
	@(cd $(PSAUX) && $(MAKE))	
	@(cd $(PHILO) && $(MAKE))
	@(cd $(BIG) && $(MAKE))

clean:
	@(cd $(MYLS) && $(MAKE) $@)
	@(cd $(PSAUX) && $(MAKE) $@)
	@(cd $(PHILO) && $(MAKE) $@)
	@(cd $(BIG) && $(MAKE) $@)
	@echo "Cleaning done :)"
