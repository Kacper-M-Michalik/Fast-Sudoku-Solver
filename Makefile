build:
	gcc -g -O3 -msse4.1 -msse4.2 -mavx -mavx2 -Wall -Wextra -Wundef -Wswitch-default -Wunreachable-code main.c -o Solver