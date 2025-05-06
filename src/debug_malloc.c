#include "../includes/malloc.h"
#include <stdio.h>

/*
 * Bonus function that provides more detailed memory information
 * This is useful for debugging purposes
 */
void show_alloc_mem_ex(void)
{
    t_zone *zone;
    t_block *block;
    size_t total_allocated = 0;
    size_t total_free = 0;
    int block_count = 0;
    
    // Lock mutex for thread safety
    pthread_mutex_lock(&g_mutex);
    
    printf("\n=== DETAILED MEMORY ALLOCATION REPORT ===\n\n");
    
    // Process tiny zones
    printf("TINY ZONES:\n");
    zone = g_heap.tiny_zones;
    while (zone)
    {
        printf("  Zone at %p, size: %zu bytes\n", zone, zone->zone_size);
        
        block = zone->blocks;
        while (block)
        {
            block_count++;
            printf("    Block #%d at %p: size=%zu, %s\n", 
                   block_count,
                   (void *)((char *)block + sizeof(t_block)),
                   block->size,
                   block->is_free ? "FREE" : "ALLOCATED");
            
            if (block->is_free)
                total_free += block->size;
            else
                total_allocated += block->size;
            
            block = block->next;
        }
        zone = zone->next;
    }
    
    // Process small zones
    printf("\nSMALL ZONES:\n");
    zone = g_heap.small_zones;
    while (zone)
    {
        printf("  Zone at %p, size: %zu bytes\n", zone, zone->zone_size);
        
        block = zone->blocks;
        while (block)
        {
            block_count++;
            printf("    Block #%d at %p: size=%zu, %s\n", 
                   block_count,
                   (void *)((char *)block + sizeof(t_block)),
                   block->size,
                   block->is_free ? "FREE" : "ALLOCATED");
            
            if (block->is_free)
                total_free += block->size;
            else
                total_allocated += block->size;
            
            block = block->next;
        }
        zone = zone->next;
    }
    
    // Process large zones
    printf("\nLARGE ZONES:\n");
    zone = g_heap.large_zones;
    while (zone)
    {
        printf("  Zone at %p, size: %zu bytes\n", zone, zone->zone_size);
        
        block = zone->blocks;
        while (block)
        {
            block_count++;
            printf("    Block #%d at %p: size=%zu, %s\n", 
                   block_count,
                   (void *)((char *)block + sizeof(t_block)),
                   block->size,
                   block->is_free ? "FREE" : "ALLOCATED");
            
            if (block->is_free)
                total_free += block->size;
            else
                total_allocated += block->size;
            
            block = block->next;
        }
        zone = zone->next;
    }
    
    // Print summary
    printf("\n=== MEMORY ALLOCATION SUMMARY ===\n");
    printf("Total blocks: %d\n", block_count);
    printf("Total allocated: %zu bytes\n", total_allocated);
    printf("Total free: %zu bytes\n", total_free);
    printf("Allocation efficiency: %.2f%%\n", 
           (total_allocated + total_free > 0) ? 
           ((float)total_allocated / (total_allocated + total_free) * 100.0) : 0.0);
    
    // Unlock mutex
    pthread_mutex_unlock(&g_mutex);
}

/*
 * Hexdump of a memory region
 * This is a helper function for debugging
 */
void hexdump(void *ptr, size_t size)
{
    unsigned char *p = (unsigned char *)ptr;
    size_t i, j;
    
    for (i = 0; i < size; i += 16)
    {
        printf("%p: ", p + i);
        
        // Print hex values
        for (j = 0; j < 16 && i + j < size; j++)
        {
            printf("%02x ", p[i + j]);
            if (j == 7)
                printf(" ");
        }
        
        // Fill remaining space for alignment
        for (; j < 16; j++)
        {
            printf("   ");
            if (j == 7)
                printf(" ");
        }
        
        printf(" |");
        
        // Print ASCII values
        for (j = 0; j < 16 && i + j < size; j++)
        {
            char c = p[i + j];
            printf("%c", (c >= 32 && c <= 126) ? c : '.');
        }
        
        printf("|\n");
    }
}