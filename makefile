MYLS=shell/modules-externes/myls/
PSAUX=shell/modules-externes/myps/
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
