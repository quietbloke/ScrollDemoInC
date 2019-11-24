APP := scrolldemo
ECHO := echo
FIND := find
SORT := sort
MAKE := make

MAKEFILES != $(FIND) . -mindepth 2 -name Makefile -type f | $(SORT)
MAKEDIRS = $(MAKEFILES:%/Makefile=%) 

.PHONY: default
default: build

.PHONY: build
build: $(MAKEDIRS)
	for dir in $(MAKEDIRS); do \
			$(MAKE) -C $$dir; \
		done

.PHONY: clean
clean:
	for dir in $(MAKEDIRS); do \
			$(MAKE) -C $$dir clean; \
		done

.PHONY: print
print:
	@$(ECHO) "****** Make variables ******"
	@$(ECHO) Makefiles   : $(MAKEFILES)
	@$(ECHO) Make dirs   : $(MAKEDIRS)

