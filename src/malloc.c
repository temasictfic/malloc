#include "../includes/malloc.h"

/*
 * allocate_memory - Internal function that handles the actual allocation
 * @size: Size to allocate
 *
 * This function handles the internal details of memory allocation without
 * the mutex locking/unlocking which is done in the public malloc function.
 *
 * Return: Pointer to allocated memory, or NULL if allocation fails
 */
void *allocate_memory(size_t size)
{
    t_zone *zone;
    t_block *block;
    
    // Align size to ensure proper memory alignment
    size = align_size(size);
    
    // Get an appropriate zone for this size
    zone = get_zone_for_size(size);
    if (!zone)
        return (NULL);
    
    // For large allocations, the zone was created specifically for this allocation
    if (size > SMALL_MAX)
    {
        block = zone->blocks;
        return ((void *)((char *)block + sizeof(t_block)));
    }
    
    // Find a free block in the zone
    block = find_free_block(zone, size);
    if (!block)
        return (NULL);
    
    // If the block is larger than needed, split it
    if (block->size >= size + sizeof(t_block) + 16)  // Minimum useful block size is 16 bytes
    {
        split_block(block, size);
    }
    
    // Mark the block as used
    block->is_free = false;
    
    // Return a pointer to the usable area of the block (after the metadata)
    return ((void *)((char *)block + sizeof(t_block)));
}

/*
 * malloc - Allocate memory
 * @size: Size to allocate
 *
 * This function allocates size bytes of memory and returns a pointer
 * to the allocated memory. The memory is not initialized.
 *
 * Return: Pointer to allocated memory, or NULL if allocation fails
 */
void *malloc(size_t size)
{
    void *ptr;
    
    // Handle edge case
    if (size == 0)
        return (NULL);
    
    // Lock mutex for thread safety
    pthread_mutex_lock(&g_mutex);
    
    // Perform the allocation
    ptr = allocate_memory(size);
    
    // Unlock mutex
    pthread_mutex_unlock(&g_mutex);
    
    return (ptr);
}