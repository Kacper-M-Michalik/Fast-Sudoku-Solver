# FastSudokuSolver
Header only implementation(s) of a fast sudoku solver.

## To build:
run 'make'

## To use:
Submit sudoku rows as args, valid tile values are 1-9 and '.' to represent an empty tile.

Example input:
'./Solver 8...6.... .2....9.. 9...17..8 .5..74.1. ...2....4 7..6..... 2...98..1 ..3....5. ...4.....'

Example output:
Starting Board:
8 0 0 0 6 0 0 0 0
0 2 0 0 0 0 9 0 0
9 0 0 0 1 7 0 0 8
0 5 0 0 7 4 0 1 0
0 0 0 2 0 0 0 0 4
7 0 0 6 0 0 0 0 0
2 0 0 0 9 8 0 0 1
0 0 3 0 0 0 0 5 0
0 0 0 4 0 0 0 0 0

Solved Board in 0.000009s:
8 7 4 9 6 2 1 3 5
5 2 1 8 4 3 9 6 7
9 3 6 5 1 7 4 2 8
6 5 9 3 7 4 8 1 2
3 1 8 2 5 9 6 7 4
7 4 2 6 8 1 5 9 3
2 6 5 7 9 8 3 4 1
4 8 3 1 2 6 7 5 9
1 9 7 4 3 5 2 8 6