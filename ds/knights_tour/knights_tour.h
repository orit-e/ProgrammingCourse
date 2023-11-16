/******************************************************************************
 				
		File name:              knights_tour.h
		Written by:             Orit			
		Reviewed by:    	Diana
 											
 ******************************************************************************/
#ifndef __KNIGHTS_TOUR_H__
#define __KNIGHTS_TOUR_H__

enum KNIGHT_STATUS
{
        KNIGHT_SUCCESS,
        KNIGHT_FAIL,
        MALLOC_FAIL,
        TIME_OUT
};

int KnightsTourNaive(char starting_square[3]);

int KnightsTour(char starting_square[3]);


#endif /* __KNIGHTS_TOUR_H__ */