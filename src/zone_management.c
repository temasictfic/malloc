#include "../includes/malloc.h"

/*
 * Global variables
 */
t_heap g_heap = {NULL, NULL, NULL};
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Align size to ensure proper memory alignment
 * This is critical for performance and compatibility
 */
size_t align_size(size_t size)
{
    return ((size + 15) & ~15);
}

/*
 * Create a new zone with a specified size
 * Returns NULL if allocation fails
 */
t_zone *create_zone(size_t zone_size, size_t alloc_size)
{
    t_zone *zone;
    t_block *block;
    
    // Allocate memory for the zone using mmap
    zone = mmap(NULL, zone_size, MMAP_PROT, MMAP_FLAGS, -1, 0);
    if (zone == MAP_FAILED)
        return (NULL);
    
    // Initialize zone metadata
    zone->zone_size = zone_size;
    zone->next = NULL;
    
    // Initialize the first block in the zone
    block = (t_block *)((char *)zone + sizeof(t_zone));
    zone->blocks = block;
    
    // Set up the first block to cover the entire zone (minus metadata)
    block->size = zone_size - sizeof(t_zone) - sizeof(t_block);
    block->is_free = true;
    block->next = NULL;
    block->prev = NULL;
    
    // If we have a specific allocation size, adjust the first block
    if (alloc_size > 0)
    {
        // Align the allocation size
        alloc_size = align_size(alloc_size);
        
        // If the allocation size is smaller than the block, split it
        if (alloc_size + sizeof(t_block) < block->size)
        {
            split_block(block, alloc_size);
        }
        
        // Mark the first block as used
        block->is_free = false;
    }
    
    return (zone);
}

/*
 * Find a free block of the required size in the given zone
 * Returns NULL if no suitable block is found
 */
t_block *find_free_block(t_zone *zone, size_t size)
{
    t_block *block;
    
    if (!zone)
        return (NULL);
    
    block = zone->blocks;
    while (block)
    {
        if (block->is_free && block->size >= size)
            return (block);
        block = block->next;
    }
    
    return (NULL);
}

/*
 * Split a block into two: one of the requested size and one with the remainder
 * Returns the second block (remainder)
 */
t_block *split_block(t_block *block, size_t size)
{
    t_block *new_block;
    
    // Calculate the address of the new block
    new_block = (t_block *)((char *)block + sizeof(t_block) + size);
    
    // Initialize the new block
    new_block->size = block->size - size - sizeof(t_block);
    new_block->is_free = true;
    new_block->next = block->next;
    new_block->prev = block;
    
    // Update the original block
    block->size = size;
    block->next = new_block;
    
    // Update the next block's prev pointer if it exists
    if (new_block->next)
        new_block->next->prev = new_block;
    
    return (new_block);
}

/*
 * Get the appropriate zone for a given size
 * Returns NULL if no suitable zone is found
 */
t_zone *get_zone_for_size(size_t size)
{
    t_zone *zone;
    t_zone **zone_list;
    size_t zone_size;
    
    // Determine which zone list to use based on size
    if (size <= TINY_MAX)
    {
        zone_list = &g_heap.tiny_zones;
        zone_size = TINY_ZONE_SIZE;
    }
    else if (size <= SMALL_MAX)
    {
        zone_list = &g_heap.small_zones;
        zone_size = SMALL_ZONE_SIZE;
    }
    else
    {
        // For large allocations, create a dedicated zone
        zone_size = align_size(size + sizeof(t_zone) + sizeof(t_block));
        zone_size = ((zone_size / getpagesize()) + 1) * getpagesize();
        zone = create_zone(zone_size, size);
        
        // Add to large zones list
        if (zone)
        {
            zone->next = g_heap.large_zones;
            g_heap.large_zones = zone;
        }
        return (zone);
    }
    
    // Look for a zone with a free block of the required size
    zone = *zone_list;
    while (zone)
    {
        if (find_free_block(zone, size))
            return (zone);
        zone = zone->next;
    }
    
    // If no suitable zone is found, create a new one
    zone = create_zone(zone_size, 0);
    if (!zone)
        return (NULL);
    
    // Add the new zone to the beginning of the list
    zone->next = *zone_list;
    *zone_list = zone;
    
    return (zone);
}

/*
 * Find the zone containing a pointer
 * Returns NULL if the pointer is not in any zone
 */
t_zone *find_zone_containing_ptr(void *ptr)
{
    t_zone *zone;
    
    // Check tiny zones
    zone = g_heap.tiny_zones;
    while (zone)
    {
        if (ptr >= (void *)zone && ptr < (void *)((char *)zone + zone->zone_size))
            return (zone);
        zone = zone->next;
    }
    
    // Check small zones
    zone = g_heap.small_zones;
    while (zone)
    {
        if (ptr >= (void *)zone && ptr < (void *)((char *)zone + zone->zone_size))
            return (zone);
        zone = zone->next;
    }
    
    // Check large zones
    zone = g_heap.large_zones;
    while (zone)
    {
        if (ptr >= (void *)zone && ptr < (void *)((char *)zone + zone->zone_size))
            return (zone);
        zone = zone->next;
    }
    
    return (NULL);
}

/*
 * Merge adjacent free blocks in a zone
 * This helps to defragment memory
 */
void merge_free_blocks(t_zone *zone)
{
    t_block *block;
    
    if (!zone)
        return;
    
    block = zone->blocks;
    while (block && block->next)
    {
        if (block->is_free && block->next->is_free)
        {
            // Merge with the next block
            block->size += sizeof(t_block) + block->next->size;
            block->next = block->next->next;
            
            // Update the next block's prev pointer if it exists
            if (block->next)
                block->next->prev = block;
            
            // Continue checking from the current block
            continue;
        }
        block = block->next;
    }
}