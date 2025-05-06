#!/bin/bash

# Go to the project root directory
cd "$(dirname "$0")/.."

# Compile the basic test
echo "Compiling basic test..."
gcc -o test_malloc tests/test_malloc.c -I. -L. -lft_malloc

# Check if compilation succeeded
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Set LD_LIBRARY_PATH to include current directory
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

# Run the test
echo "Running basic test..."
./test_malloc

# Check if execution succeeded
if [ $? -ne 0 ]; then
    echo "Test execution failed!"
    exit 1
fi

echo "Basic test completed successfully!"

# Clean up
rm -f test_malloc