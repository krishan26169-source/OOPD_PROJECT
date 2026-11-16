#!/bin/bash
# Project: OOPD_PROJECT
# Submitted by: KRISHAN KANT KUMAR
# Roll Number: MT26169
# Group Number: 119

cd "$(dirname "$0")" || exit 1
trap 'rm -f acaderp' EXIT

echo "======================================"
echo "       OOPD_PROJECT Test Suite"
echo "======================================"

echo
echo "Compiling project..."

g++ -std=c++11 -Wall -Wextra -fno-exceptions \
    src/main.cpp \
    src/ERP.cpp \
    src/Course.cpp \
    src/Student.cpp \
    src/Registration.cpp \
    src/Utilities.cpp \
    -o acaderp

if [ $? -ne 0 ]; then
    echo "COMPILATION FAILED"
    exit 1
fi

echo "Compilation successful."
echo

passed=0
failed=0
total=0

for input_file in tests/T*.in
do
    [ -f "$input_file" ] || continue

    test_name=$(basename "$input_file" .in)
    expected_file="tests/${test_name}.out"
    actual_file="/tmp/${test_name}.actual"

    total=$((total + 1))

    if [ ! -f "$expected_file" ]; then
        echo "$test_name : MISSING EXPECTED OUTPUT"
        failed=$((failed + 1))
        continue
    fi

    ./acaderp < "$input_file" > "$actual_file"

    if diff -u "$expected_file" "$actual_file" > /dev/null; then
        echo "$test_name : PASS"
        passed=$((passed + 1))
    else
        echo "$test_name : FAIL"
        echo "Expected:"
        cat "$expected_file"
        echo "Actual:"
        cat "$actual_file"
        echo
        failed=$((failed + 1))
    fi
done

echo
echo "======================================"
echo "Total tests : $total"
echo "Passed      : $passed"
echo "Failed      : $failed"
echo "======================================"

if [ "$failed" -ne 0 ]; then
    exit 1
fi

exit 0
