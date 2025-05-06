#ifndef MALLOC_H
# define MALLOC_H

# include <stdlib.h>
# include <unistd.h>
# include <sys/mman.h>
# include <pthread.h>
# include <stddef.h>
# include <stdbool.h>

/*
 * Memory allocation constants
 * These values are carefully chosen to balance performance and memory usage
 */

# define TINY_MAX 128        // Max size for tiny allocations
# define SMALL_MAX 1024      // Max size for small allocations

/*
 * Calculate zone sizes to ensure at least 100 allocations per zone
 * and align to page size
 */
# define ZONE_PAGE_MULTIPLIER(max_size) ((size_t)((((max_size) * 100) / getpagesize() + 1) * getpagesize()))

# define TINY_ZONE_SIZE ZONE_PAGE_MULTIPLIER(TINY_MAX)
# define SMALL_ZONE_SIZE ZONE_PAGE_MULTIPLIER(SMALL_MAX)

/*
 * Flags for mmap
 */
# define MMAP_PROT (PROT_READ | PROT_WRITE)
# define MMAP_FLAGS (MAP_PRIVATE | MAP_ANONYMOUS)

/*
 * Memory block metadata
 */
typedef struct s_block {
    size_t          size;       // Actual size of the block
    bool            is_free;    // Whether the block is free
    struct s_block  *next;      // Next block in the zone
    struct s_block  *prev;      // Previous block in the zone
} t_block;

/*
 * Memory zone structure
 */
typedef struct s_zone {
    size_t          zone_size;  // Total size of this zone
    t_block         *blocks;    // First block in the zone
    struct s_zone   *next;      // Next zone of the same type
} t_zone;

/*
 * Heap structure
 */
typedef struct s_heap {
    t_zone  *tiny_zones;    // List of tiny zones
    t_zone  *small_zones;   // List of small zones
    t_zone  *large_zones;   // List of large zones
} t_heap;

/*
 * Global variables
 */
extern t_heap g_heap;                // The heap structure
extern pthread_mutex_t g_mutex;      // Mutex for thread safety

/*
 * Core malloc functions
 */
void    *malloc(size_t size);
void    free(void *ptr);
void    *realloc(void *ptr, size_t size);
void    show_alloc_mem(void);

/*
 * Bonus functions
 */
void    show_alloc_mem_ex(void);
void    hexdump(void *ptr, size_t size);

/*
 * Internal helper functions
 */
void    *allocate_memory(size_t size);
t_zone  *create_zone(size_t zone_size, size_t alloc_size);
t_block *find_free_block(t_zone *zone, size_t size);
t_block *find_block(void *ptr);
t_block *split_block(t_block *block, size_t size);
t_zone  *get_zone_for_size(size_t size);
t_zone  *find_zone_containing_ptr(void *ptr);
void    merge_free_blocks(t_zone *zone);
void    try_free_zone(t_zone *zone);
size_t  align_size(size_t size);

#endif /* MALLOC_H */