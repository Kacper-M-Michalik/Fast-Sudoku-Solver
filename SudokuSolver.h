#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define BOARDSIZE 9
#define TILESIZE 3

typedef struct backtrackResult
{
    size_t x;
    size_t y;
    uint8_t AssumedValue;
    bool Complete;
} BacktrackResult;

bool Solve(uint8_t* solvedBoard);

BacktrackResult BacktrackGuess(uint8_t* finalSolvedBoard, uint8_t* solvedBoard, uint16_t* sourceConstraintBoard);

bool IntialiseConstraint(uint8_t* solvedBoard, uint16_t* constraintBoard, uint8_t value, size_t xPos, size_t yPos);

void UpdateBoard(uint8_t* solvedBoard, uint16_t* constraintBoard, uint8_t value, size_t xPos, size_t yPos);

uint8_t GetBitInt(uint16_t value);

void PrintBoard(uint8_t* solvedBoard);

void PrintConstraintBoard(uint16_t* constraintBoard);


bool Solve(uint8_t* solvedBoard)
{   
    uint16_t constraintBoard[BOARDSIZE * BOARDSIZE] = {[0 ... BOARDSIZE * BOARDSIZE - 1] = 511}; //Pad?
    
    //Set initial constraints and validate start
    for (size_t y = 0; y < BOARDSIZE; y++)
    {
        for (size_t x = 0; x < BOARDSIZE; x++)
        {
            const uint8_t number = solvedBoard[y * BOARDSIZE + x];
            if (number != 0 && !IntialiseConstraint(solvedBoard, constraintBoard, number, x, y)) return false;
        }
    }  

    //Deterministic solve and guess
    while (true)
    {   
        bool Complete;
        bool FilledValue = true;
        while (FilledValue)
        {
            Complete = true;
            FilledValue = false;
            for (size_t y = 0; y < BOARDSIZE; y++)
            {
                for (size_t x = 0; x < BOARDSIZE; x++)
                {
                    //Check if only one valid possibility exists
                    const uint16_t constraint = constraintBoard[y * BOARDSIZE + x];
                    if (constraint && (constraint & (constraint - 1)) == 0)
                    {
                        FilledValue = true;
                        UpdateBoard(solvedBoard, constraintBoard, GetBitInt(constraint), x, y);
                    }
                    else if (constraint) Complete = false;
                }
            }
        }

        if (Complete) return true;

        //Need to do backtrackable guessing
        BacktrackResult result = BacktrackGuess(solvedBoard, solvedBoard, constraintBoard);    
        if (result.Complete) return true;
        
        //Update constraint with failed guess
        constraintBoard[result.y * BOARDSIZE + result.x] &= ~(1 << (result.AssumedValue - 1));
    }
}

BacktrackResult BacktrackGuess(uint8_t* finalSolvedBoard, uint8_t* sourceSolvedBoard, uint16_t* sourceConstraintBoard)
{
    uint8_t solvedBoard[BOARDSIZE * BOARDSIZE];
    memcpy(solvedBoard, sourceSolvedBoard, sizeof(solvedBoard));
    uint16_t constraintBoard[BOARDSIZE * BOARDSIZE];
    memcpy(constraintBoard, sourceConstraintBoard, sizeof(constraintBoard));

    int32_t lowestBitCount = INT32_MAX;
    size_t lowX;
    size_t lowY;

    //Find lowest candidate-count cell -> maximises probabilty we guess correctly
    for (size_t y = 0; y < BOARDSIZE; y++)
    {
        for (size_t x = 0; x < BOARDSIZE; x++)
        {
            int bitCount = __builtin_popcount(constraintBoard[y * BOARDSIZE + x]);
            if (bitCount && bitCount < lowestBitCount)
            {
                lowestBitCount = bitCount;
                lowX = x;
                lowY = y;
            }
            //Add test to skip to end if bitCount == 2?
        }
    }

    //Get lowest bit, convert to number, and guess it
    uint8_t guessValue = GetBitInt(((int16_t)constraintBoard[lowY * BOARDSIZE + lowX]) & -((int16_t)constraintBoard[lowY * BOARDSIZE + lowX]));
    UpdateBoard(solvedBoard, constraintBoard, guessValue, lowX, lowY);

    //Deterministic solve attempt then recursive guess
    while (true)
    {
        bool Complete;
        bool FilledValue = true;
        while (FilledValue)
        {
            Complete = true;
            FilledValue = false;
            for (size_t y = 0; y < BOARDSIZE; y++)
            {
                for (size_t x = 0; x < BOARDSIZE; x++)
                {
                    const uint16_t constraint = constraintBoard[y * BOARDSIZE + x];
                    
                    if (constraint == 0 && solvedBoard[y * BOARDSIZE + x] == 0) 
                    {
                        BacktrackResult result = {lowX, lowY, guessValue, false};
                        return result;
                    }                 
                    if (constraint && (constraint & (constraint - 1)) == 0)
                    {
                        FilledValue = true;
                        UpdateBoard(solvedBoard, constraintBoard, GetBitInt(constraint), x, y);
                    }
                    else if (constraint) Complete = false;
                }
            }
        }

        if (Complete)
        {
            memcpy(finalSolvedBoard, solvedBoard, sizeof(solvedBoard));
            BacktrackResult result = {lowX, lowY, guessValue, true};
            return result;
        }
        
        //Need to guess again
        BacktrackResult result = BacktrackGuess(finalSolvedBoard, solvedBoard, constraintBoard);    
        if (result.Complete)
        {
            BacktrackResult result = {lowX, lowY, guessValue, true};
            return result;
        }
        
        //Update constraint with failed guess
        constraintBoard[result.y * BOARDSIZE + result.x] &= ~(1 << (result.AssumedValue - 1));    
    }
}

bool IntialiseConstraint(uint8_t* solvedBoard, uint16_t* constraintBoard, uint8_t value, size_t xPos, size_t yPos)
{
    const size_t yOff = yPos * BOARDSIZE;  
    const size_t placementIndex = yPos * BOARDSIZE + xPos;  

    constraintBoard[yOff + xPos] = 0;

    for (size_t i = yOff; i < yOff + BOARDSIZE; i++)
    {        
        constraintBoard[i] &= ~(1 << (value - 1));       
        if (i != placementIndex && solvedBoard[i] == value) return false;
    }    

    for (size_t i = xPos; i < BOARDSIZE * BOARDSIZE; i += BOARDSIZE)
    {        
        constraintBoard[i] &= ~(1 << (value - 1));
        if (i != placementIndex && solvedBoard[i] == value) return false;
    }       

    //Change to switch or else-chain instead?
    size_t tileOffX = 0;
    if (xPos >= 3) tileOffX = TILESIZE;
    if (xPos >= 6) tileOffX = 2 * TILESIZE;
    size_t tileOffY = 0;
    if (yPos >= 3) tileOffY = TILESIZE * BOARDSIZE;
    if (yPos >= 6) tileOffY = 2 * TILESIZE * BOARDSIZE;        
    for (size_t y = 0; y < TILESIZE; y++)
    {
        for (size_t x = 0; x < TILESIZE; x++)
        {
            const size_t i = y * BOARDSIZE + x + tileOffX + tileOffY;

            constraintBoard[i] &= ~(1 << (value - 1));
            if (i != placementIndex && solvedBoard[i] == value) return false;
        }
    }   

    return true;
}

void UpdateBoard(uint8_t* solvedBoard, uint16_t* constraintBoard, uint8_t value, size_t xPos, size_t yPos)
{
    const size_t yOff = yPos * BOARDSIZE;   
     
    solvedBoard[yOff + xPos] = value;
    constraintBoard[yOff + xPos] = 0;

    //Loop over row
    for (size_t i = yOff; i < yOff + BOARDSIZE; i++)
    {        
        //Force bit to 0
        constraintBoard[i] &= ~(1 << (value - 1));
    }    

    //Loop over column
    for (size_t i = xPos; i < BOARDSIZE * BOARDSIZE; i += BOARDSIZE)
    {        
        constraintBoard[i] &= ~(1 << (value - 1));
    }       

    //Change to switch or else-chain instead?
    size_t tileOffX = 0;
    if (xPos >= 3) tileOffX = TILESIZE;
    if (xPos >= 6) tileOffX = 2 * TILESIZE;
    size_t tileOffY = 0;
    if (yPos >= 3) tileOffY = TILESIZE * BOARDSIZE;
    if (yPos >= 6) tileOffY = 2 * TILESIZE * BOARDSIZE;        
    //Loop over tile
    for (size_t y = 0; y < TILESIZE; y++)
    {
        for (size_t x = 0; x < TILESIZE; x++)
        {
            constraintBoard[y * BOARDSIZE + x + tileOffX + tileOffY] &= ~(1 << (value - 1));
        }
    }
}

uint8_t GetBitInt(uint16_t value)
{
    switch (value)
    {
        case 1u:
            return 1;
        case 2u:
            return 2;
        case 4u:
            return 3;
        case 8u:
            return 4;
        case 16u:
            return 5;
        case 32u:
            return 6;                
        case 64u:
            return 7;
        case 128u:
            return 8;
        case 256u:
            return 9;    
        default:
            printf("\nGetBitInt GRAVE ERROR: %d\n", value);
            return 0;
    }
}

void PrintBoard(uint8_t* solvedBoard)
{    
    for (size_t y = 0; y < BOARDSIZE * BOARDSIZE; y+= BOARDSIZE)
    {
        for (size_t x = 0; x < BOARDSIZE; x++)
        {
            printf("%d ", solvedBoard[y + x]);
        }
        printf("\n");
    }    
}

void PrintConstraintBoard(uint16_t* constraintBoard)
{
    for (size_t y = 0; y < BOARDSIZE * BOARDSIZE; y+= BOARDSIZE)
    {
        for (size_t x = 0; x < BOARDSIZE; x++)
        {
            for (uint16_t mask = 256, num = 9; mask > 0; mask >>= 1, num--)
            {
                //Print list of possible numbers, (_ if not possible)
                if ((constraintBoard[y + x] & mask) > 0) printf("%d", num);
                else printf("_");
            }
            printf(" ");
        }
        printf("\n");
    }
}