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
        printf("\nWrong size Sudoku input! (Too tall/short)\n");
        return 22;
    }    

    Board solvedBoard;

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
                    solvedBoard.Data[(i - 1) * BOARDSIZE + x] = 0;
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
                    solvedBoard.Data[(i - 1) * BOARDSIZE + x] = (uint8_t)argv[i][x] - 48u;
                    break;
                default:
                    //Invalid arg
                    printf("\nInvalid board character!\n");
                    return 22;
            }
        }
    }

    printf("\nStarting Board:\n");
    PrintBoard(&solvedBoard);

    clock_t begin = clock();
    bool solved = Solve(&solvedBoard);
    clock_t end = clock();
    double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

    if (solved)
    {
        printf("\nSolved Board in %lfs:\n", timeSpent);
        PrintBoard(&solvedBoard);
    }
    else printf("\nCould not solve!\n");

    return 0;
}