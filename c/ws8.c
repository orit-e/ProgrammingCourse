/***********************
 * Submitted by: Orit. *
 * Reviewed by: Liron. *
 ***********************/ 

#include <stdio.h>  /* printf, sprintf, size_t */
#include <string.h> /* strcat, strlen */
#include <stdlib.h>	/* malloc, realloc */
#include <assert.h> /* assert */

/*****************************************************************************/
/*Macro for exercise 1 */
#define STRUCT_ARRAY_ELEMENTS 5
#define INT_NUM_AS_STRING 12

/*Macros for exercise 2*/
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define MAX3(a, b, c) (MAX2(MAX2((a), (b)), (c)))

/*Macros for exercise 3 + 4*/
#define SIZEOF_VAR(var) (size_t)((char *)((&var) + 1) - (char *)(&var))
#define SIZEOF_TYPE(type) (size_t)((1 + (type *)0))

/*****************************************************************************/
/*Enum for return values in exercise 1*/
enum is_error
{
	SUCCESS = 0,
	ERROR_MALLOC,
	ERROR_REALLOC
};

/*Typedef for struct and struct elements + struct declaration in exercise 2*/
typedef struct changing_type changing_type;
typedef void (*print_value)(const changing_type *);
typedef int (*add_value)(changing_type *, int);
struct changing_type
{
	void *ptr_to_value;
	print_value print;
	add_value add;
};
/*Error messages for exercise 1*/
static char *add_print_return_message[] = 
{
	"Exit code 0: PrintAndAddToCangingType() successfuly terminated.",
	"Exit code 1: malloc() error.",
	"Exit code 2: realloc() error."
};

/*****************************************************************************/
/* Exercise 1 functions declarations */
int PrintAndAddToCangingType();
/* DESCRIPTION:
 * The PrintAndAddToCangingType() function builds array of changing_type struct 
 * type, than prints ptr_to_value for each struct of the array, adds requested 
 * number to each ptr_to_value, and finaly prints ptr_to_value for each struct 
 * of the array again.
 *
 * Return value:
 * Return SUCCESS or an error message if one occur.
 *
 * Errors:
 * The function will return one of the following errors if one occur:
 * ERROR_MALLOC - If malloc error occurred.
 * ERROR_REALLOC - If realloc error occurred.
 */

int InitializeStructArray(changing_type *changing_type_array);
/* DESCRIPTION:
 * The InitializeStructArray() function builds array of changing_type struct 
 * type.
 *
 * Return value:
 * Return SUCCESS or an error if one occur.
 *
 * Errors:
 * The function will return the following error if it occur:
 * ERROR_MALLOC - If malloc error occurred.
 */
 
void SendToPrint(const changing_type *changing_type_array);
/* DESCRIPTION:
 * The SendToPrint() function recieves changing_type_array of changing_type 
 * struct type. Than send each struct in the array to the appropriate print 
 * function according to the print function pointer of the changing_type struct 
 * type.
 */
 
void PrintFloat(const changing_type *changing_type_element);
/* DESCRIPTION:
 * The PrintFloat() function recieves changing_type_element of changing_type 
 * struct type with a floating number associated to its ptr_to_value. The 
 * function print out that number.
 */
 
void PrintInt(const changing_type *changing_type_element);
/* DESCRIPTION:
 * The PrintInt() function recieves changing_type_element of changing_type 
 * struct type with an integer number associated to its ptr_to_value. The 
 * function print out that number.
 */
 
void PrintString(const changing_type *changing_type_element);
/* DESCRIPTION:
 * The PrintString() function recieves changing_type_element of changing_type 
 * struct type with a string associated to its ptr_to_value. The function print 
 * out that string.
 */
 
int SendToAdd(changing_type *changing_type_array, int add);
/* DESCRIPTION:
 * The SendToAdd() function recieves changing_type_aray of changing_type struct 
 * type and an add integer type. The function than send each struct in the array
 * to the appropriate add function according to the add function pointer of the 
 * changing_type struct type.
 *
 * Return value:
 * Return SUCCESS or an error message if one occur.
 *
 * Errors:
 * The function will return one of the following errors if one occur:
 * ERROR_MALLOC - If malloc error occurred.
 * ERROR_REALLOC - If realloc error occurred.
 */

int AddFloat(changing_type *changing_type_element, int add);
/* DESCRIPTION:
 * The AddFloat() function recieves changing_type_element of changing_type 
 * struct type with a floating number associated to its ptr_to_value and an add 
 * integer type. The function than adds the add integer to ptr_to_value floating
 * number.
 *
 * Return value:
 * Return SUCCESS.
 */
 
int AddInt(changing_type *changing_type_element, int add);
/* DESCRIPTION:
 * The AddInt() function recieves changing_type_element of changing_type 
 * struct type with a integer number associated to its ptr_to_value and an add 
 * integer type. The function than adds the add integer to ptr_to_value integer
 * number.
 *
 * Return value:
 * Return SUCCESS.
 */
 
int AddString(changing_type *changing_type_element, int add);
/* DESCRIPTION:
 * The AddString() function recieves changing_type_element of changing_type 
 * struct type with a string associated to its ptr_to_value and an add 
 * integer type. The function converts the add integer to its string 
 * repersentation, than appends it to the ptr_to_value string.
 *
 * Return value:
 * Return SUCCESS or an error message if one occur.
 *
 * Errors:
 * The function will return one of the following errors if one occur:
 * ERROR_MALLOC - If malloc error occurred.
 * ERROR_REALLOC - If realloc error occurred.
 */
 
void Free(changing_type *changing_type_array);
/* DESCRIPTION:
 * The Free() function recieves changing_type_aray of changing_type struct 
 * type. The function than free all allocated space pointed to by each of the 
 * ptr_to_value pointers of the changing_type struct type.
 */

/*****************************************************************************/
/* Exercises 2-4 functions declarations */
void TestMAX2();
/* DESCRIPTION:
 * Test for the MAX2() macro. Prints success or fail message if the test succeed
 * or fail accordingly.
 */
 
void TestMAX3();
/* DESCRIPTION:
 * Test for the MAX3() macro. Prints success or fail message if the test succeed
 * or fail accordingly.
 */
 
void TestSizeofVar();
/* DESCRIPTION:
 * Test for the SIZEOF_VAR() macro. Prints success or fail message if the test 
 * succeed or fail accordingly.
 */
 
void TestSziseofType();
/* DESCRIPTION:
 * Test for the SIZEOF_TYPE() macro. Prints success or fail message if the test 
 * succeed or fail accordingly.
 */
 
/*****************************************************************************/
int main(int argc, char *argv[])
{
	int return_val = 0;
	
	printf("Testing PrintAndAddToCangingType() of exercise 1:\n");
	return_val = PrintAndAddToCangingType();
	printf("%s\n",add_print_return_message[return_val]);
	if(0 != return_val)
	{
		return return_val;
	}
	
	printf("\nTesting macros from exercises 2-4:\n");
	TestMAX2();
	TestMAX3();
	TestSizeofVar();
	TestSziseofType();
	
	(void)argc;
	(void)argv;
	
	return SUCCESS;
}

/*****************************************************************************/
/*Exercise 1*/
void PrintFloat(const changing_type *changing_type_element)
{
	assert(NULL != changing_type_element);
	
	printf("%f ", *((float *)(changing_type_element->ptr_to_value)));
}

void PrintInt(const changing_type *changing_type_element)
{
	assert(NULL != changing_type_element);
	
	printf("%d ", *((int *)changing_type_element->ptr_to_value));
}

void PrintString(const changing_type *changing_type_element)
{
	assert(NULL != changing_type_element);
	
	printf("%s ", (char *)changing_type_element->ptr_to_value);
}

void SendToPrint(const changing_type *changing_type_array)
{
	int j = 0;
	
	assert(NULL != changing_type_array);
	
	for(j = 0; j < STRUCT_ARRAY_ELEMENTS; ++j)
	{
		changing_type_array->print(changing_type_array);
		++changing_type_array;
	}
	printf("\n");
}

int AddFloat(changing_type *changing_type_element, int add)
{
	  assert(NULL != changing_type_element);
	  (*(float *)changing_type_element->ptr_to_value) += add;
	  return SUCCESS;
}

int AddInt(changing_type *changing_type_element, int add)
{
	assert(NULL != changing_type_element);
	
	(*(int *)changing_type_element->ptr_to_value) += add;
	return SUCCESS;
}

int AddString(changing_type *changing_type_element, int add)
{
	size_t string_new_size = 
						strlen((char *)changing_type_element->ptr_to_value) + 1;
	char add_as_string[INT_NUM_AS_STRING] = {0};
	
	assert(NULL != changing_type_element);
	
	sprintf(add_as_string, "%d", add);
	string_new_size += strlen(add_as_string);
	changing_type_element->ptr_to_value = 
				realloc(changing_type_element->ptr_to_value, string_new_size);
	if(NULL == changing_type_element->ptr_to_value)
	{
		return ERROR_REALLOC;
	}
	changing_type_element->ptr_to_value = 
				strcat(changing_type_element->ptr_to_value, add_as_string);
	
	return SUCCESS;
}

int SendToAdd(changing_type *changing_type_array, int add)
{
	int j = 0;
	int return_val = 0;
	
	assert(NULL != changing_type_array);
	
	for(j = 0; j < STRUCT_ARRAY_ELEMENTS; ++j)
	{
		return_val = changing_type_array->add(changing_type_array, add);
		if(0 != return_val)
		{
			return return_val;
		}
		++changing_type_array;
	}
	
	return SUCCESS;
}

int InitializeStructArray(changing_type *changing_type_array)
{	
	float f1 = 4.2;
	float *ptr_f1 = (float *)malloc(sizeof(float) * 1);
	float f2 = 6.7;
	float *ptr_f2 = (float *)malloc(sizeof(float) * 1);
	char c1[] = "chapter";
	char *ptr_c1 = (char *)malloc(sizeof(char) * 8);
	int i1 = 12;
	int *ptr_i1 = (int *)malloc(sizeof(int) * 1);
	float f3 = 56.32;
	float *ptr_f3 = (float *)malloc(sizeof(float) * 1);
	
	assert(NULL != changing_type_array);
	
	if(NULL == ptr_f1 || NULL == ptr_f2 || NULL == ptr_c1 || NULL == ptr_i1 || 
		NULL == ptr_f3)
	{
		return ERROR_MALLOC;
	}
	
	*ptr_f1 = f1;
	*ptr_f2 = f2;
	ptr_c1 = strcpy(ptr_c1, c1);
	*ptr_i1 = i1;
	*ptr_f3 = f3;
	
	changing_type_array->ptr_to_value = ptr_f1;
	changing_type_array->print = PrintFloat;
	changing_type_array->add = AddFloat;
	++changing_type_array;
	
	changing_type_array->ptr_to_value = ptr_f2;
	changing_type_array->print = PrintFloat;
	changing_type_array->add = AddFloat;
	++changing_type_array;
	
	changing_type_array->ptr_to_value = ptr_c1;
	changing_type_array->print = PrintString;
	changing_type_array->add = AddString;
	++changing_type_array;
	
	changing_type_array->ptr_to_value = ptr_i1;
	changing_type_array->print = PrintInt;
	changing_type_array->add = AddInt;
	++changing_type_array;
	
	changing_type_array->ptr_to_value = ptr_f3;
	changing_type_array->print = PrintFloat;
	changing_type_array->add = AddFloat;
	
	return SUCCESS;
}

void Free(changing_type *changing_type_array)
{
	int j = 0;
	
	assert(NULL != changing_type_array);
	
	for(j = 0; j < STRUCT_ARRAY_ELEMENTS; ++j)
	{
		free(changing_type_array->ptr_to_value);
		changing_type_array->ptr_to_value = NULL;
		++changing_type_array;
	}
}

int PrintAndAddToCangingType()
{
	changing_type changing_type_array[STRUCT_ARRAY_ELEMENTS] = {NULL};
	int return_val = 0;
	int add = 10;
	
	return_val = InitializeStructArray(changing_type_array);
	if(0 != return_val)
	{
		Free(changing_type_array);
		return return_val;
	}
	SendToPrint(changing_type_array);
	return_val = SendToAdd(changing_type_array, add);
	if(0 != return_val)
	{
		Free(changing_type_array);
		return return_val;
	}
	SendToPrint(changing_type_array);
	Free(changing_type_array);
	
	return SUCCESS;
}

/*****************************************************************************/
/* Test functions for exercises 2-4 macros */
void TestMAX2()
{
	if(2 == MAX2(2,1) && 1 == MAX2(1, 1) && -1 == MAX2(-1, -4))
	{
		printf("MAX2: success.\n");
	}
	else
	{
		printf("MAX2: fail.\n");
	}
}

void TestMAX3()
{
	if(3 == MAX3(3, 2,1) && 1 == MAX3(1, 1, 1) && -1 == MAX3(-2, -1, -4) && 
		2 == MAX3(1, 0, 2))
	{
		printf("MAX3: success.\n");
	}
	else
	{
		printf("MAX3: fail.\n");
	}
}

void TestSizeofVar()
{
	float a[] = {3.5, 5.6, 6.4, 0.2};
	int b = 9;
	char c = 'a';
	
	if(16 == SIZEOF_VAR(a) && 4 == SIZEOF_VAR(b) && 1 == SIZEOF_VAR(c))
	{
		printf("SIZEOF_VAR: success.\n");
	}
	else
	{
		printf("SIZEOF_VAR: fail.\n");
	}
}

void TestSizeofType()
{
	if(24 == SIZEOF_TYPE(changing_type) && 4 == SIZEOF_TYPE(int) && 
		4 == SIZEOF_TYPE(float) && 8 == SIZEOF_TYPE(double))
	{
		printf("SIZEOF_TYPE: success.\n");
	}
	else
	{
		printf("SIZEOF_TYPE: fail.\n");
	}
}
