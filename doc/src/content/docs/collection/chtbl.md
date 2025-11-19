---
title: Chtbl
description: Chained hash table implementation with key-value pairs
---

The `Chtbl` is a chained hash table data structure that provides efficient insertion, deletion, and lookup of key-value pairs using separate chaining for collision resolution.

## Header Files

To use the chained hash table in your code, include the header file:

```c
#include <collection/chtbl.h>
```

This provides access to the `Chtbl` and `ChtblEntry` types and all hash table functions.

## Macros

Efficient inline operations implemented as macros:

### chtbl_size

```c
chtbl_size(chtbl)
```

Returns the number of key-value pairs in the hash table.

**Parameters:**

-   `chtbl` - Pointer to the hash table

**Return Value:**

The number of key-value pairs currently stored in the hash table.

**Complexity:** O(1)

**Example:**

```c
Chtbl chtbl;
chtbl_init(&chtbl, NULL, NULL, NULL, NULL);

printf("Size: %zu\n", chtbl_size(&chtbl));

chtbl_insert(&chtbl, "key1", "value1");
printf("Size: %zu\n", chtbl_size(&chtbl));  // Prints "Size: 1"
```

---

### chtbl_buckets

```c
chtbl_buckets(chtbl)
```

Returns the number of buckets in the hash table.

**Parameters:**

-   `chtbl` - Pointer to the hash table

**Return Value:**

The number of buckets in the hash table.

**Complexity:** O(1)

**Example:**

```c
Chtbl chtbl;
chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 32);

printf("Buckets: %zu\n", chtbl_buckets(&chtbl));  // Prints "Buckets: 32"
```

---

### chtbl_in

```c
chtbl_in(chtbl, key)
```

Checks if a key exists in the hash table.

**Parameters:**

-   `chtbl` - Pointer to the hash table
-   `key` - The key to check

**Return Value:**

`true` if the key exists in the hash table, `false` otherwise.

**Complexity:** O(1)

**Example:**

```c
Chtbl chtbl;
chtbl_init(&chtbl, NULL, NULL, NULL, NULL);

chtbl_insert(&chtbl, "key1", "value1");

if (chtbl_in(&chtbl, "key1")) {
    printf("Key exists\n");
}

if (!chtbl_in(&chtbl, "nonexistent")) {
    printf("Key does not exist\n");
}
```

---

## Functions

Public interfaces for chained hash table operations:

### chtbl_init

```c
int chtbl_init(Chtbl* chtbl, 
    uint32_t (*hash)(const void * key),
    bool (*match)(const void * key1, const void * key2),
    void (*destroy_key)(void * key),
    void (*destroy_value)(void * value));
```

Initializes a chained hash table to an empty state with default capacity.

**Parameters:**

-   `chtbl` - Pointer to the hash table to initialize
-   `hash` - Hash function for keys. Pass `NULL` to use default string hash function
-   `match` - Match function to compare keys. Pass `NULL` to use default string comparison
-   `destroy_key` - Optional destructor function called when keys are removed. Pass `NULL` if no cleanup is needed
-   `destroy_value` - Optional destructor function called when values are removed. Pass `NULL` if no cleanup is needed

**Return Value:**

Returns `0` on success, `-1` on failure.

**Complexity:** O(1)

**Example:**

```c
Chtbl chtbl;
chtbl_init(&chtbl, NULL, NULL, NULL, NULL);  // Use defaults

// With custom hash and match functions
chtbl_init(&chtbl, hash_str, 
    (bool (*)(const void *, const void *))strcmp, NULL, NULL);

// With destroy functions
chtbl_init(&chtbl, NULL, NULL, free, free);
```

---

### chtbl_init_capacity

```c
int chtbl_init_capacity(Chtbl* chtbl, 
    uint32_t (*hash)(const void * key),
    bool (*match)(const void * key1, const void * key2),
    void (*destroy_key)(void * key),
    void (*destroy_value)(void * value),
    size_t capacity);
```

Initializes a chained hash table with a specified number of buckets.

**Parameters:**

-   `chtbl` - Pointer to the hash table to initialize
-   `hash` - Hash function for keys. Pass `NULL` to use default string hash function
-   `match` - Match function to compare keys. Pass `NULL` to use default string comparison
-   `destroy_key` - Optional destructor function called when keys are removed. Pass `NULL` if no cleanup is needed
-   `destroy_value` - Optional destructor function called when values are removed. Pass `NULL` if no cleanup is needed
-   `capacity` - The number of buckets to allocate

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

If the specified capacity is below the minimum, it will be adjusted to the minimum bucket count.

**Complexity:** O(n) where n is the capacity

**Example:**

```c
Chtbl chtbl;
chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 64);
// Hash table now has 64 buckets
```

---

### chtbl_set_load_factor

```c
void chtbl_set_load_factor(Chtbl* chtbl, float threshold);
```

Sets the maximum load factor threshold for the hash table.

**Parameters:**

-   `chtbl` - Pointer to the hash table
-   `threshold` - The load factor threshold (0.0 to 1.0)

**Description:**

The load factor is the ratio of the number of entries to the number of buckets. If the threshold is exceeded, the hash table may resize automatically (implementation dependent). Negative values are ignored, and values greater than 1.0 are clamped to 1.0.

**Complexity:** O(1)

**Example:**

```c
Chtbl chtbl;
chtbl_init(&chtbl, NULL, NULL, NULL, NULL);

chtbl_set_load_factor(&chtbl, 0.75f);  // Set threshold to 0.75
```

---

### chtbl_load_factor

```c
float chtbl_load_factor(Chtbl* chtbl);
```

Gets the current load factor of the hash table.

**Parameters:**

-   `chtbl` - Pointer to the hash table

**Return Value:**

The current load factor (number of entries / number of buckets), or `0.0f` if the table is empty or `NULL`.

**Complexity:** O(1)

**Example:**

```c
Chtbl chtbl;
chtbl_init(&chtbl, NULL, NULL, NULL, NULL);

chtbl_insert(&chtbl, "key1", "value1");
float load = chtbl_load_factor(&chtbl);
printf("Load factor: %.2f\n", load);
```

---

### chtbl_destroy

```c
void chtbl_destroy(Chtbl* chtbl);
```

Destroys the hash table and frees all associated memory.

**Parameters:**

-   `chtbl` - Pointer to the hash table to destroy

**Description:**

Calls the destroy functions (if provided) on all keys and values, then frees all entries and buckets. After destruction, the hash table is reset to an empty state.

**Complexity:** O(n) where n is the number of entries

**Example:**

```c
Chtbl chtbl;
chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
// ... use hash table ...
chtbl_destroy(&chtbl);
```

---

### chtbl_insert

```c
int chtbl_insert(Chtbl* chtbl, const void * key, const void * value);
```

Inserts a key-value pair into the hash table.

**Parameters:**

-   `chtbl` - Pointer to the hash table
-   `key` - Pointer to the key to insert
-   `value` - Pointer to the value to insert (can be `NULL`)

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

If the key already exists, the value is updated and the previous value is destroyed (if `destroy_value` function was provided during initialization). The size of the hash table does not increase when updating an existing key. If memory allocation fails, `-1` is returned and the hash table remains unchanged.

**Complexity:** O(1) average case

**Example:**

```c
Chtbl chtbl;
chtbl_init(&chtbl, NULL, NULL, NULL, NULL);

if (chtbl_insert(&chtbl, "key1", "value1") == 0) {
    // Successfully inserted
}

// Update existing key
chtbl_insert(&chtbl, "key1", "newvalue");

// Insert with NULL value
chtbl_insert(&chtbl, "key2", NULL);
```

---

### chtbl_find

```c
void* chtbl_find(Chtbl* chtbl, const void * key);
```

Finds a value in the hash table by key.

**Parameters:**

-   `chtbl` - Pointer to the hash table
-   `key` - The key to search for

**Return Value:**

Pointer to the value associated with the key, or `NULL` if the key is not found or if `chtbl` or `key` is `NULL`.

**Complexity:** O(1) average case

**Example:**

```c
Chtbl chtbl;
chtbl_init(&chtbl, NULL, NULL, NULL, NULL);

chtbl_insert(&chtbl, "key1", "value1");

void* value = chtbl_find(&chtbl, "key1");
if (value != NULL) {
    printf("Found: %s\n", (char*)value);
}

// Key not found
void* notfound = chtbl_find(&chtbl, "nonexistent");
if (notfound == NULL) {
    printf("Key not found\n");
}
```

---

### chtbl_find_entry

```c
ChtblEntry* chtbl_find_entry(Chtbl* chtbl, const void * key);
```

Finds an entry in the hash table by key.

**Parameters:**

-   `chtbl` - Pointer to the hash table
-   `key` - The key to search for

**Return Value:**

Pointer to the `ChtblEntry` structure containing the key-value pair, or `NULL` if the key is not found or if `chtbl` or `key` is `NULL`.

**Description:**

The returned entry pointer is valid until the entry is removed or the hash table is destroyed. Do not modify the entry structure directly.

**Complexity:** O(1) average case

**Example:**

```c
Chtbl chtbl;
chtbl_init(&chtbl, NULL, NULL, NULL, NULL);

chtbl_insert(&chtbl, "key1", "value1");

ChtblEntry* entry = chtbl_find_entry(&chtbl, "key1");
if (entry != NULL) {
    printf("Key: %s, Value: %s\n", 
        (char*)entry->key, (char*)entry->value);
}
```

---

### chtbl_remove

```c
int chtbl_remove(Chtbl* chtbl, const void * key, ChtblEntry** entry);
```

Removes a key-value pair from the hash table.

**Parameters:**

-   `chtbl` - Pointer to the hash table
-   `key` - The key to remove
-   `entry` - Optional pointer to store the removed entry. Pass `NULL` to call destroy functions

**Return Value:**

Returns `0` on success, `-1` on failure (e.g., key not found).

**Description:**

If `entry` is not `NULL`, the removed entry is stored at `*entry` and the destroy functions are **not** called (user takes ownership). If `entry` is `NULL`, the destroy functions are called on the key and value if provided.

**Complexity:** O(1) average case

**Example:**

```c
Chtbl chtbl;
chtbl_init(&chtbl, NULL, NULL, NULL, NULL);

chtbl_insert(&chtbl, "key1", "value1");

// Remove without retrieving entry (destroy functions called)
chtbl_remove(&chtbl, "key1", NULL);

// Remove and retrieve entry (user responsible for cleanup)
ChtblEntry* entry = NULL;
if (chtbl_remove(&chtbl, "key2", &entry) == 0) {
    // Use entry
    free(entry);  // User must free if needed
}
```

**Note:**

-   When retrieving an entry, the caller is responsible for freeing the memory if necessary.

---

### chtbl_clear

```c
void chtbl_clear(Chtbl* chtbl);
```

Removes all entries from the hash table.

**Parameters:**

-   `chtbl` - Pointer to the hash table

**Description:**

Removes all entries from the hash table and sets size to 0. The destroy functions are called on each key and value if provided. After clearing, the hash table is in an empty state and can be reused. The number of buckets remains unchanged.

**Complexity:** O(n) where n is the number of entries

**Example:**

```c
Chtbl chtbl;
chtbl_init(&chtbl, NULL, NULL, NULL, NULL);

chtbl_insert(&chtbl, "key1", "value1");
chtbl_insert(&chtbl, "key2", "value2");
chtbl_insert(&chtbl, "key3", "value3");
// size=3

chtbl_clear(&chtbl);
// size=0

// Hash table can be reused
chtbl_insert(&chtbl, "newkey", "newvalue");
// size=1
```

---

### chtbl_resize

```c
int chtbl_resize(Chtbl* chtbl, size_t nbuckets);
```

Resizes the hash table to a new number of buckets.

**Parameters:**

-   `chtbl` - Pointer to the hash table
-   `nbuckets` - The new number of buckets

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Resizes the hash table to the specified number of buckets. All existing entries are rehashed and preserved. This operation may improve performance by reducing collisions or reduce memory usage.

**Complexity:** O(n) where n is the number of entries

**Example:**

```c
Chtbl chtbl;
chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);

// Add many entries
for (int i = 0; i < 100; i++) {
    char key[32];
    snprintf(key, sizeof(key), "key%d", i);
    chtbl_insert(&chtbl, key, "value");
}

// Resize to reduce load factor
if (chtbl_resize(&chtbl, 128) == 0) {
    printf("Resized successfully\n");
}
```

---

## Usage Examples

### Basic Usage

```c
#include <collection/chtbl.h>
#include <stdio.h>

int main(void) {
    Chtbl chtbl;
    chtbl_init(&chtbl, NULL, NULL, NULL, NULL);

    // Insert key-value pairs
    chtbl_insert(&chtbl, "name", "Alice");
    chtbl_insert(&chtbl, "age", "30");
    chtbl_insert(&chtbl, "city", "New York");

    // Find values
    printf("Name: %s\n", (char*)chtbl_find(&chtbl, "name"));
    printf("Age: %s\n", (char*)chtbl_find(&chtbl, "age"));

    // Check if key exists
    if (chtbl_in(&chtbl, "city")) {
        printf("City key exists\n");
    }

    // Remove entry
    chtbl_remove(&chtbl, "age", NULL);

    // Cleanup
    chtbl_destroy(&chtbl);
    return 0;
}
```

### With Custom Hash and Match Functions

```c
#include <collection/chtbl.h>
#include <algorithm/hash.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    Chtbl chtbl;
    chtbl_init(&chtbl, hash_str, 
        (bool (*)(const void *, const void *))strcmp, NULL, NULL);

    chtbl_insert(&chtbl, "key1", "value1");
    chtbl_insert(&chtbl, "key2", "value2");

    void* value = chtbl_find(&chtbl, "key1");
    printf("Value: %s\n", (char*)value);

    chtbl_destroy(&chtbl);
    return 0;
}
```

### With Integer Keys

```c
#include <collection/chtbl.h>
#include <algorithm/hash.h>
#include <stdio.h>

static uint32_t hash_int_ptr(const void *key) {
    return hash_int(*(int*)key);
}

static bool match_int_ptr(const void *key1, const void *key2) {
    return *(int*)key1 == *(int*)key2;
}

int main(void) {
    Chtbl chtbl;
    chtbl_init(&chtbl, hash_int_ptr, match_int_ptr, NULL, NULL);

    int key1 = 1, key2 = 2, key3 = 3;
    chtbl_insert(&chtbl, &key1, "value1");
    chtbl_insert(&chtbl, &key2, "value2");
    chtbl_insert(&chtbl, &key3, "value3");

    void* value = chtbl_find(&chtbl, &key2);
    printf("Value for key 2: %s\n", (char*)value);

    chtbl_destroy(&chtbl);
    return 0;
}
```

### With Custom Destructors

```c
#include <collection/chtbl.h>
#include <stdlib.h>
#include <string.h>

void destroy_key(void *key) {
    free(key);
}

void destroy_value(void *value) {
    free(value);
}

int main(void) {
    Chtbl chtbl;
    chtbl_init(&chtbl, NULL, NULL, destroy_key, destroy_value);

    char *key1 = strdup("key1");
    char *value1 = strdup("value1");
    char *key2 = strdup("key2");
    char *value2 = strdup("value2");

    chtbl_insert(&chtbl, key1, value1);
    chtbl_insert(&chtbl, key2, value2);

    // All keys and values will be freed automatically
    chtbl_destroy(&chtbl);
    return 0;
}
```

### Load Factor Management

```c
#include <collection/chtbl.h>
#include <stdio.h>

int main(void) {
    Chtbl chtbl;
    chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);

    // Set load factor threshold
    chtbl_set_load_factor(&chtbl, 0.75f);

    // Add entries
    for (int i = 0; i < 20; i++) {
        char key[32];
        snprintf(key, sizeof(key), "key%d", i);
        chtbl_insert(&chtbl, key, "value");
    }

    // Check current load factor
    float load = chtbl_load_factor(&chtbl);
    printf("Current load factor: %.2f\n", load);

    // Resize if needed
    if (load > 0.75f) {
        chtbl_resize(&chtbl, chtbl_buckets(&chtbl) * 2);
    }

    chtbl_destroy(&chtbl);
    return 0;
}
```

---

## Complexity

| Operation              | Complexity      |
| ---------------------- | --------------- |
| `chtbl_init`           | O(1)            |
| `chtbl_init_capacity`  | O(n)            |
| `chtbl_destroy`        | O(n)            |
| `chtbl_size`           | O(1)            |
| `chtbl_buckets`        | O(1)            |
| `chtbl_set_load_factor`| O(1)            |
| `chtbl_load_factor`    | O(1)            |
| `chtbl_insert`         | O(1) average    |
| `chtbl_find`           | O(1) average    |
| `chtbl_find_entry`     | O(1) average    |
| `chtbl_in`             | O(1) average    |
| `chtbl_remove`         | O(1) average    |
| `chtbl_clear`          | O(n)            |
| `chtbl_resize`         | O(n)            |

Where n is the number of entries. Average case complexities assume a good hash function that distributes keys uniformly.

---

## Important Notes

### Memory Management

-   The hash table manages its own memory allocation for entries and buckets
-   The destroy functions are called automatically when entries are removed
-   The hash table never calls `free()` on key or value pointers unless the destroy functions do so
-   Insert operations may fail due to memory allocation failure

### Hash Functions

-   A good hash function should distribute keys uniformly across buckets
-   The default hash function (`hash_str`) is suitable for string keys
-   For custom key types, provide an appropriate hash function
-   Poor hash functions can degrade performance to O(n) worst case

### Collision Resolution

-   The hash table uses separate chaining (linked lists) for collision resolution
-   Keys that hash to the same bucket are stored in a chain
-   Performance depends on the average chain length (load factor)

### Load Factor

-   Load factor = number of entries / number of buckets
-   Higher load factors increase collision probability
-   Lower load factors use more memory but reduce collisions
-   Typical optimal load factor is between 0.5 and 0.75

### Key Updates

-   Inserting a key that already exists updates the associated value
-   The old value is destroyed if destroy functions are provided
-   The size of the hash table does not increase when updating existing keys

### Thread Safety

Hash table operations are **not thread-safe**. External synchronization is required for concurrent access.

### Undefined Behavior

Avoid the following:

-   Passing `NULL` hash table pointers to any function (except where explicitly allowed)
-   Using a hash table after calling `chtbl_destroy()` without re-initializing
-   Assuming insert operations always succeed (check return value)
-   Using entry pointers returned by `chtbl_find_entry()` after the entry has been removed or the hash table has been destroyed
-   Modifying the `ChtblEntry` structure directly
-   Using keys or values after they have been destroyed by the hash table

