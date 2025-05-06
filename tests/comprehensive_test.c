#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "../includes/malloc.h"

#define NUM_ALLOCATIONS 1000
#define MAX_ALLOC_SIZE 4096
#define NUM_THREADS 8
#define ITERATIONS_PER_THREAD 100

// Array to hold pointers
void *ptr_array[NUM_ALLOCATIONS];
int size_array[NUM_ALLOCATIONS];

/*
 * Test various malloc/free patterns
 */
void test_malloc_free_patterns(void)
{
    printf("\n=== Test: Various malloc/free patterns ===\n");
    
    // Initialize the random number generator
    srand(time(NULL));
    
    // Test 1: Allocate a lot of small blocks
    printf("1. Allocating %d small blocks...\n", NUM_ALLOCATIONS);
    for (int i = 0; i < NUM_ALLOCATIONS; i++)
    {
        // Random size between 1 and 100 bytes
        size_array[i] = rand() % 100 + 1;
        ptr_array[i] = malloc(size_array[i]);
        
        if (!ptr_array[i])
        {
            printf("Failed to allocate block %d\n", i);
            break;
        }
        
        // Fill memory with a pattern
        memset(ptr_array[i], i % 256, size_array[i]);
    }
    
    // Test 2: Free blocks in random order
    printf("2. Freeing blocks in random order...\n");
    for (int i = 0; i < NUM_ALLOCATIONS; i++)
    {
        int idx = rand() % NUM_ALLOCATIONS;
        
        // Skip if already freed
        if (!ptr_array[idx])
            continue;
        
        free(ptr_array[idx]);
        ptr_array[idx] = NULL;
    }
    
    // Ensure all blocks are freed
    for (int i = 0; i < NUM_ALLOCATIONS; i++)
    {
        if (ptr_array[i])
        {
            free(ptr_array[i]);
            ptr_array[i] = NULL;
        }
    }
    
    // Test 3: Allocate and free blocks of varying sizes
    printf("3. Allocating and freeing blocks of varying sizes...\n");
    for (int i = 0; i < NUM_ALLOCATIONS; i++)
    {
        // Random size between 1 and MAX_ALLOC_SIZE bytes
        size_array[i] = rand() % MAX_ALLOC_SIZE + 1;
        ptr_array[i] = malloc(size_array[i]);
        
        if (!ptr_array[i])
        {
            printf("Failed to allocate block %d\n", i);
            break;
        }
        
        // Fill memory with a pattern
        memset(ptr_array[i], i % 256, size_array[i]);
        
        // Free some blocks as we go (every 3rd block)
        if (i % 3 == 0 && i > 0)
        {
            free(ptr_array[i-1]);
            ptr_array[i-1] = NULL;
        }
    }
    
    // Ensure all blocks are freed
    for (int i = 0; i < NUM_ALLOCATIONS; i++)
    {
        if (ptr_array[i])
        {
            free(ptr_array[i]);
            ptr_array[i] = NULL;
        }
    }
    
    printf("Malloc/free pattern tests completed.\n");
}

/*
 * Test realloc with various patterns
 */
void test_realloc_patterns(void)
{
    printf("\n=== Test: Various realloc patterns ===\n");
    
    // Test 1: Simple realloc growth
    printf("1. Testing realloc growth...\n");
    char *str = (char *)malloc(15);
    if (!str)
    {
        printf("Failed to allocate initial memory\n");
        return;
    }
    
    strcpy(str, "Hello, World!");
    printf("Initial string: %s\n", str);
    
    // Grow the allocation
    str = (char *)realloc(str, 30);
    if (!str)
    {
        printf("Failed to reallocate memory\n");
        return;
    }
    
    strcat(str, " Expanded!");
    printf("After realloc growth: %s\n", str);
    
    // Free the memory
    free(str);
    
    // Test 2: Realloc shrink
    printf("2. Testing realloc shrink...\n");
    int *numbers = (int *)malloc(sizeof(int) * 100);
    if (!numbers)
    {
        printf("Failed to allocate initial memory\n");
        return;
    }
    
    for (int i = 0; i < 100; i++)
        numbers[i] = i;
    
    printf("Initial array size: 100 integers\n");
    
    // Shrink the allocation
    numbers = (int *)realloc(numbers, sizeof(int) * 50);
    if (!numbers)
    {
        printf("Failed to reallocate memory\n");
        return;
    }
    
    printf("After realloc shrink: 50 integers\n");
    printf("First 5 elements: %d %d %d %d %d\n", 
           numbers[0], numbers[1], numbers[2], numbers[3], numbers[4]);
    
    // Free the memory
    free(numbers);
    
    // Test 3: Multiple reallocs
    printf("3. Testing multiple reallocs...\n");
    void *ptr = malloc(100);
    if (!ptr)
    {
        printf("Failed to allocate initial memory\n");
        return;
    }
    
    printf("Initial allocation: 100 bytes\n");
    
    // Perform multiple reallocs
    for (int i = 1; i <= 10; i++)
    {
        int new_size = 100 * i;
        ptr = realloc(ptr, new_size);
        
        if (!ptr)
        {
            printf("Failed to reallocate memory at iteration %d\n", i);
            return;
        }
        
        memset(ptr, i, new_size);
        printf("Realloc %d: %d bytes\n", i, new_size);
    }
    
    // Free the memory
    free(ptr);
    
    printf("Realloc pattern tests completed.\n");
}

/*
 * Test thread worker function
 */
void *thread_worker(void *arg)
{
    int thread_id = *(int *)arg;
    void *ptrs[ITERATIONS_PER_THREAD];
    
    printf("Thread %d started\n", thread_id);
    
    for (int i = 0; i < ITERATIONS_PER_THREAD; i++)
    {
        // Allocate memory of random size
        int size = rand() % 1000 + 1;
        ptrs[i] = malloc(size);
        
        if (!ptrs[i])
        {
            printf("Thread %d: Failed to allocate memory at iteration %d\n", thread_id, i);
            continue;
        }
        
        // Use the memory
        memset(ptrs[i], thread_id, size);
        
        // Random operations
        int op = rand() % 3;
        if (op == 0 && i > 0)
        {
            // Free a random previous allocation
            int idx = rand() % i;
            if (ptrs[idx])
            {
                free(ptrs[idx]);
                ptrs[idx] = NULL;
            }
        }
        else if (op == 1 && i > 0)
        {
            // Realloc a random previous allocation
            int idx = rand() % i;
            if (ptrs[idx])
            {
                int new_size = rand() % 1000 + 1;
                ptrs[idx] = realloc(ptrs[idx], new_size);
                
                if (ptrs[idx])
                    memset(ptrs[idx], thread_id, new_size);
            }
        }
        
        // Simulate some work
        usleep(1000);  // 1ms delay
    }
    
    // Free all remaining allocations
    for (int i = 0; i < ITERATIONS_PER_THREAD; i++)
    {
        if (ptrs[i])
        {
            free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }
    
    printf("Thread %d completed\n", thread_id);
    return NULL;
}

/*
 * Test concurrent allocations in multiple threads
 */
void test_multithreaded_allocations(void)
{
    printf("\n=== Test: Multithreaded allocations ===\n");
    
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    // Reset random seed
    srand(time(NULL));
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_worker, &thread_ids[i]) != 0)
        {
            printf("Failed to create thread %d\n", i);
            return;
        }
    }
    
    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    printf("Multithreaded allocation test completed.\n");
}

/*
 * Test edge cases
 */
void test_edge_cases(void)
{
    printf("\n=== Test: Edge cases ===\n");
    
    // Test 1: Zero size malloc
    printf("1. Testing malloc(0)...\n");
    void *ptr = malloc(0);
    printf("malloc(0) returned: %p\n", ptr);
    free(ptr);
    
    // Test 2: Free NULL pointer
    printf("2. Testing free(NULL)...\n");
    free(NULL);
    printf("free(NULL) completed without errors\n");
    
    // Test 3: Realloc with NULL pointer
    printf("3. Testing realloc(NULL, size)...\n");
    ptr = realloc(NULL, 100);
    printf("realloc(NULL, 100) returned: %p\n", ptr);
    if (ptr)
        free(ptr);
    
    // Test 4: Realloc with size 0
    printf("4. Testing realloc(ptr, 0)...\n");
    ptr = malloc(100);
    if (ptr)
    {
        void *new_ptr = realloc(ptr, 0);
        printf("realloc(ptr, 0) returned: %p\n", new_ptr);
        // Note: No need to free as realloc with size 0 should free the original pointer
    }
    
    // Test 5: Very large allocation
    printf("5. Testing very large allocation...\n");
    size_t large_size = 100 * 1024 * 1024; // 100 MB
    ptr = malloc(large_size);
    printf("malloc(%zu) returned: %p\n", large_size, ptr);
    if (ptr)
    {
        // Test if we can write to the memory
        memset(ptr, 0, 1024); // Just test a small part
        printf("Successfully wrote to allocated memory\n");
        free(ptr);
    }
    
    printf("Edge case tests completed.\n");
}

/*
 * Main function
 */
int main(void)
{
    printf("Starting comprehensive malloc implementation tests...\n");
    
    // Run the tests
    test_malloc_free_patterns();
    test_realloc_patterns();
    test_multithreaded_allocations();
    test_edge_cases();
    
    // Show final memory state
    printf("\n=== Final memory state ===\n");
    show_alloc_mem();
    
    printf("\nDetailed memory report:\n");
    show_alloc_mem_ex();
    
    printf("\nAll tests completed successfully.\n");
    return 0;
}