run:
	mkdir -p build
	gcc -g -c -flto -O3 -Wall -Wextra -Wundef -Iinclude src/SudokuSolver.c -o build/solver.o
	gcc -g -c -flto -O3 -Wall -Wextra -Wundef -Iinclude src/main.c -o build/main.o
	gcc -g -flto -O3 -Wall -Wextra -Wundef -o build/Solver build/main.o build/solver.o

optimised:
	mkdir -p build
	gcc -g -c -flto -O3 -mavx2 -march=native -Wall -Wextra -Wundef -Iinclude src/SudokuSolver.c -o build/solver.o
	gcc -g -c -flto -O3 -mavx2 -march=native -Wall -Wextra -Wundef -Iinclude src/main.c -o build/main.o
	gcc -g -flto -O3 -mavx2 -march=native -Wall -Wextra -Wundef -o build/Solver build/main.o build/solver.o