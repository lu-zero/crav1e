extern crate cbindgen;

fn main() {
    let crate_dir = std::env::var("CARGO_MANIFEST_DIR").unwrap();

    cbindgen::Builder::new()
        .with_crate(crate_dir)
        .with_parse_deps(true)
        .with_header("// SPDX-License-Identifier: MIT")
        .with_include_guard("RAV1E_H")
        .with_language(cbindgen::Language::C)
        .generate()
        .expect("Cannot generate bindings")
        .write_to_file("rav1e.h");

    println!("cargo:rerun-if-changed=src/lib.rs");
}
