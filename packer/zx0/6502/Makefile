ACME ?= acme
ACME_OPTs ?= -f cbm

all: dzx0

dzx0: depack.asm dzx0.asm testfile.zx0
	$(ACME) $(ACME_OPTS) -o $@ $<
vice:
	x64 -autostartprgmode 1 -moncommands dzx0

clean:
	@-rm dzx0
