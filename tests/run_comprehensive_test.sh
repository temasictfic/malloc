#!/bin/bash

# Go to the project root directory
cd "$(dirname "$0")/.."

# Compile the comprehensive test
echo "Compiling comprehensive test..."
gcc -o comprehensive_test tests/comprehensive_test.c -I. -L. -lft_malloc -pthread

# Check if compilation succeeded
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Set LD_LIBRARY_PATH to include current directory
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

# Run the test
echo "Running comprehensive test..."
./comprehensive_test

# Check if execution succeeded
if [ $? -ne 0 ]; then
    echo "Test execution failed!"
    exit 1
fi

echo "Comprehensive test completed successfully!"

# Clean up
rm -f comprehensive_test