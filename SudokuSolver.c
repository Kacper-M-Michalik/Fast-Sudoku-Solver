#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "SudokuSolver.h"

int main(int count, char* argv[])
{
    if (count != BOARDSIZE + 1) 
    {
        printf("\nWrong size Sudoku input! (Too tall//short)\n");
        return 22;
    }

    //char* testBoard[BOARDSIZE] = {"8...6....", ".2....9..", "9...17..8", ".5..74.1.", "...2....4", "7..6.....", "2...98..1", "..3....5.", "...4....."};
    //{"47......5", "...5...3.", "5........", "..3.72.68", "...9..34.", ".546.8.97", "7.6..4..1", "..58.....", ".91..5.7."};
    //{"9.2.63.8.", "8..4..9..", ".56.2...4", "..31.7...", "5..3.6.18", ".1.....5.", "..8...5..", ".2.....4.", "3.15.9..."};
    //{"53..7....", "6..195...", ".98....6.", "8...6...3", "4..8.3..1", "7...2...6", ".6....28.", "...419..5", "....8..79"};

    uint8_t solvedBoard[BOARDSIZE * BOARDSIZE];

    for (size_t i = 1; i < (size_t)count; i++)
    {
        if (strlen(argv[i]) != BOARDSIZE)
        {
            printf("\nWrong size Sudoku input! (Too wide/narrow)\n");
            return 22;
        }
        for (size_t x = 0; x < BOARDSIZE; x++)
        {
            switch (argv[i][x])
            {
                case '.':
                    solvedBoard[(i - 1) * BOARDSIZE + x] = 0;
                    break;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':                
                    solvedBoard[(i - 1) * BOARDSIZE + x] = (uint8_t)argv[i][x] - 48u;
                    break;
                default:
                    //Invalid arg
                    printf("\nInvalid board character!\n");
                    return 22;
            }
        }
    }

    printf("\nStarting Board:\n");
    PrintBoard(solvedBoard);

    clock_t begin = clock();
    bool Solved = Solve(solvedBoard);
    clock_t end = clock();
    double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

    if (Solved)
    {
        printf("\nSolved Board in %lfs:\n", timeSpent);
        PrintBoard(solvedBoard);
    }
    else printf("\nCould not solve!\n");

    return 0;
}