/******************************************************************************
 				
				File name:			knights_tour.c
				Written by:     	Orit			
				Reviewed by:    	Diana
 											
 ******************************************************************************/
#include <assert.h> /*  assert()            */
#include <time.h>   /*  time()              */
#include <stdio.h>  /*  printf(), size_t    */

#include "knights_tour.h"
#include "stack.h" 

#define BITS_IN_WORDSIZE 64
#define BYTES_IN_WORDSIZE 8
#define BITS_IN_BYTE 8
#define NEXT_MOVES 8
#define NOT_ALLOWED 9

enums MOVE_DEGREE
{
    MOVE,
    DEGREE,
    MOVE_AND_DEGREE
};

static void InitLUT(size_t moves_LUT[BITS_IN_WORDSIZE][NEXT_MOVES]);
static size_t FindNextMove(int row, int column, int move_row, int move_column);
static int FindKnightsPathNaive(size_t chess_board, size_t current_square,
                            size_t moves_LUT[BITS_IN_WORDSIZE][NEXT_MOVES], 
                            stack_t *stack, time_t start_time);
static int FindKnightsPath(size_t chess_board, size_t current_square,
                            size_t moves_LUT[BITS_IN_WORDSIZE][NEXT_MOVES], 
                            stack_t *stack, time_t start_time);
static int CountBitsOn(size_t n);
static size_t ConvertToBitArray(char *starting_square);
static void ConvertToStr(size_t bit_array_move, char *square);
static void RateDegrees(size_t chess_board, size_t current_square, 
                        size_t moves_LUT[BITS_IN_WORDSIZE][NEXT_MOVES], 
                        size_t next_move_degrees[NEXT_MOVES][MOVE_AND_DEGREE]);
static void SortDegrees(size_t next_move_degrees[NEXT_MOVES][MOVE_AND_DEGREE]);
static int GetDegree(size_t chess_board, size_t square, 
                        size_t moves_LUT[BITS_IN_WORDSIZE][NEXT_MOVES]);


int KnightsTourNaive(char starting_square[3])
{
    stack_t *stack = NULL;
    size_t moves_LUT[BITS_IN_WORDSIZE][NEXT_MOVES] = {0};
    size_t starting_point = 0;
    int status = KNIGHT_SUCCESS;
    char square[3] = {0, 0, '\0'};
    
    starting_point = ConvertToBitArray(starting_square);
    InitLUT(moves_LUT);
    
    stack = StackCreate(BITS_IN_WORDSIZE);
    if(NULL == stack)
    {
        return MALLOC_FAIL;
    }

    status = FindKnightsPathNaive(starting_point, starting_point, moves_LUT, 
                                stack, time(NULL));
    
    square[0] = starting_square[0];
    square[1] = starting_square[1];
    while(1 != StackIsEmpty(stack))
    {
        printf("%s moves to ", square);
        ConvertToStr((size_t)StackPeek(stack), square);
        StackPop(stack);
        printf("%s.\n", square);
    }
        
    StackDestroy(stack);
    stack = NULL;

    return status;
}

int KnightsTour(char starting_square[3])
{
    stack_t *stack = NULL;
    size_t moves_LUT[BITS_IN_WORDSIZE][NEXT_MOVES] = {0};
    size_t starting_point = 0;
    int status = KNIGHT_SUCCESS;
    char square[3] = {0, 0, '\0'};

    starting_point = ConvertToBitArray(starting_square);
    InitLUT(moves_LUT);
    
    stack = StackCreate(BITS_IN_WORDSIZE);
    if(NULL == stack)
    {
        return MALLOC_FAIL;
    }

    status = FindKnightsPath(starting_point, starting_point, moves_LUT, stack, 
                            time(NULL));
    
    square[0] = starting_square[0];
    square[1] = starting_square[1];
    while(1 != StackIsEmpty(stack))
    {
        printf("%s moves to ", square);
        ConvertToStr((size_t)StackPeek(stack), square);
        StackPop(stack);
        printf("%s.\n", square);
    }

    StackDestroy(stack);
    stack = NULL;

    return status;
}

static size_t FindNextMove(int row, int column, int move_row, int move_column)
{
    if((0 <= (column + move_column)) && (0 <= (row + move_row)) && 
            (BITS_IN_BYTE > (column + move_column)) && 
            (BYTES_IN_WORDSIZE > (row + move_row)))
    {
        size_t new_row = (size_t)(row + move_row);
        size_t new_column = (size_t)(column + move_column);

        return (1lu << (new_row * BITS_IN_BYTE + new_column));
    }

    return 0;
}

static void InitLUT(size_t moves_LUT[BITS_IN_WORDSIZE][NEXT_MOVES])
{
    int i = 0;
    
    assert(NULL != moves_LUT);

    for(i = 0; i < BITS_IN_WORDSIZE; ++i)
    {
        int column = i % BITS_IN_BYTE;
        int row = i / BITS_IN_BYTE;

        moves_LUT[i][0] = FindNextMove(row, column, -1, 2);
        moves_LUT[i][1] = FindNextMove(row, column, -2, -1);
        moves_LUT[i][2] = FindNextMove(row, column, 1, -2);
        moves_LUT[i][3] = FindNextMove(row, column, 2, 1);
        moves_LUT[i][4] = FindNextMove(row, column, 1, 2);
        moves_LUT[i][5] = FindNextMove(row, column, 2, -1);
        moves_LUT[i][6] = FindNextMove(row, column, -1, -2);
        moves_LUT[i][7] = FindNextMove(row, column, -2, 1);
    }
}

static int FindKnightsPathNaive(size_t chess_board, size_t current_square,
                                size_t moves_LUT[BITS_IN_WORDSIZE][NEXT_MOVES], 
                                stack_t *stack, time_t start_time)
{
    size_t next_move = 0;
    int index = 0;
    int i = 0;
    int status = KNIGHT_FAIL;

    assert(0 != chess_board);
    assert(0 != current_square);
    assert(NULL != stack);

    if(~0lu == chess_board)
    {
        return KNIGHT_SUCCESS;
    }  
    if(300 <= time(NULL) - start_time)
    {
        return TIME_OUT;
    }

    index = CountBitsOn(current_square - 1);
    while(i < NEXT_MOVES && KNIGHT_SUCCESS != status && TIME_OUT != status)
    {
        next_move = moves_LUT[index][i];
        if(0 != next_move && 0 == (next_move & chess_board))
        {
            status = FindKnightsPathNaive((chess_board | next_move), next_move, 
                                        moves_LUT, stack, start_time);
        }
        ++i;
    }

    if(KNIGHT_SUCCESS == status || TIME_OUT == status)
    {
        void *ptr = (void *)next_move;
        StackPush(stack, ptr);
    }

    return status;
}

static int FindKnightsPath(size_t chess_board, size_t current_square,
                            size_t moves_LUT[BITS_IN_WORDSIZE][NEXT_MOVES], 
                            stack_t *stack, time_t start_time)
{
    size_t next_move = 0;
    size_t next_move_degrees[NEXT_MOVES][MOVE_AND_DEGREE] = {0};
    int i = 0;
    int status = KNIGHT_FAIL;

    assert(0 != chess_board);
    assert(0 != current_square);
    assert(NULL != stack);

    if(~0lu == chess_board)
    {
        return KNIGHT_SUCCESS;
    }
    if(300 <= time(NULL) - start_time)
    {
        return TIME_OUT;
    }

    RateDegrees(chess_board, current_square, moves_LUT, next_move_degrees);
    while(i < NEXT_MOVES && KNIGHT_SUCCESS != status && TIME_OUT != status)
    {
        next_move = next_move_degrees[i][MOVE];
        if(NOT_ALLOWED != next_move_degrees[i][DEGREE])
        {
            status = FindKnightsPath((chess_board | next_move), next_move, 
                                        moves_LUT, stack, start_time);
        }
        ++i;
    }
    if(KNIGHT_SUCCESS == status || TIME_OUT == status)
    {
        void *ptr = (void *)next_move;
        StackPush(stack, ptr);
    }

    return status;
}

static int CountBitsOn(size_t n)
{
    n = (n & 0x5555555555555555) + ((n >> 1) & 0x5555555555555555);
    n = (n & 0x3333333333333333) + ((n >> 2) & 0x3333333333333333);
    n = (n & 0x0F0F0F0F0F0F0F0F) + ((n >> 4) & 0x0F0F0F0F0F0F0F0F);

    return ((n & 0xFF) + ((n >> BITS_IN_BYTE) & 0xFF) + 
            ((n >> BITS_IN_BYTE * 2) & 0xFF) + 
            ((n >> BITS_IN_BYTE * 3) & 0xFF) + 
            ((n >> BITS_IN_BYTE * 4) & 0xFF) + 
            ((n >> BITS_IN_BYTE * 5) & 0xFF) + 
            ((n >> BITS_IN_BYTE * 6) & 0xFF) + 
            ((n >> BITS_IN_BYTE * 7) & 0xFF));
}

static size_t ConvertToBitArray(char *starting_square)
{
    size_t bit_array = 1;

    assert(NULL != starting_square);

    bit_array <<= ((starting_square[0] - '1') * BITS_IN_BYTE);
    bit_array <<= ((starting_square[1] - 'A'));
    
    return bit_array;
}

static void ConvertToStr(size_t bit_array_move, char *square)
{
    int index = 0;

    assert(NULL != square);

    index = CountBitsOn(bit_array_move - 1);
    square[0] = index / BITS_IN_BYTE + '1';
    square[1] = index % BITS_IN_BYTE + 'A';
}

static void RateDegrees(size_t chess_board, size_t current_square, 
                        size_t moves_LUT[BITS_IN_WORDSIZE][NEXT_MOVES], 
                        size_t next_move_degrees[NEXT_MOVES][MOVE_AND_DEGREE])
{
    int index = CountBitsOn(current_square - 1);
    int i = 0;

    for(i = 0; i < NEXT_MOVES; ++i)
    {
        next_move_degrees[i][MOVE] = moves_LUT[index][i];
        next_move_degrees[i][DEGREE] = 
                        GetDegree(chess_board, moves_LUT[index][i], moves_LUT);
    }
    SortDegrees(next_move_degrees);
}

static void SortDegrees(size_t next_move_degrees[NEXT_MOVES][MOVE_AND_DEGREE])
{
    size_t i = 0;
    size_t counter[10] = {0};
    size_t output[NEXT_MOVES][MOVE_AND_DEGREE] = {0};

    assert(NULL != next_move_degrees);

    for(i = 0; i < NEXT_MOVES; ++i)
    {
        counter[next_move_degrees[i][DEGREE]] += 1;
    }
    for(i = 1; i < 10; ++i)
    {
        counter[i] += counter[i - 1];
    }
    for(i = 1; i <= NEXT_MOVES; ++i)
    {
        output[counter[next_move_degrees[NEXT_MOVES - i][1]] - 1][DEGREE] = 
                                    next_move_degrees[NEXT_MOVES - i][DEGREE];
        output[counter[next_move_degrees[NEXT_MOVES - i][1]] - 1][MOVE] = 
                                    next_move_degrees[NEXT_MOVES - i][MOVE];
        counter[next_move_degrees[NEXT_MOVES - i][DEGREE]] -= 1;
    }
    for(i = 0; i < NEXT_MOVES; ++i)
    {
        next_move_degrees[i][MOVE] = output[i][MOVE];
        next_move_degrees[i][DEGREE] = output[i][DEGREE];
    }
}

static int GetDegree(size_t chess_board, size_t square, 
                        size_t moves_LUT[BITS_IN_WORDSIZE][NEXT_MOVES])
{
    int index = 0;
    int counter = 0;
    int i = 0;

    if(0 == square)
    {
        return NOT_ALLOWED;
    }
    if(0 != (square & chess_board))
    {
        return NOT_ALLOWED;
    }
    index = CountBitsOn(square - 1);
    for(i = 0; i < NEXT_MOVES; ++i)
    {
        counter += (0 != moves_LUT[index][i] && 
                    0 == (chess_board & moves_LUT[index][i]));
    }

    return counter;
}
