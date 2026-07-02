<p align="center">
    <img src="assets/logo.png" alt="collection logo" style="width: 100%; height: 320px;">
</p>

# collection

**v1.3.1**

Generic data structures and algorithms for C, inspired by the C++ STL. Containers use opaque heap-allocated handles, optional element destructors, and a consistent init/free API. Build as a static or shared library via `config/config.mk`.

**Requires:** GCC or Clang, GNU Make, and a standard C library.

## What's included

**11 containers**

| Module | Description |
| --- | --- |
| `vector` | Contiguous resizable array |
| `stack` | LIFO adapter |
| `queue` | FIFO adapter |
| `deque` | Double-ended queue |
| `heap` | Binary heap priority queue |
| `slist` | Singly linked list |
| `dlist` | Doubly linked list |
| `avltree` | Self-balancing binary search tree |
| `hashtbl` | Hash table |
| `set` | Ordered set (AVL-backed) |
| `string` | Length-counted byte string |

**3 algorithm modules**

| Module | Description |
| --- | --- |
| `sort` | Sorting (`qsort`, heap sort, quick sort) |
| `hash` | Hash helpers for integer and string keys |
| `util` | Small utilities (e.g. swap) |

Include everything with `#include <collection.h>`, or pull in individual headers such as `#include <collection/vector.h>`.

## Documentation

API reference: [collection-c.vercel.app](https://collection-c.vercel.app/)

## Quick start

```bash
make lib          # build lib/libcollection.a
make test-vector  # run one test module
make all          # build and run all tests
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
```

Running `make` with no target shows the same usage.

## Contributing

Bug reports, docs, and patches are welcome. Open an issue to discuss larger changes, then fork and submit a pull request.

## Copyright

Copyright © 2025 Yixiang Qiu. Licensed under the GNU General Public License v3.0 or later.
