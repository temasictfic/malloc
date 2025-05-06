#!/bin/bash
# This script runs a program with our custom malloc implementation

# Check if we have a program to run
if [ $# -lt 1 ]; then
    echo "Usage: $0 program [arguments]"
    echo "Example: $0 ls -la"
    exit 1
fi

# Get the current directory
CURRENT_DIR=$(pwd)

# Set the LD_PRELOAD environment variable to use our malloc
export LD_PRELOAD="$CURRENT_DIR/libft_malloc.so"

# Run the program with all arguments
echo "Running $@ with custom malloc implementation..."
"$@"

# Reset LD_PRELOAD
unset LD_PRELOAD