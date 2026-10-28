<p align="center">
    <img src="assets/logo.png" alt="collection logo" style="width: 100%; height: 320px;">
</p>

# collection

**v2.0.0** (in progress)

A high-performance, optimized C data structure library.

**Requires:** GCC or Clang, GNU Make, and a standard C library.

## Layout

| Path | Role |
| --- | --- |
| `include/` | Public headers |
| `src/` | Implementations |
| `test/external/` | utest framework |
| `test/cases/` | Tests and (later) benchmarks |

## Quick start

```bash
make lib          # build lib/libcollection.a
make test         # build and run tests
make all          # build the library and run tests
```

Link against the library:

```bash
gcc -I./include your_program.c -L./lib -lcollection -o your_program
```

## Make targets

```
make all            - Build the library and run all tests
make lib            - Build the library
make test           - Build and run all tests
make test-NAME      - Build and run tests for a specific module
make clean          - Clean the build artifacts
make flags          - Show the compile and link flags
make clang          - Run clang to generate compile commands
make format         - Format the code
make docker         - Run Docker container with development environment
make line           - Count source lines in include and src
```

Running `make` with no target shows the same usage.

## Contributing

Bug reports and patches are welcome. Open an issue to discuss larger changes, then fork and submit a pull request.

## Copyright

Copyright © 2026-2027 Yixiang Qiu. Licensed under the GNU General Public License v3.0 or later.
