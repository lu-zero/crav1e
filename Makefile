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

STATIC_NAME=librav1e.a

OS=$(shell uname -s)

# Horrible hack for Msys
#
# This only works natively building on Windows
# with a GNU toolchain Rust. Tested in MSYS2.
ifneq ($(OS),Linux)
ifneq ($(OS),Darwin)
OS=$(shell uname -o)
STATIC_NAME=rav1e.lib
endif
endif

SO_NAME_Darwin=librav1e.dylib
SO_NAME_INSTALL_Darwin=librav1e.$(VERSION).dylib
SO_NAME_Linux=librav1e.so
SO_NAME_INSTALL_Linux=librav1e.so.$(VERSION)
SO_NAME_INSTALL_Msys=rav1e.dll
SO_NAME_Msys=rav1e.dll

all: target/$(build_mode)/$(STATIC_NAME) rav1e.pc include/rav1e.h

examples: simple_encoding

clean:
	cargo clean
	-rm -f rav1e.pc
	-rm -f include/rav1e.h
	-rm -f simple_encoding

include/rav1e.h: target/$(build_mode)/$(STATIC_NAME)

target/$(build_mode)/$(STATIC_NAME): Cargo.toml src/lib.rs
	cargo build ${cargo_mode_opt}

rav1e.pc: data/rav1e.pc.in Makefile Cargo.toml
	sed -e "s;@prefix@;$(prefix);" \
	    -e "s;@libdir@;$(libdir);" \
	    -e "s;@incdir@;$(incdir);" \
            -e "s;@VERSION@;$(VERSION);" \
            -e "s;@PRIVATE_LIBS@;$$(rustc --print native-static-libs /dev/null --crate-type staticlib 2>&1| grep native-static-libs | cut -d ':' -f 3);" data/rav1e.pc.in > $@

simple_encoding: c-examples/simple_encoding.c target/$(build_mode)/$(STATIC_NAME) rav1e.pc
	$(CC) -g -O0 -std=c99 $< -o $@ target/$(build_mode)/$(STATIC_NAME) `grep Libs.private rav1e.pc | cut -d ':' -f 2` -Iinclude

status_to_str: c-tests/status_to_str.c target/$(build_mode)/$(STATIC_NAME) rav1e.pc
	$(CC) -g -O0 -std=c99 $< -o $@ target/$(build_mode)/$(STATIC_NAME) `grep Libs.private rav1e.pc | cut -d ':' -f 2` -Iinclude

simple_encoding_installed: c-examples/simple_encoding.c target/$(build_mode)/$(STATIC_NAME) rav1e.pc
	$(CC) -g -O0 -std=c99 $< -o $@ `pkg-config --cflags --libs rav1e`


install: target/$(build_mode)/$(STATIC_NAME) rav1e.pc include/rav1e.h target/$(build_mode)/$(SO_NAME_$(OS))
	-mkdir -p $(INCDIR) $(LIBDIR)/pkgconfig
	cp rav1e.pc $(LIBDIR)/pkgconfig
	cp target/$(build_mode)/$(STATIC_NAME) $(LIBDIR)
	cp target/$(build_mode)/$(SO_NAME_$(OS)) $(LIBDIR)/$(SO_NAME_INSTALL_$(OS))
ifneq ($(OS),Msys)
	ln -sf $(LIBDIR)/$(SO_NAME_INSTALL_$(OS)) $(LIBDIR)/$(SO_NAME_$(OS))
else
	cp target/$(build_mode)/$(SO_NAME_$(OS)).a $(LIBDIR)/$(SO_NAME_INSTALL_$(OS)).a
	cp target/$(build_mode)/rav1e.def $(LIBDIR)/rav1e.def
endif
	cp include/rav1e.h $(INCDIR)

uninstall:
ifneq ($(OS),Msys)
	-rm $(LIBDIR)/$(SO_NAME_INSTALL_$(OS))
else
	-rm $(LIBDIR)/rav1e.def
endif
	-rm $(LIBDIR)/$(SO_NAME_$(OS))
	-rm $(LIBDIR)/pkgconfig/rav1e.pc
	-rm $(LIBDIR)/$(STATIC_NAME)
	-rm $(INCDIR)/rav1e.h

.PHONY: all install uninstall examples
