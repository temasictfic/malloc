#include "../includes/malloc.h"

/*
 * Find the block corresponding to a user pointer
 */
t_block *find_block(void *ptr)
{
    t_zone *zone;
    t_block *block;
    
    // Find the zone containing this pointer
    zone = find_zone_containing_ptr(ptr);
    if (!zone)
        return (NULL);
    
    // Iterate through blocks in the zone to find the matching one
    block = zone->blocks;
    while (block)
    {
        void *block_data = (void *)((char *)block + sizeof(t_block));
        if (block_data == ptr)
            return (block);
        block = block->next;
    }
    
    return (NULL);
}

/*
 * Unmap a large zone
 */
static void free_large_zone(t_zone *zone)
{
    t_zone *current;
    t_zone *prev;
    
    prev = NULL;
    current = g_heap.large_zones;
    
    while (current)
    {
        if (current == zone)
        {
            // Remove from linked list
            if (prev)
                prev->next = current->next;
            else
                g_heap.large_zones = current->next;
            
            // Unmap the zone
            munmap(zone, zone->zone_size);
            return;
        }
        prev = current;
        current = current->next;
    }
}

/*
 * Check if a zone is completely free and can be unmapped
 */
static int can_free_zone(t_zone *zone)
{
    t_block *block;
    
    // Only one block that covers the entire usable space
    block = zone->blocks;
    return (block && block->is_free && !block->next);
}

/*
 * Try to free a zone if it's completely unused
 */
void try_free_zone(t_zone *zone)
{
    t_zone *current;
    t_zone *prev;
    t_zone **zone_list;
    
    // Only attempt to free regular zones (not large ones)
    if (zone->zone_size == TINY_ZONE_SIZE)
        zone_list = &g_heap.tiny_zones;
    else if (zone->zone_size == SMALL_ZONE_SIZE)
        zone_list = &g_heap.small_zones;
    else
        return;
    
    // Check if zone is completely free
    if (!can_free_zone(zone))
        return;
    
    // Find the zone in the appropriate list
    prev = NULL;
    current = *zone_list;
    
    while (current)
    {
        if (current == zone)
        {
            // Remove from linked list
            if (prev)
                prev->next = current->next;
            else
                *zone_list = current->next;
            
            // Unmap the zone
            munmap(zone, zone->zone_size);
            return;
        }
        prev = current;
        current = current->next;
    }
}

/*
 * free - Free previously allocated memory
 * @ptr: Pointer to memory to free
 *
 * This function frees the memory space pointed to by ptr, which must have been
 * returned by a previous call to malloc or realloc. If ptr is NULL, no operation
 * is performed.
 */
void free(void *ptr)
{
    t_block *block;
    t_zone *zone;
    
    // Handle NULL pointer
    if (!ptr)
        return;
    
    // Lock mutex for thread safety
    pthread_mutex_lock(&g_mutex);
    
    // Find the block corresponding to this pointer
    block = find_block(ptr);
    if (!block)
    {
        // Invalid pointer, ignore
        pthread_mutex_unlock(&g_mutex);
        return;
    }
    
    // Mark the block as free
    block->is_free = true;
    
    // Find the zone containing this block
    zone = find_zone_containing_ptr(ptr);
    if (!zone)
    {
        // This should never happen if find_block succeeded
        pthread_mutex_unlock(&g_mutex);
        return;
    }
    
    // For large allocations, unmap the zone
    if (zone->zone_size > SMALL_ZONE_SIZE)
    {
        free_large_zone(zone);
        pthread_mutex_unlock(&g_mutex);
        return;
    }
    
    // Merge adjacent free blocks
    merge_free_blocks(zone);
    
    // Try to free the zone if it's completely unused
    try_free_zone(zone);
    
    // Unlock mutex
    pthread_mutex_unlock(&g_mutex);
}