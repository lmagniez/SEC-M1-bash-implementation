MYLS=myls/
PSAUX=psAux/ 

all:
	@(cd $(MYLS) && $(MAKE))
	@(cd $(PSAUX) && $(MAKE))	

clean:
	@(cd $(MYLS) && $(MAKE) $@)
	@(cd $(PSAUX) && $(MAKE) $@)
	@echo "Cleaning done :)"
