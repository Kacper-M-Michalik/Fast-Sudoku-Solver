build:
	gcc -g -O3 -march=native -mavx2 -Wall -Wextra -Wundef -Wswitch-default -Wunreachable-code main.c -o Solver