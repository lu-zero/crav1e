extern crate cbindgen;
extern crate cdylib_link_lines;

fn main() {
    let crate_dir = std::env::var("CARGO_MANIFEST_DIR").unwrap();
    let header_path: std::path::PathBuf = ["include", "rav1e.h"].iter().collect();

    cbindgen::generate(crate_dir).unwrap().write_to_file(header_path);

    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=cbindgen.toml");

    cdylib_link_lines::metabuild();
}
