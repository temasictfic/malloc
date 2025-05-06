# Dynamic Memory Allocation Implementation (malloc)

This project implements a custom dynamic memory allocation mechanism in C, including the following functions:
- `malloc(size_t size)`: Allocate memory
- `free(void *ptr)`: Deallocate memory
- `realloc(void *ptr, size_t size)`: Resize allocated memory
- `show_alloc_mem()`: Display allocated memory

## Key Features

- Thread-safe implementation using pthread mutex
- Efficient memory management with pre-allocated zones
- Memory alignment for optimal performance
- Memory defragmentation via block merging
- Detailed memory visualization tools

## Memory Zone Structure

The implementation uses three types of memory zones:

1. **TINY**: For allocations from 1 to 128 bytes
2. **SMALL**: For allocations from 129 to 1024 bytes
3. **LARGE**: For allocations larger than 1024 bytes

Each zone (except LARGE) is pre-allocated to reduce system calls and improve performance.

## Building the Library

To build the library, simply run:

```
make
```

This will compile the library and create:
- `libft_malloc_$HOSTTYPE.so` - The dynamic library
- `libft_malloc.so` - A symbolic link to the library

## Using the Library

### Linking with the Library

To use this implementation in your programs, you have two options:

1. **At compile time**:
   ```
   gcc -o your_program your_code.c -L. -lft_malloc
   ```

2. **At runtime using LD_PRELOAD**:
   ```
   export LD_PRELOAD=./libft_malloc.so
   ./your_program
   ```

### Memory Visualization

To display the current memory allocation state in your program:

```c
#include "includes/malloc.h"

// Show basic memory allocation information
show_alloc_mem();

// Show detailed memory allocation information (bonus)
show_alloc_mem_ex();

// Display hexadecimal dump of memory region (bonus)
hexdump(ptr, size);
```

## Testing

The project includes comprehensive test suites. To run them:

```
make test
```

This will compile and run:
1. Basic tests that verify core functionality
2. Comprehensive tests that cover various allocation patterns, thread safety, and edge cases

## Project Structure

```
.
├── Makefile
├── includes/
│   └── malloc.h         # Main header file
├── src/
│   ├── malloc.c         # Implementation of malloc
│   ├── free.c           # Implementation of free
│   ├── realloc.c        # Implementation of realloc
│   ├── show_alloc_mem.c # Implementation of show_alloc_mem
│   ├── zone_management.c # Zone and block management functions
│   └── debug_malloc.c   # Bonus debugging functions
├── libft/               # libft library
├── tests/
│   ├── test_malloc.c    # Basic test program
│   └── comprehensive_test.c # Comprehensive test program
└── README.md
```

## Implementation Details

### Memory Block Structure

Each memory block has metadata including:
- Size of the block
- Free/used status
- Pointers to next/previous blocks

### Memory Zone Management

Zones are organized in linked lists for each size category (TINY, SMALL, LARGE). When a new allocation is requested:

1. The appropriate zone type is determined based on the requested size
2. If a suitable free block exists, it is used (and potentially split)
3. If no suitable block exists, a new zone is created

### Thread Safety

All operations are protected by a mutex to ensure thread safety in multi-threaded applications.

### Bonus Features

The implementation includes the following bonus features:
- Thread safety via pthread mutex
- Enhanced debugging with `show_alloc_mem_ex()`
- Memory region visualization with `hexdump()`