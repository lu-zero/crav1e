VERSION = 0.1.0
prefix=/usr/local

incdir=$(prefix)/include
bindir=$(prefix)/bin
libdir=$(prefix)/lib
mandir=$(prefix)/man

BINDIR=$(DESTDIR)$(bindir)
INCDIR=$(DESTDIR)$(incdir)/rav1e
LIBDIR=$(DESTDIR)$(libdir)

build_mode:=release
ifeq ($(build_mode),debug)
cargo_mode_opt:=
else
cargo_mode_opt:=--${build_mode}
endif

all: target/$(build_mode)/librav1e.a rav1e.pc include/rav1e.h

examples: simple_encoding

clean:
	cargo clean
	-rm -f rav1e.pc
	-rm -f include/rav1e.h
	-rm -f simple_encoding

include/rav1e.h: target/$(build_mode)/librav1e.a

target/$(build_mode)/librav1e.a: Cargo.toml src/lib.rs
	cargo build ${cargo_mode_opt}

rav1e.pc: data/rav1e.pc.in Makefile Cargo.toml
	sed -e "s;@prefix@;$(prefix);" \
	    -e "s;@libdir@;$(libdir);" \
	    -e "s;@incdir@;$(incdir);" \
            -e "s;@VERSION@;$(VERSION);" \
            -e "s;@PRIVATE_LIBS@;$$(rustc --print native-static-libs /dev/null --crate-type staticlib 2>&1| grep native-static-libs | cut -d ':' -f 3);" data/rav1e.pc.in > $@

simple_encoding: c-examples/simple_encoding.c target/$(build_mode)/librav1e.a rav1e.pc
	$(CC) -g -O0 -std=c99 $< -o $@ -Ltarget/$(build_mode)/ -lrav1e `grep Libs.private rav1e.pc | cut -d ':' -f 2` -Iinclude

install: target/$(build_mode)/librav1e.a rav1e.pc include/rav1e.h
	-mkdir -p $(INCDIR) $(LIBDIR)/pkgconfig
	cp rav1e.pc $(LIBDIR)/pkgconfig
	cp target/$(build_mode)/librav1e.a $(LIBDIR)
	cp include/rav1e.h $(INCDIR)

uninstall:
	-rm $(LIBDIR)/pkgconfig/rav1e.pc
	-rm $(LIBDIR)/librav1e.a
	-rm $(INCDIR)/rav1e.h

.PHONY: all install uninstall examples
