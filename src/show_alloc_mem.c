#include "../includes/malloc.h"
#include <stdio.h>

/*
 * Print information about blocks in a zone
 * Returns the total allocated bytes in the zone
 */
static size_t print_zone_blocks(t_zone *zone)
{
    t_block *block;
    void *block_start;
    void *block_end;
    size_t total;
    
    total = 0;
    block = zone->blocks;
    
    while (block)
    {
        if (!block->is_free)
        {
            block_start = (void *)((char *)block + sizeof(t_block));
            block_end = (void *)((char *)block_start + block->size);
            printf("%p - %p : %zu bytes\n", block_start, block_end, block->size);
            total += block->size;
        }
        block = block->next;
    }
    
    return (total);
}

/*
 * Print information about all zones and their blocks
 */
void show_alloc_mem(void)
{
    t_zone *zone;
    size_t total;
    
    // Lock mutex for thread safety
    pthread_mutex_lock(&g_mutex);
    
    total = 0;
    
    // Print tiny zones
    if (g_heap.tiny_zones)
    {
        printf("TINY : %p\n", g_heap.tiny_zones);
        zone = g_heap.tiny_zones;
        while (zone)
        {
            total += print_zone_blocks(zone);
            zone = zone->next;
        }
    }
    
    // Print small zones
    if (g_heap.small_zones)
    {
        printf("SMALL : %p\n", g_heap.small_zones);
        zone = g_heap.small_zones;
        while (zone)
        {
            total += print_zone_blocks(zone);
            zone = zone->next;
        }
    }
    
    // Print large zones
    if (g_heap.large_zones)
    {
        printf("LARGE : %p\n", g_heap.large_zones);
        zone = g_heap.large_zones;
        while (zone)
        {
            total += print_zone_blocks(zone);
            zone = zone->next;
        }
    }
    
    printf("Total : %zu bytes\n", total);
    
    // Unlock mutex
    pthread_mutex_unlock(&g_mutex);
}