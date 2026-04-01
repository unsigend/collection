<p align="center">
    <img src="assets/logo.png" alt="collection logo" style="width: 100%; height: 320px;">
</p>

# collection

**v1.0.0**

Generic data structures and algorithms for C, STL-inspired: void-pointer generics, optional custom destructors, static or shared builds via `config/config.mk`.

**Requires:** GCC or Clang, GNU Make, and a standard C library.

## Documentation

API reference: [Collection](https://collection-c.vercel.app/).

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
```

Running `make` with no target shows the same usage.

## Contributing

Bug reports, docs, and patches are welcome. Open an issue to discuss larger changes, then fork and submit a pull request.

## Copyright

Copyright © 2025 Yixiang Qiu. Licensed under the GNU General Public License v3.0 or later.
