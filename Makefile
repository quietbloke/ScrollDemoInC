APP := scrolldemo
ECHO := echo
FIND := find
SORT := sort
MAKE := make

# Detect Operating System
ifeq ($(OS),Windows_NT)
    OSNAME := WIN32
    MONO := 
    WINE := 
else
    OSNAME != uname -s
    MONO := mono
    WINE := wine
endif

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
	@$(ECHO) OSNAME      : $(OSNAME)
	@$(ECHO) Makefiles   : $(MAKEFILES)
	@$(ECHO) Make dirs   : $(MAKEDIRS)

