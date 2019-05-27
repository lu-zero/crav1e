extern crate cbindgen;


// TODO move to a separate crate and/or cargo-c to not repeat yourself.
fn shared_object_link_line() {
    // We do not care about `_pre` and such.
    let major = env!("CARGO_PKG_VERSION_MAJOR");
    let minor = env!("CARGO_PKG_VERSION_MINOR");
    let micro = env!("CARGO_PKG_VERSION_PATCH");

    let link = "cargo:rustc-cdylib-link-arg=";

    if cfg!(target_os = "linux") {
        println!("{}-Wl,-soname,librav1e.so.{}", link, major);
    } else if cfg!(target_os = "macos") {
        let libdir = "/usr/local/lib";
        println!("{0}-Wl,-install_name,{1}/librav1e.{2}.{3}.{4}.dylib,-current_version,{2}.{3}.{4},-compatibility_version,{2}",
                link, libdir, major, minor, micro);
    } else if cfg!(target_os = "windows") {
        // This is only set up to work on GNU toolchain versions of Rust
        let out_dir = std::path::PathBuf::from("target").join(std::env::var("PROFILE").unwrap());

        println!("{}-Wl,--out-implib,{}", link, out_dir.join("rav1e.dll.a").to_string_lossy());
        println!("{}-Wl,--output-def,{}", link, out_dir.join("rav1e.def").to_string_lossy());
    }
}

fn main() {
    let crate_dir = std::env::var("CARGO_MANIFEST_DIR").unwrap();
    let header_path: std::path::PathBuf = ["include", "rav1e.h"].iter().collect();

    cbindgen::generate(crate_dir).unwrap().write_to_file(header_path);

    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=cbindgen.toml");

    shared_object_link_line();
}
