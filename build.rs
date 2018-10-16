extern crate cbindgen;

fn main() {
    let crate_dir = std::env::var("CARGO_MANIFEST_DIR").unwrap();
    let header_path: std::path::PathBuf = ["include", "rav1e.h"].iter().collect();
/*
    cbindgen::Builder::new()
        .with_crate(crate_dir)
        .with_parse_deps(true)
        .with_header("// SPDX-License-Identifier: MIT")
        .with_include_guard("RAV1E_H")
        .with_language(cbindgen::Language::C)
        .generate()
        .expect("Cannot generate bindings")
*/
    cbindgen::generate(crate_dir).unwrap().write_to_file(header_path);

    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=cbindgen.toml");
}
