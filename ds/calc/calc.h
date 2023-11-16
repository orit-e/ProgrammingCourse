/******************************************************************************
	
 						File name: 		calc.h		
 						Written by: 	Orit		
 						Reviewed by: 	Yona		
 						Last edit: 		15.09.2020	
 								
 ******************************************************************************/
#ifndef __CALC_H__
#define __CALC_H__

typedef enum calc_status
{
	CALC_SUCCESS, 
	CALC_MALLOC_ERROR,
	CALC_SYNTAX_ERROR,
	DIVIDE_BY_ZERO_ERROR,
	PARAN_IMBALANCE
}calc_status_ty;

/** 
 * @DESCRIPTION:
 * Calculate() function calculates the math expression written in the string 
 * pointed to by math_exp and places the answer in result.
 * The function can perform the following operations:
 * * addition.
 * * substration.
 * * multiplication.
 * * division.
 * * power (see Warnings section for further detail).
 * The function accepts round parentheses ('(' and ')') only.
 * The function allows single spacing between two expressions (number, operator
 * or parentheses) or no spacing at all. Anything else will be considered as an
 * error by Calculate() (see Errors section for further detail).
 * The function's return value represents whether the calculation was 
 * successful or if an error occured durring the run (see Errors and Warnings 
 * section for further details).
 * The function uses allocation fucntions for its inner workings.
 * The function performs with time complexity of O(nlogn) where n represents 
 * the number of characters in the string pointed to by math_exp.
 * The function performs with space complexity of O(n) where n represents 
 * the number of characters in the string pointed to by math_exp.
 * 
 * @Parameters:
 * math_exp			pointer to the mathematical equation in string format.
 * result			pointer to the location where the function will insert the 
 * 					result of the calculation.
 * 
 * @Return value:
 * Calculate() function returns the status of the function as defined in enum 
 * calc_status above. The function will return CALC_SUCCESS for successfully
 * calculating math_exp and an appropriate error value otherwise (see Errors 
 * section for further detail).
 *
 * @Errors:
 * Calculate() can fail with the following errors:
 * * MALLOC_ERROR -			malloc() related error has occured durring the 
 * 							execution.
 * * SYNTAX_ERROR - 		Indicates that there is some syntax error in 
 * 							math_exp string.
 * * DIVIDE_BY_ZERO_ERROR - math_exp has tried to divide by zero.
 * * PARAN_IMBALANCE - 		The number of opened parentheses is not equal to 
 * 							the number of closed parentheses in math_exp.
 *
 * @Warnings:
 * * Do not place NULL as input of math_exp or result. Such a thing will lead 
 *   to undefined behavior.
 * * Calculate() function can perform the operations specified in the 
 *   Description section only. Any sign implying other operations will result 
 *   in SYNTAX_ERROR.
 * * Calculate() function is able to recognize round parentheses only ('(' and 
 *   ')'). Any other type of parentheses as input will be interpreted by the 
 *   function as SYNTAX_ERROR.
 * * The function allows single spacing between two expressions (number, 
 *   operator or parentheses) or no spacing at all. Anything else will be 
 *   interpreted by the function as SYNTAX_ERROR ('[', ']', '{', '}' are not 
 *   acceptable as input in math_exp).
 * * If the user wishes to place a unary ('+' or '-' as the sign of a number) 
 *   after closed parentheses, the number should be placed inside parentheses. 
 *   Otherwise it will be considered as an operator followed by a number.
 * * Calculate() function uses pow() in its power function. For more 
 *   information about pow() return values and errors, see pow() man page.
 * * The function uses malloc() and free() fucntions for its inner workings.
 *   MALLOC_ERROR status will be returned to the user if malloc() fails while 
 *   the function is running.
 **/
calc_status_ty Calculate(const char *math_exp, double *result);

#endif /* __CALC_H__ */
