MYLS=shell/modules-externes/myls/
PSAUX=shell/modules-externes/myps/

all:
	@(cd $(MYLS) && $(MAKE))
	@(cd $(PSAUX) && $(MAKE))	

clean:
	@(cd $(MYLS) && $(MAKE) $@)
	@(cd $(PSAUX) && $(MAKE) $@)
	@echo "Cleaning done :)"
