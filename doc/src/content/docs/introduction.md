---
title: Introduction
description: An overview of the Collection library and how to get started
---

**Collection** is a modern generic data structures and algorithms library for C, inspired by C++ STL. It provides essential data structures like dynamic arrays, heap, linked lists, hash tables, and efficient algorithms with a clean, type-safe API.

## Features

-   **Generic Design** - Type-agnostic implementation using void pointers
-   **Memory Safe** - Automatic memory management with custom destructor support
-   **Modern C** - Clean API following C11 standards and best practices
-   **STL-Inspired** - Familiar interface for developers coming from C++

## Requirements

Build Dependencies

-   **C Compiler**: GCC or Clang with C11 support
-   **Make**: GNU Make 3.81 or later
-   **Standard C Library**: glibc or compatible

## Quick Start

### 1. Clone the Repository

```bash
git clone <repository-url>
cd collection
```

### 2. Build the Library

Build the static library:

```bash
make lib
```

This creates a static library at `lib/libcollection.a`.

### 3. Build Tests

Compile and run all tests:

```bash
make test
```

### 4. Use the Library

Include the library in your project:

```bash
gcc -I./include your_program.c -L./lib -lcollection -o your_program
```

## Build Configuration

The build system is configured via `config/config.mk`:

-   **C Standard**: c11
-   **Build Method**: static or dynamic
-   **Library Name**: collection

To build as a dynamic library, edit `config/config.mk`:

```makefile
BUILD_METHOD := dynamic
```

Then rebuild:

```bash
make clean
make lib
```

## Docker Development Environment

For a consistent development environment across different platforms, you can use Docker:

```bash
make docker
```

This command will:

-   Check if the Docker image exists, and build it if needed (Ubuntu-based with GCC and C development tools)
-   Remove any existing container with the same name
-   Start an interactive container with your project directory mounted at `/workspace`

The Docker image includes:

-   Ubuntu Linux
-   GCC (GNU Compiler Collection)
-   Make and build-essential tools

Once inside the container, you can build and test the library:

```bash
make lib
make test
```

## Available Make Targets

| Target               | Description                                       |
| -------------------- | ------------------------------------------------- |
| `make help`          | Show available commands                           |
| `make lib`           | Build the library                                 |
| `make all`           | Build library                                     |
| `make test`          | Build and run all tests                           |
| `make test-[module]` | Run tests for specific module                     |
| `make test-bench`    | Build and run benchmarks                          |
| `make docker`        | Run Docker container with development environment |
| `make clean`         | Remove all build artifacts                        |
| `make flags`         | Show the compile and link flags                   |

## Contributing

Contributions are welcome! Whether it's:

-   Bug reports
-   Feature requests
-   Documentation improvements
-   Code contributions

Please feel free to:

-   Open an issue to discuss proposed changes
-   Fork the repository
-   Submit a pull request

## Building Documentation

The documentation is built using Astro and Starlight.

```bash
cd doc
npm install
npm run dev
npm run build
```

## License

This project is licensed under the GNU General Public License v3.0 or later.

## Support

-   **Documentation**: [Full API Reference](/collection/vector/)
-   **Issues**: Report bugs or request features via GitHub Issues
-   **Discussions**: Ask questions in GitHub Discussions

## Acknowledgments

-   Inspired by C++ Standard Template Library (STL)
-   Test framework: [utest](https://github.com/unsigend/util)
-   Documentation: [Astro Starlight](https://starlight.astro.build/)

---
