# Crafting Interpreters (Nystrom)

This is a C++ implementation of the material in `Crafting Interpreters` by Robert 
Nystrom. I made this version because I got stuck on something in Rust and wanted to
have a reference to check against.

### Installation/Use
Executables are built into the root of the repo. To bulid the executables and tests 
run `make all`. Test binaries appear in the directory `/bin/test/`. A shell script is 
provided at `test/run_test.sh` to run all unit test binaries.


### Requirements 
- C++ 17
- I use `gcc`
- The standard library
- Tests use `Catch2`, header is in the project.
