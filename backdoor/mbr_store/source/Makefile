# mbr_store Makefile

MANDIR      = /usr/share/man/man1/

all: 
	make -C src/
	
install:
	install -m 0755 bin/mbr_store /usr/bin/
	install -m 0644 mbr_store.1.gz $(MANDIR)/
	
uninstall:
	rm -f /usr/bin/mbr_store
	rm -f $(MANDIR)mbr_store.1.gz
	
clean:
	rm -f bin/mbr_store
