#!/bin/bash
set -euo pipefail

# Build fizzbzz
gcc -mavx2 -c fizzbuzz.S
ld -o fizzbuzz fizzbuzz.o


g++ main.cpp -o main



# Run
# ./fizzbuzz | cat | pv | ./main
