#!/bin/bash

# Function to check if stderr is empty
#!/bin/bash

is_stderr_empty() {
    # Execute the command within a timeout of 25 seconds
    timeout 30s "$@">/dev/null 2>/tmp/stderr_initial

    # Check if the command exceeded the time limit
    if [ $? -eq 124 ]; then
        echo "⌛ Command took longer than 30 seconds (Probably because you didn't create graph30.bin or downloaded graphWHOLEBDD.bin)"
        return 1 # Command took longer than 25 seconds
    elif [ -s /tmp/stderr_initial ]; then
        return 1 # stderr is not empty
    else
        return 0 # stderr is empty and within the time limit
    fi

    # Clean up the temporary file
    rm -f /tmp/stderr_initial
}

echo "🧪 Running tests..."

# Test 1
if is_stderr_empty './main' '-r' '132,18,231,175,93' '-t' '-a' '2'; then
    echo "✅ Test 1/8 passed"
else
    echo "❌ Test 1/8 failed"
fi

# Test 2
if is_stderr_empty './main' '-r' '132,18,231,175,93' '-n' '25'; then
    echo "✅ Test 2/8 passed"
else
    echo "Test 2/8 failed"
fi

# Test 3
if is_stderr_empty './main' '-r' '132,18,231,175,93' '-f' 'tests.txt'; then
    echo "✅ Test 3/8 passed"
    rm -f tests.txt
else
    echo "❌ Test 3/8 failed"
fi

# Test 4
if is_stderr_empty './main' '-r' '132,18,231,175,93' '-z' '2' '-l' '2000'; then
    echo "✅ Test 4/8 passed"
else
    echo "❌ Test 4/8 failed"
fi

# Test 5
if is_stderr_empty './main' '-r' '132,18,231,175,93' '-z' '2' '-c' '513,846,123,45'; then
    echo "✅ Test 5/8 passed"
else
    echo "❌ Test 5/8 failed"
fi

# Test 6
if is_stderr_empty './main' '-r' '132,18,231,175,93' '-z' '2' '-e' '20'; then
    echo "✅ Test 6/8 passed"
else
    echo "❌ Test 6/8 failed"
fi

# Test 7
if is_stderr_empty './main' '-r' '132,18,231,175,93' '-z' '2' '-b' '132,18,231,175,93'; then
    echo "✅ Test 7/8 passed"
else
    echo "❌ Test 7/8 failed"
fi
# Test 8
if is_stderr_empty './main' '-r' '132,18,231,175,93' '-z' '2' '-n' '25' '-f' 'tests.txt' '-l' '2000' '-c' '513,846,123,45' '-e' '20' '-b' '132,18,231,175,93' '-t' '-a' '2'; then
    echo "✅ Test 8/8 passed"
    rm -f tests.txt
else
    echo "❌ Test 8/8 failed"
fi

echo "🧪 Tests completed"