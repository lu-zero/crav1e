# C rav1e API

[cbindgen]()-based API for C/C++ users.

## Status

The API is far from stable

- [x] Basic encoding usage
- [x] OBU Sequence Header for mkv/mp4 support
- [x] Documentation
- [ ] TimeInfo/generic data pinning
- [ ] pkg-conf `.pc` generation
- [ ] Examples
- [ ] Install target

## Usage
You may generate the library and the header file using

```
cargo build
```
or
```
cargo build --release
```

The header will be available as `include/rav1e.h`, the library will be in `target/<debug or release>/librav1e.<so or dylib>`

## Developing
I suggest to use the cargo [paths override](https://doc.rust-lang.org/cargo/reference/config.html) to have a local `rav1e`:

```
# Clone the trees
$ git clone https://github.com/xiph/rav1e
$ git clone https://github.com/lu_zero/crav1e
# Setup the override
$ cd crav1e
$ mkdir .cargo/rav1e
$ echo 'paths=[../rav1e]'
# Check it is doing the right thing
$ cargo build
```

