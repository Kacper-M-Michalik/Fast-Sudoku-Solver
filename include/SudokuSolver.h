#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define BOARDSIZE 9
#define TILESIZE 3

typedef struct board
{
    uint8_t Data[BOARDSIZE * BOARDSIZE];
} Board;

typedef struct backtrackResult
{
    size_t x;
    size_t y;
    uint8_t AssumedValue;
    bool Complete;
} BacktrackResult;

bool Solve(Board* solvedBoard);

void PrintBoard(const Board* board);