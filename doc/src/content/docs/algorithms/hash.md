---
title: Hash
description: Hash functions for strings and integers
---

The hash module provides efficient hash functions for common data types, suitable for use in hash tables and other data structures requiring fast key hashing.

## Header Files

To use the hash functions in your code, include the header file:

```c
#include <algorithm/hash.h>
```

This provides access to all hash functions.

## Functions

Public interfaces for hash operations:

### hash_str

```c
uint32_t hash_str(const char * key);
```

Hashes a null-terminated string.

**Parameters:**

-   `key` - Pointer to the null-terminated string to hash

**Return Value:**

Returns a 32-bit unsigned integer hash value.

**Description:**

Computes a hash value for the given null-terminated string. The hash function processes characters until it encounters the null terminator. The same string will always produce the same hash value.

**Complexity:** O(n) where n is the length of the string

**Example:**

```c
const char* str = "hello";
uint32_t hash = hash_str(str);
printf("Hash of 'hello': %u\n", hash);

// Same string produces same hash
uint32_t hash2 = hash_str("hello");
// hash == hash2
```

**Notes:**

-   The function processes the string until the null terminator (`\0`) is encountered
-   Empty strings (containing only `\0`) produce a valid hash value
-   The hash function is case-sensitive: `"Hello"` and `"hello"` produce different hashes
-   Whitespace is significant: `"hello"` and `"hello "` produce different hashes

---

### hash_int

```c
uint32_t hash_int(const int key);
```

Hashes an integer value.

**Parameters:**

-   `key` - The integer value to hash

**Return Value:**

Returns a 32-bit unsigned integer hash value.

**Description:**

Computes a hash value for the given integer. The same integer will always produce the same hash value. The function handles both positive and negative integers.

**Complexity:** O(1)

**Example:**

```c
int value = 42;
uint32_t hash = hash_int(value);
printf("Hash of 42: %u\n", hash);

// Same integer produces same hash
uint32_t hash2 = hash_int(42);
// hash == hash2

// Different integers produce different hashes
uint32_t hash3 = hash_int(43);
// hash != hash3
```

**Notes:**

-   The function handles all integer values including `INT_MIN` and `INT_MAX`
-   Negative and positive integers produce different hash values
-   Zero produces a valid hash value

---

## Usage Examples

### Basic Usage

```c
#include <algorithm/hash.h>
#include <stdio.h>

int main(void) {
    // Hash strings
    const char* str1 = "hello";
    const char* str2 = "world";
    
    uint32_t hash1 = hash_str(str1);
    uint32_t hash2 = hash_str(str2);
    
    printf("Hash of '%s': %u\n", str1, hash1);
    printf("Hash of '%s': %u\n", str2, hash2);
    
    // Hash integers
    int val1 = 42;
    int val2 = -42;
    
    uint32_t hash3 = hash_int(val1);
    uint32_t hash4 = hash_int(val2);
    
    printf("Hash of %d: %u\n", val1, hash3);
    printf("Hash of %d: %u\n", val2, hash4);
    
    return 0;
}
```

### Using with Hash Tables

```c
#include <algorithm/hash.h>
#include <collection/chtbl.h>

// Hash function wrapper for string keys
uint32_t string_hash(const void* key) {
    return hash_str((const char*)key);
}

// Hash function wrapper for integer keys
uint32_t int_hash(const void* key) {
    return hash_int(*(const int*)key);
}

int main(void) {
    // Create hash table with string keys
    CHashTable table;
    chtbl_init(&table, string_hash, NULL, NULL);
    
    // Use hash table...
    
    chtbl_destroy(&table);
    return 0;
}
```

### Deterministic Hashing

```c
#include <algorithm/hash.h>
#include <assert.h>

int main(void) {
    // Same input always produces same output
    const char* str = "test";
    uint32_t hash1 = hash_str(str);
    uint32_t hash2 = hash_str(str);
    uint32_t hash3 = hash_str("test");
    
    assert(hash1 == hash2);
    assert(hash2 == hash3);
    
    // Same integer always produces same output
    int val = 12345;
    uint32_t hash4 = hash_int(val);
    uint32_t hash5 = hash_int(val);
    uint32_t hash6 = hash_int(12345);
    
    assert(hash4 == hash5);
    assert(hash5 == hash6);
    
    return 0;
}
```

### Edge Cases

```c
#include <algorithm/hash.h>
#include <stdio.h>
#include <limits.h>

int main(void) {
    // Empty string
    uint32_t hash1 = hash_str("");
    printf("Empty string hash: %u\n", hash1);
    
    // Single character
    uint32_t hash2 = hash_str("a");
    printf("Single char hash: %u\n", hash2);
    
    // Integer edge cases
    uint32_t hash3 = hash_int(0);
    uint32_t hash4 = hash_int(INT_MAX);
    uint32_t hash5 = hash_int(INT_MIN);
    
    printf("Zero hash: %u\n", hash3);
    printf("INT_MAX hash: %u\n", hash4);
    printf("INT_MIN hash: %u\n", hash5);
    
    return 0;
}
```

---

## Complexity

| Operation    | Complexity |
| ------------ | ---------- |
| `hash_str`   | O(n)       |
| `hash_int`   | O(1)       |

Where n is the length of the string.

---

## Important Notes

### Deterministic Behavior

-   The same input will always produce the same hash value
-   This property is essential for hash table operations and caching
-   Hash values are consistent across multiple calls with the same input

### Distribution

-   Hash functions aim to provide uniform distribution of hash values
-   Different inputs should produce different hash values with high probability
-   Hash collisions are possible but should be rare for well-distributed inputs

### String Hashing

-   The function processes characters until the null terminator is encountered
-   Strings are hashed as-is: case-sensitive and whitespace-sensitive
-   Empty strings produce valid hash values
-   The function does not validate that the string is properly null-terminated; undefined behavior may occur if the string is not null-terminated

### Integer Hashing

-   All integer values are supported, including `INT_MIN` and `INT_MAX`
-   Negative and positive integers produce different hash values
-   The function treats integers as signed values

### Thread Safety

Hash functions are **thread-safe** as they operate on immutable input data and do not maintain internal state. Multiple threads can call these functions concurrently without synchronization.

### Undefined Behavior

Avoid the following:

-   Passing a non-null-terminated string to `hash_str()` (may cause buffer overread)
-   Passing `NULL` to `hash_str()` (undefined behavior)
-   Modifying the string during hash computation (may produce inconsistent results)

### Use Cases

Hash functions are commonly used for:

-   Hash table implementations
-   Distributed systems for consistent hashing
-   Caching mechanisms
-   Data structure indexing
-   Checksumming and fingerprinting

---

## Performance Considerations

### String Hashing

-   Performance is linear with string length
-   For very long strings, consider hashing only a prefix or using a rolling hash
-   String hashing is generally fast for typical key lengths

### Integer Hashing

-   Integer hashing is constant time and very fast
-   Suitable for high-frequency operations
-   No memory allocation is performed

### Collision Handling

While hash functions aim to minimize collisions, collisions can still occur. When using these hash functions with hash tables:

-   Implement proper collision resolution (chaining, open addressing, etc.)
-   Monitor load factors and resize tables as needed
-   Consider using additional hash functions for double hashing if needed


