/******************************************************************************
 								
 						File name: 		calc.c		
 						Written by: 	Orit		
 						Reviewed by: 	Yona	
 						Last edit: 		15.09.2020	
 								
 ******************************************************************************/
#include <string.h> /* strtod(), strlen()	*/
#include <assert.h>	/* assert()				*/
#include <stdlib.h> /* memset()				*/
#include <math.h>	/* pow()				*/

#include "calc.h"
#include "stack.h"

/******************************************************************************/
/*						  Macros and enum declarations:						  */
#define NUM_OF_STATES 3
#define NUM_OF_EVENTS 256
#define OPERATORS_RANGE 55
#define NULL_TERM 44
#define ASCII_ZERO 48
#define FIRST_OP_ASCII 40

enum STATE
{
	WAIT_FOR_NUM,
	WAIT_FOR_OP,
	POST_CLOSE_PARAN,
	ERROR,
	FINAL_STATE
};

enum OPERATOR_NUM
{
	OPEN_PARAN,
	CLOSE_PARAN,
	MULTIPLY,
	PLUS,
	NULL_TERM_OP,
	MINUS,
	DIVIDE = 7,
	POWER = 54
};

/******************************************************************************/
/*							Typedef declarations:							  */
typedef struct fsm fsm_ty;
typedef struct calc calc_ty;
typedef calc_status_ty (*handler_func_t)(calc_ty *calc_data);
typedef calc_status_ty (*perform_op_t)(stack_t *numbers, stack_t *operators);

/******************************************************************************/
/*						 	Structs declarations:						  	  */
struct fsm
{
	handler_func_t handler;
	enum STATE next_state;
};

struct calc
{
	stack_t *numbers;
	stack_t *operators;
	const char *ptr_math_exp;
	double *result;
	size_t open_paran_num;
	size_t exp_len;
};

/******************************************************************************/
/*						LUTs as static global variables:					  */
static fsm_ty FSM[NUM_OF_STATES][NUM_OF_EVENTS];
static perform_op_t PERFORM_OP[OPERATORS_RANGE][OPERATORS_RANGE];

/******************************************************************************/
/*							  Init functions:							  	  */
static void InitFSM();
static void InitOpLUT();
static calc_status_ty InitCalcData(calc_ty *calc_data, const char *math_exp, 
									double *result);

/******************************************************************************/
/*						  	Handler functions:						  		  */
static calc_status_ty NumHandler(calc_ty *calc_data);
static calc_status_ty OpHandler(calc_ty *calc_data);
static calc_status_ty ErrorHandler(calc_ty *calc_data);
static calc_status_ty OpenParanHandler(calc_ty *calc_data);
static calc_status_ty CloseParanHandler(calc_ty *calc_data);
static calc_status_ty PostCloseParanHandler(calc_ty *calc_data);
static calc_status_ty FinalStateHandler(calc_ty *calc_data);
static calc_status_ty NullAtNumHandler(calc_ty *calc_data);
static calc_status_ty SpaceHandler(calc_ty *calc_data);

/******************************************************************************/
/*				Function for the performance of the operation:				  */
static calc_status_ty OpPerformer(calc_ty *calc_data, int next_op);

/******************************************************************************/
/*						  	Operation functions:						  	  */
static calc_status_ty DoNothing(stack_t *numbers, stack_t *operators);
static calc_status_ty SumFunc(stack_t *numbers, stack_t *operators);
static calc_status_ty SubtractFunc(stack_t *numbers, stack_t *operators);
static calc_status_ty MultiplyFunc(stack_t *numbers, stack_t *operators);
static calc_status_ty DivideFunc(stack_t *numbers, stack_t *operators);
static calc_status_ty PowerFunc(stack_t *numbers, stack_t *operators);

/******************************************************************************/
/*							Stack envelop functions:						  */
static calc_status_ty NumPush(stack_t *numbers, double num);
static double NumPop(stack_t *numbers);
static calc_status_ty OpPush(stack_t *operators, int op);
static int OpPop(stack_t *operators);
static void Terminate(stack_t *numbers, stack_t *operators);

/******************************************************************************/
/*							The calculator function:						  */
calc_status_ty Calculate(const char *math_exp, double *result)
{
	calc_ty calc_data = {NULL};
	fsm_ty calc_fsm = {NumHandler, WAIT_FOR_NUM};
	calc_status_ty status = CALC_SUCCESS;
	
	assert(NULL != math_exp);
	assert(NULL != result);
	
	status = InitCalcData(&calc_data, math_exp, result);
	if(CALC_MALLOC_ERROR == status)
	{
		return status;
	}
	if(ErrorHandler != FSM[WAIT_FOR_NUM]['@'].handler)
	{
		InitFSM();
		InitOpLUT();
	}
	while(FINAL_STATE != calc_fsm.next_state && ERROR != calc_fsm.next_state && 
			CALC_SUCCESS == status)
	{
		calc_fsm = FSM[calc_fsm.next_state][(int)*calc_data.ptr_math_exp];
		status = calc_fsm.handler(&calc_data);
	}
	Terminate(calc_data.numbers, calc_data.operators);
	
	return status;
}

/******************************************************************************/
/*							  Init functions:							  	  */
static void InitFSM()
{
	size_t i = 0;
	
	for(i = 0; NUM_OF_EVENTS > i; ++i)
	{
		FSM[WAIT_FOR_NUM][i].next_state = ERROR;
		FSM[WAIT_FOR_NUM][i].handler = ErrorHandler;
		FSM[WAIT_FOR_OP][i].next_state = ERROR;
		FSM[WAIT_FOR_OP][i].handler = ErrorHandler;
		FSM[POST_CLOSE_PARAN][i].next_state = ERROR;
		FSM[POST_CLOSE_PARAN][i].handler = ErrorHandler;
	}
	for(i = '0'; '9' >= i; ++i)
	{
		FSM[WAIT_FOR_NUM][i].next_state = WAIT_FOR_OP;
		FSM[WAIT_FOR_NUM][i].handler = NumHandler;
		FSM[POST_CLOSE_PARAN][i].next_state = WAIT_FOR_NUM;
		FSM[POST_CLOSE_PARAN][i].handler = PostCloseParanHandler;
	}
	FSM[WAIT_FOR_NUM]['('].next_state = WAIT_FOR_NUM;
	FSM[WAIT_FOR_NUM]['('].handler = OpenParanHandler;
	FSM[WAIT_FOR_OP]['('].next_state = WAIT_FOR_NUM;
	FSM[WAIT_FOR_OP]['('].handler = OpenParanHandler;
	FSM[WAIT_FOR_OP][')'].next_state = POST_CLOSE_PARAN;
	FSM[WAIT_FOR_OP][')'].handler = CloseParanHandler;
	
	FSM[WAIT_FOR_OP]['+'].next_state = WAIT_FOR_NUM;
	FSM[WAIT_FOR_OP]['+'].handler = OpHandler;
	FSM[WAIT_FOR_OP]['-'].next_state = WAIT_FOR_NUM;
	FSM[WAIT_FOR_OP]['-'].handler = OpHandler;
	FSM[WAIT_FOR_OP]['*'].next_state = WAIT_FOR_NUM;
	FSM[WAIT_FOR_OP]['*'].handler = OpHandler;
	FSM[WAIT_FOR_OP]['/'].next_state = WAIT_FOR_NUM;
	FSM[WAIT_FOR_OP]['/'].handler = OpHandler;
	FSM[WAIT_FOR_OP]['^'].next_state = WAIT_FOR_NUM;
	FSM[WAIT_FOR_OP]['^'].handler = OpHandler;
	
	FSM[WAIT_FOR_NUM]['+'].next_state = WAIT_FOR_OP;
	FSM[WAIT_FOR_NUM]['+'].handler = NumHandler;
	FSM[WAIT_FOR_NUM]['-'].next_state = WAIT_FOR_OP;
	FSM[WAIT_FOR_NUM]['-'].handler = NumHandler;
	
	FSM[POST_CLOSE_PARAN]['+'].next_state = WAIT_FOR_NUM;
	FSM[POST_CLOSE_PARAN]['+'].handler = OpHandler;
	FSM[POST_CLOSE_PARAN]['-'].next_state = WAIT_FOR_NUM;
	FSM[POST_CLOSE_PARAN]['-'].handler = OpHandler;
	FSM[POST_CLOSE_PARAN]['*'].next_state = WAIT_FOR_NUM;
	FSM[POST_CLOSE_PARAN]['*'].handler = OpHandler;
	FSM[POST_CLOSE_PARAN]['/'].next_state = WAIT_FOR_NUM;
	FSM[POST_CLOSE_PARAN]['/'].handler = OpHandler;
	FSM[POST_CLOSE_PARAN]['^'].next_state = WAIT_FOR_NUM;
	FSM[POST_CLOSE_PARAN]['^'].handler = OpHandler;
	FSM[POST_CLOSE_PARAN]['('].next_state = WAIT_FOR_NUM;
	FSM[POST_CLOSE_PARAN]['('].handler = PostCloseParanHandler;
	FSM[POST_CLOSE_PARAN][')'].next_state = POST_CLOSE_PARAN;
	FSM[POST_CLOSE_PARAN][')'].handler = CloseParanHandler;
	
	FSM[WAIT_FOR_NUM][' '].next_state = WAIT_FOR_NUM;
	FSM[WAIT_FOR_NUM][' '].handler = SpaceHandler;
	FSM[WAIT_FOR_OP][' '].next_state = WAIT_FOR_OP;
	FSM[WAIT_FOR_OP][' '].handler = SpaceHandler;
	FSM[POST_CLOSE_PARAN][' '].next_state = POST_CLOSE_PARAN;
	FSM[POST_CLOSE_PARAN][' '].handler = SpaceHandler;
	
	FSM[WAIT_FOR_NUM]['\0'].next_state = WAIT_FOR_OP;
	FSM[WAIT_FOR_NUM]['\0'].handler = NullAtNumHandler;
	FSM[WAIT_FOR_OP]['\0'].next_state = FINAL_STATE;
	FSM[WAIT_FOR_OP]['\0'].handler = FinalStateHandler;
	FSM[POST_CLOSE_PARAN]['\0'].next_state = FINAL_STATE;
	FSM[POST_CLOSE_PARAN]['\0'].handler = FinalStateHandler;
}

static void InitOpLUT()
{
	size_t i = 0;
	size_t j = 0;
	
	for(i = 0; OPERATORS_RANGE > i; ++i)
	{
		for(j = 0; OPERATORS_RANGE > j; ++j)
		{
			PERFORM_OP[i][j] = DoNothing;
		}
	}
	PERFORM_OP[PLUS][PLUS] = SumFunc;
	PERFORM_OP[PLUS][MINUS] = SumFunc;
	PERFORM_OP[MINUS][PLUS] = SubtractFunc;
	PERFORM_OP[MINUS ][MINUS] = SubtractFunc;
	PERFORM_OP[PLUS][NULL_TERM_OP] = SumFunc;
	PERFORM_OP[MINUS][NULL_TERM_OP] = SubtractFunc;
	
	PERFORM_OP[MULTIPLY][MULTIPLY] = MultiplyFunc;
	PERFORM_OP[MULTIPLY][DIVIDE] = MultiplyFunc;
	PERFORM_OP[DIVIDE][MULTIPLY] = DivideFunc;
	PERFORM_OP[DIVIDE][DIVIDE] = DivideFunc;
	PERFORM_OP[MULTIPLY][NULL_TERM_OP] = MultiplyFunc;
	PERFORM_OP[DIVIDE][NULL_TERM_OP] = DivideFunc;
	
	PERFORM_OP[MULTIPLY][PLUS] = MultiplyFunc;
	PERFORM_OP[MULTIPLY][MINUS] = MultiplyFunc;
	PERFORM_OP[DIVIDE][PLUS] = DivideFunc;
	PERFORM_OP[DIVIDE][MINUS] = DivideFunc;
	
	PERFORM_OP[POWER][PLUS] = PowerFunc;
	PERFORM_OP[POWER][MINUS] = PowerFunc;
	PERFORM_OP[POWER][MULTIPLY] = PowerFunc;
	PERFORM_OP[POWER][DIVIDE] = PowerFunc;
	PERFORM_OP[POWER][NULL_TERM_OP] = PowerFunc;
	
	PERFORM_OP[PLUS][CLOSE_PARAN] = SumFunc;
	PERFORM_OP[MINUS][CLOSE_PARAN] = SubtractFunc;
	PERFORM_OP[MULTIPLY][CLOSE_PARAN] = MultiplyFunc;
	PERFORM_OP[DIVIDE][CLOSE_PARAN] = DivideFunc;
	PERFORM_OP[POWER][CLOSE_PARAN] = PowerFunc;
}

/******************************************************************************/
static calc_status_ty InitCalcData(calc_ty *calc_data, const char *math_exp, 
									double *result)
{
	calc_status_ty status = CALC_SUCCESS;
	
	assert(NULL != calc_data);
	assert(NULL != math_exp);
	
	calc_data->exp_len = strlen(math_exp);
	calc_data->numbers = StackCreate(calc_data->exp_len / 2 + 2);
	if(NULL == calc_data->numbers)
	{
		return CALC_MALLOC_ERROR;
	}
	calc_data->operators = StackCreate(calc_data->exp_len / 2 + 2);
	if(NULL == calc_data->operators)
	{
		StackDestroy(calc_data->numbers);
		return CALC_MALLOC_ERROR;
	}
	status = OpPush(calc_data->operators, '=');
	calc_data->ptr_math_exp = math_exp;
	calc_data->result = result;
	calc_data->open_paran_num = 0;
	
	return status;
}

/******************************************************************************/
/*						  	Handler functions:						  		  */
static calc_status_ty NumHandler(calc_ty *calc_data)
{
	char *ptr_next = NULL;
	double num = 0;
	calc_status_ty status = CALC_SUCCESS;
	
	assert(NULL != calc_data);
	
	num = strtod(calc_data->ptr_math_exp, &ptr_next);
	if(calc_data->ptr_math_exp != ptr_next)
	{
		status = NumPush(calc_data->numbers, num);
		calc_data->ptr_math_exp = (const char *)ptr_next;
	}
	else
	{
		status = CALC_SYNTAX_ERROR;
	}
	
	return status;
}

static calc_status_ty OpHandler(calc_ty *calc_data)
{
	int next_op = 0;
	calc_status_ty status = CALC_SUCCESS;
	
	assert(NULL != calc_data);
	
	next_op = (int)*calc_data->ptr_math_exp;
	status = OpPerformer(calc_data, next_op);
	if(CALC_SUCCESS == status)
	{
		status = OpPush(calc_data->operators, next_op);
	}
	++calc_data->ptr_math_exp;
	
	return status;
}

static calc_status_ty ErrorHandler(calc_ty *calc_data)
{
	(void)calc_data;
	
	return CALC_SYNTAX_ERROR;
}

static calc_status_ty OpenParanHandler(calc_ty *calc_data)
{
	calc_status_ty status = CALC_SUCCESS;
	
	assert(NULL != calc_data);
	
	if(StackSize(calc_data->numbers) == StackSize(calc_data->operators) - 
										calc_data->open_paran_num)
	{
		status = OpPush(calc_data->operators, '*');
	}
	status = OpPush(calc_data->operators, '(');
	calc_data->open_paran_num += 1;
	++calc_data->ptr_math_exp;
	
	return status;
}

static calc_status_ty CloseParanHandler(calc_ty *calc_data)
{
	calc_status_ty status = CALC_SUCCESS;
	
	assert(NULL != calc_data);
	
	if(0 != calc_data->open_paran_num)
	{
		status = OpPerformer(calc_data, ')');
		if(CALC_SUCCESS == status)
		{
			OpPop(calc_data->operators);
			++calc_data->ptr_math_exp;
			calc_data->open_paran_num -= 1;
		}
	}
	else
	{
		status = PARAN_IMBALANCE;
	}
		
	return status;
}

static calc_status_ty PostCloseParanHandler(calc_ty *calc_data)
{
	assert(NULL != calc_data);
	
	return OpPush(calc_data->operators, '*');
}

static calc_status_ty FinalStateHandler(calc_ty *calc_data)
{
	calc_status_ty status = CALC_SUCCESS;
	
	assert(NULL != calc_data);
	
	status = OpPerformer(calc_data, NULL_TERM);
	
	if(CALC_SUCCESS == status)
	{	
		int op = OpPop(calc_data->operators);

		if('=' == op)
		{
			*calc_data->result = NumPop(calc_data->numbers);;
		}
		else
		{
			status = PARAN_IMBALANCE;
		}
	}

	return status;
}

static calc_status_ty NullAtNumHandler(calc_ty *calc_data)
{
	calc_status_ty status = CALC_SUCCESS;
	
	assert(NULL != calc_data);
	
	if(0 == calc_data->exp_len)
	{
		status = NumPush(calc_data->numbers, 0);
	}
	else
	{
		status = CALC_SYNTAX_ERROR;
	}
	
	return status;
}

static calc_status_ty SpaceHandler(calc_ty *calc_data)
{
	calc_status_ty status = CALC_SUCCESS;
	
	assert(NULL != calc_data);
	
	++calc_data->ptr_math_exp;
	if(' ' == *calc_data->ptr_math_exp)
	{
		status = CALC_SYNTAX_ERROR;
	}
	
	return status;
}

/******************************************************************************/
/*				Function for the performance of the operation:				  */
static calc_status_ty OpPerformer(calc_ty *calc_data, int next_op)
{
	perform_op_t perform_op = NULL;
	calc_status_ty status = CALC_SUCCESS;
	int op = 0;
	
	assert(NULL != calc_data);
	
	while(DoNothing != perform_op && CALC_SUCCESS == status)
	{
		op = OpPop(calc_data->operators);
	 	perform_op = PERFORM_OP[op - FIRST_OP_ASCII][next_op - FIRST_OP_ASCII];
	 	status = perform_op(calc_data->numbers, calc_data->operators);
	}
	if(DoNothing == perform_op)
	{
		status = OpPush(calc_data->operators, op);
	}
	
	return status;
}

/******************************************************************************/
/*						  	Operation functions:						  	  */
static calc_status_ty DoNothing(stack_t *numbers, stack_t *operators)
{
	(void)numbers;
	(void)operators;
	
	return CALC_SUCCESS;
}

static calc_status_ty SumFunc(stack_t *numbers, stack_t *operators)
{
	double num1 = 0;
	double num2 = 0;
	double result = 0;
	calc_status_ty status = CALC_SUCCESS;
	
	assert(NULL != numbers);
	assert(NULL != operators);
	
	num2 = NumPop(numbers);
	num1 = NumPop(numbers);
	result = num1 + num2;
	status = NumPush(numbers, result);
	
	return status;
}

static calc_status_ty SubtractFunc(stack_t *numbers, stack_t *operators)
{
	double num1 = 0;
	double num2 = 0;
	double result = 0;
	calc_status_ty status = CALC_SUCCESS;
	
	assert(NULL != numbers);
	assert(NULL != operators);
	
	num2 = NumPop(numbers);
	num1 = NumPop(numbers);
	result = num1 - num2;
	status = NumPush(numbers, result);
	
	return status;
}

static calc_status_ty MultiplyFunc(stack_t *numbers, stack_t *operators)
{
	double num1 = 0;
	double num2 = 0;
	double result = 0;
	calc_status_ty status = CALC_SUCCESS;
	
	assert(NULL != numbers);
	assert(NULL != operators);
	
	num2 = NumPop(numbers);
	num1 = NumPop(numbers);
	result = num1 * num2;
	status = NumPush(numbers, result);
	
	return status;
}

static calc_status_ty DivideFunc(stack_t *numbers, stack_t *operators)
{
	double num1 = 0;
	double num2 = 0;
	double result = 0;
	calc_status_ty status = CALC_SUCCESS;
	
	assert(NULL != numbers);
	assert(NULL != operators);
	
	num2 = NumPop(numbers);
	if(0 != num2)
	{
		num1 = NumPop(numbers);
		result = num1 / num2;
		status = NumPush(numbers, result);
	}
	else
	{
		status = DIVIDE_BY_ZERO_ERROR;
	}
	
	return status;
}

static calc_status_ty PowerFunc(stack_t *numbers, stack_t *operators)
{
	double num1 = 0;
	double num2 = 0;
	double result = 0;
	calc_status_ty status = CALC_SUCCESS;
	
	assert(NULL != numbers);
	assert(NULL != operators);
	
	num2 = NumPop(numbers);
	num1 = NumPop(numbers);
	result = pow(num1, num2);
	status = NumPush(numbers, result);
	
	return status;
}

/******************************************************************************/
/*							Stack envelop functions:						  */
static calc_status_ty NumPush(stack_t *numbers, double num)
{
	double *alloc_num = NULL;
	calc_status_ty status = CALC_SUCCESS;
	
	assert(NULL != numbers);
	
	alloc_num = (double *)malloc(sizeof(double));
	if(NULL == alloc_num)
	{
		return CALC_MALLOC_ERROR;
	}
	*alloc_num = num;
	if(StackCapacity(numbers) > StackSize(numbers))
	{
		StackPush(numbers, (void *)alloc_num);
	}
	else
	{
		status = PARAN_IMBALANCE;
	}
	
	return status;
}

static double NumPop(stack_t *numbers)
{
	double *alloc_num = NULL;
	double num = 0;
	
	assert(NULL != numbers);
	
	alloc_num = (double *)StackPeek(numbers);
	num = *alloc_num;
	memset(alloc_num, 0, sizeof(double));
	free(alloc_num);
	StackPop(numbers);
	
	return num;
}

static calc_status_ty OpPush(stack_t *operators, int op)
{
	int *alloc_op = NULL;
	
	assert(NULL != operators);
	
	alloc_op = (int *)malloc(sizeof(int));
	if(NULL == alloc_op)
	{
		return CALC_MALLOC_ERROR;
	}
	*alloc_op = op;
	if(StackCapacity(operators) > StackSize(operators))
	{
		StackPush(operators, (void *)alloc_op);
	}
	else
	{
		return PARAN_IMBALANCE;
	}
	
	return CALC_SUCCESS;
}

static int OpPop(stack_t *operators)
{
	int *alloc_op = NULL;
	int op = 0;
	
	assert(NULL != operators);
	
	alloc_op = (int *)StackPeek(operators);
	op = *alloc_op;
	memset(alloc_op, 0, sizeof(int));
	free(alloc_op);
	StackPop(operators);
	
	return op;
}

static void Terminate(stack_t *numbers, stack_t *operators)
{
	while(1 != StackIsEmpty(operators))
	{
		OpPop(operators);
	}
	while(1 != StackIsEmpty(numbers))
	{
		NumPop(numbers);
	}
	StackDestroy(numbers);
	StackDestroy(operators);
}
