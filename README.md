# C rav1e API

[![LICENSE](https://img.shields.io/badge/license-BSD2-blue.svg)](LICENSE)
[![dependency status](https://deps.rs/repo/github/lu-zero/crav1e/status.svg)](https://deps.rs/repo/github/lu-zero/crav1e)

[cbindgen](https://github.com/eqrion/cbindgen)-based API for C/C++ users.

## Status

The API is far from stable

- [x] Basic encoding usage
- [x] OBU Sequence Header for mkv/mp4 support
- [x] Documentation
- [ ] TimeInfo/generic data pinning
- [x] pkg-conf `.pc` generation
- [ ] Examples
- [x] Install target

## Usage
You may generate the library and the header file using

### Makefile
A quite simple makefile is provided:

``` sh
# Build librav1e.a, rav1e.h and rav1e.pc
$ make
```

``` sh
# Install librav1e.a, rav1e.h and rav1e.pc
$ make DESTDIR=${D} prefix=${prefix} libdir=${libdir} install
```

``` sh
# Remove librav1e.a, rav1e.h and rav1e.pc
$ make DESTDIR=${D} prefix=${prefix} libdir=${libdir} uninstall
```

### cargo
Currently `cargo install` does not work for libraries.

```
$ cargo build
```
or
```
$ cargo build --release
```

The header will be available as `include/rav1e.h`, the library will be in `target/<debug or release>/librav1e.<so or dylib>`
Look in `c-examples` for working examples.

## Developing
I suggest to use the cargo [paths override](https://doc.rust-lang.org/cargo/reference/config.html) to have a local `rav1e`:

```
# Clone the trees
$ git clone https://github.com/xiph/rav1e
$ git clone https://github.com/lu_zero/crav1e
# Setup the override
$ cd crav1e
$ mkdir .cargo
$ echo 'paths=["../rav1e"]' > .cargo/config
# Check it is doing the right thing
$ cargo build
```

