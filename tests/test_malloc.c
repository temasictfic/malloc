#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "../includes/malloc.h"

/*
 * Test 1: Basic malloc and free
 */
void test_basic_malloc(void)
{
    printf("\n=== Test 1: Basic malloc and free ===\n");
    
    // Allocate and use memory
    char *str = (char *)malloc(20);
    if (!str)
    {
        printf("Failed to allocate memory\n");
        return;
    }
    
    strcpy(str, "Hello, world!");
    printf("Allocated string: %s\n", str);
    
    // Free memory
    free(str);
    printf("Memory freed successfully\n");
}

/*
 * Test 2: Multiple allocations of different sizes
 */
void test_multiple_allocations(void)
{
    printf("\n=== Test 2: Multiple allocations ===\n");
    
    int *tiny = (int *)malloc(sizeof(int) * 10);  // Tiny allocation
    if (!tiny)
    {
        printf("Failed to allocate tiny memory\n");
        return;
    }
    
    char *small = (char *)malloc(500);  // Small allocation
    if (!small)
    {
        printf("Failed to allocate small memory\n");
        free(tiny);
        return;
    }
    
    double *large = (double *)malloc(2000);  // Large allocation
    if (!large)
    {
        printf("Failed to allocate large memory\n");
        free(tiny);
        free(small);
        return;
    }
    
    // Initialize memory
    for (int i = 0; i < 10; i++)
        tiny[i] = i;
    
    memset(small, 'A', 499);
    small[499] = '\0';
    
    for (int i = 0; i < 250; i++)
        large[i] = i * 1.5;
    
    // Verify memory contents
    printf("Tiny allocation (first 5 elements): ");
    for (int i = 0; i < 5; i++)
        printf("%d ", tiny[i]);
    printf("\n");
    
    printf("Small allocation (first 10 chars): %.10s...\n", small);
    
    printf("Large allocation (first 5 elements): ");
    for (int i = 0; i < 5; i++)
        printf("%.1f ", large[i]);
    printf("\n");
    
    // Show memory allocation
    printf("Memory allocation before free:\n");
    show_alloc_mem();
    
    // Free memory
    free(tiny);
    free(small);
    free(large);
    
    printf("Memory allocation after free:\n");
    show_alloc_mem();
}

/*
 * Test 3: Realloc functionality
 */
void test_realloc(void)
{
    printf("\n=== Test 3: Realloc functionality ===\n");
    
    // Allocate memory
    int *numbers = (int *)malloc(sizeof(int) * 5);
    if (!numbers)
    {
        printf("Failed to allocate memory\n");
        return;
    }
    
    // Initialize memory
    for (int i = 0; i < 5; i++)
        numbers[i] = i * 10;
    
    printf("Original allocation (5 elements): ");
    for (int i = 0; i < 5; i++)
        printf("%d ", numbers[i]);
    printf("\n");
    
    // Reallocate memory to a larger size
    numbers = (int *)realloc(numbers, sizeof(int) * 10);
    if (!numbers)
    {
        printf("Failed to reallocate memory\n");
        return;
    }
    
    // Original data should be preserved
    printf("After realloc (original 5 elements): ");
    for (int i = 0; i < 5; i++)
        printf("%d ", numbers[i]);
    printf("\n");
    
    // Add more data
    for (int i = 5; i < 10; i++)
        numbers[i] = i * 10;
    
    printf("After realloc (all 10 elements): ");
    for (int i = 0; i < 10; i++)
        printf("%d ", numbers[i]);
    printf("\n");
    
    // Reallocate to smaller size
    numbers = (int *)realloc(numbers, sizeof(int) * 3);
    if (!numbers)
    {
        printf("Failed to reallocate memory\n");
        return;
    }
    
    printf("After shrinking (first 3 elements): ");
    for (int i = 0; i < 3; i++)
        printf("%d ", numbers[i]);
    printf("\n");
    
    // Free memory
    free(numbers);
}

/*
 * Test 4: Thread safety
 */
#define NUM_THREADS 4
#define ALLOCS_PER_THREAD 100

void *thread_alloc_free(void *arg)
{
    int thread_id = *(int *)arg;
    void *ptrs[ALLOCS_PER_THREAD];
    
    // Allocate memory
    for (int i = 0; i < ALLOCS_PER_THREAD; i++)
    {
        // Various sizes to hit different zone types
        size_t size = (i % 3 == 0) ? 64 : (i % 3 == 1) ? 512 : 2048;
        ptrs[i] = malloc(size);
        if (!ptrs[i])
        {
            printf("Thread %d: Failed to allocate memory\n", thread_id);
            continue;
        }
        
        // Write to memory to ensure it's usable
        memset(ptrs[i], thread_id, size);
    }
    
    // Free memory in reverse order
    for (int i = ALLOCS_PER_THREAD - 1; i >= 0; i--)
    {
        if (ptrs[i])
            free(ptrs[i]);
    }
    
    printf("Thread %d completed\n", thread_id);
    return NULL;
}

void test_thread_safety(void)
{
    printf("\n=== Test 4: Thread safety ===\n");
    
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_alloc_free, &thread_ids[i]) != 0)
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
    
    // Show final memory state
    printf("Final memory state after all threads:\n");
    show_alloc_mem();
}

/*
 * Test 5: Bonus functions
 */
void test_bonus_functions(void)
{
    printf("\n=== Test 5: Bonus functions ===\n");
    
    // Allocate some memory to view
    int *ptr1 = (int *)malloc(sizeof(int) * 10);
    char *ptr2 = (char *)malloc(100);
    
    if (!ptr1 || !ptr2)
    {
        printf("Failed to allocate memory for bonus test\n");
        return;
    }
    
    // Initialize memory
    for (int i = 0; i < 10; i++)
        ptr1[i] = i * 100;
    
    snprintf(ptr2, 100, "This is a test string for hexdump demonstration");
    
    // Show detailed memory allocation
    printf("\nDetailed memory allocation:\n");
    show_alloc_mem_ex();
    
    // Show hexdump of allocated memory
    printf("\nHexdump of ptr1:\n");
    hexdump(ptr1, sizeof(int) * 10);
    
    printf("\nHexdump of ptr2:\n");
    hexdump(ptr2, 50);
    
    // Free memory
    free(ptr1);
    free(ptr2);
}

/*
 * Main test function
 */
int main(void)
{
    printf("Starting malloc implementation tests...\n");
    
    test_basic_malloc();
    test_multiple_allocations();
    test_realloc();
    test_thread_safety();
    test_bonus_functions();
    
    printf("\nAll tests completed.\n");
    return 0;
}