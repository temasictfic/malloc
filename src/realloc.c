#include "../includes/malloc.h"
#include <string.h>

/*
 * Try to expand a block in-place
 * Returns true if successful, false if not possible
 */
static bool try_expand_block(t_block *block, size_t new_size)
{
    t_block *next_block;
    size_t available_size;
    
    // If there's no next block, we can't expand
    if (!block->next)
        return (false);
    
    next_block = block->next;
    
    // If the next block is not free, we can't expand
    if (!next_block->is_free)
        return (false);
    
    // Calculate total available size including the next block
    available_size = block->size + sizeof(t_block) + next_block->size;
    
    // Check if the available size is enough
    if (available_size < new_size)
        return (false);
    
    // Expand the current block
    block->size = available_size;
    block->next = next_block->next;
    
    // Update the next-next block's prev pointer if it exists
    if (next_block->next)
        next_block->next->prev = block;
    
    // If the expanded block is much larger than needed, split it
    if (block->size >= new_size + sizeof(t_block) + 16)
        split_block(block, new_size);
    
    return (true);
}

/*
 * Copy a memory block to a new allocation
 */
static void *copy_and_free(void *ptr, size_t size)
{
    void *new_ptr;
    t_block *block;
    
    // Find the block corresponding to the old pointer
    block = find_block(ptr);
    if (!block)
        return (NULL);
    
    // Allocate new memory
    new_ptr = allocate_memory(size);
    if (!new_ptr)
        return (NULL);
    
    // Copy data from old to new allocation
    // Use the smaller of the old and new sizes to avoid overflow
    memcpy(new_ptr, ptr, block->size < size ? block->size : size);
    
    // Free the old allocation
    block->is_free = true;
    
    // Find the zone and merge adjacent free blocks
    t_zone *zone = find_zone_containing_ptr(ptr);
    if (zone)
    {
        merge_free_blocks(zone);
        try_free_zone(zone);
    }
    
    return (new_ptr);
}

/*
 * realloc - Change the size of a memory block
 * @ptr: Pointer to memory to resize
 * @size: New size
 *
 * This function changes the size of the memory block pointed to by ptr
 * to size bytes. The contents will be unchanged in the range from the start
 * of the region up to the minimum of the old and new sizes.
 *
 * Return: Pointer to the new allocation, or NULL if allocation fails
 */
void *realloc(void *ptr, size_t size)
{
    t_block *block;
    void *new_ptr;
    
    // If ptr is NULL, act like malloc
    if (!ptr)
        return (malloc(size));
    
    // If size is 0, act like free
    if (size == 0)
    {
        free(ptr);
        return (NULL);
    }
    
    // Lock mutex for thread safety
    pthread_mutex_lock(&g_mutex);
    
    // Find the block corresponding to this pointer
    block = find_block(ptr);
    if (!block)
    {
        // Invalid pointer
        pthread_mutex_unlock(&g_mutex);
        return (NULL);
    }
    
    // Align the size
    size = align_size(size);
    
    // If the new size is smaller than or equal to the current size, we can reuse the block
    if (size <= block->size)
    {
        // If the block is much larger than needed, split it
        if (block->size >= size + sizeof(t_block) + 16)
            split_block(block, size);
        
        pthread_mutex_unlock(&g_mutex);
        return (ptr);
    }
    
    // Try to expand the block in-place by merging with next free block
    if (try_expand_block(block, size))
    {
        pthread_mutex_unlock(&g_mutex);
        return (ptr);
    }
    
    // If we can't expand in-place, allocate a new block and copy data
    new_ptr = copy_and_free(ptr, size);
    
    // Unlock mutex
    pthread_mutex_unlock(&g_mutex);
    
    return (new_ptr);
}