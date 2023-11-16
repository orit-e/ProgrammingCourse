#ifndef __WS5_H__
#define __WS5_H__

#include <stdio.h>  /* printf , all of the f*() for files... */
#include <assert.h> /* assert */
#include <string.h> /* strncmp */

/* Definitions for Exercise 2*/
#define MAX_CHARACTERS_PER_LINE 140
#define MAX_APPLY_INPUT 5
#define MAX_LOG_MARK_SIZE 8

/* Exercise 1 */
int PrintNumbersFromStruct();
/*
* Description:
* The PrintNumbersFromStruct() function, when invoked, creates a struct array of
* 10 elements from the print_me struct type and insert to each the integer 
* number of its element. At the end of this proccess it sends each integer 
* elemnents of the array to Print using the ptr_print function pointer if its 
* struct type.
*/

void Print(int number);
/*
* Description:
* The Print() function prints out the integer number it recieves.
*/

/* Exercise 2 - Logger */
int LoggerFunc(const char *file_name);
/*
* Description:
* The LoggerFunc() function recieves file_name and asks the user to enter 
* strings that are than appended to the end of file_name. The loop continue 
* untile the exit statment is invoked.
* Posible special strings will invoke different action from the one stated
* above:
* * "-exit" - will make the program to stop and exit.
* * "-remove" - will make the program to delete file_name from the computer.
* * "-count" - will make the program count the number of lines in file_name.
* * "<" sign attached to the first word of the string from the left  - will make 
*   the program write that string at the beginning of the file.
*
* Return value:
* The LoggerFunc() will return SUCCESS upon successfuly exiting the program 
* loop. Otherwise it will return one of the error statements described in the 
* error section.
*
* Errors:
* The LoggerFunc() function will return an error if one of the following 
* instances occurs:
* * Error while attempting to associete a stream to a file.
* * Error while attempting to close a file.
* * Error while attempting to write a string at the beginning of the file.
* * Error while attempting to remove a file.
*/

int ProcessInput(const char *file_name, const char *input);
/*
* Description:
* The ProcessInput() function recieves file_name and input string and 
* attempts to match string to one of the options available from apply_input 
* array of logger struct type with the help of the compare function pointer of 
* that apply_input element. When a match is found, ProcessInput() will send the
* input string to the appropriate function of execution with the help of the 
* matched element's operate function pointer.
* Posible outcomes of this process:
* * "-exit" - if matched will make the program to stop and exit.
* * "-remove" - if matched will make the program to delete file_name from the 
*   computer.
* * "-count" - if matched will make the program count the number of lines in 
*   file_name.
* * "<" sign attached to the first word of the string from the left  - if 
*   matched will make the program write that string at the beginning of the 
*   file.
* * If none of the above is invoked, the program will send the string to be 
*   appended at the end of file_name.
*
* Return value:
* The LoggerFunc() will return SUCCESS upon successfuly completing the job. 
* Otherwise it will return one of the error statements described in the error 
* section.
*
* Errors:
* The LoggerFunc() function will return an error if one of the following 
* instances occurs:
* * Error while attempting to associete a stream to a file.
* * Error while attempting to close a file.
* * Error while attempting to write a string at the beginning of the file.
* * Error while attempting to remove a file.
*/

int AppendString(const char *file_name, const char *string);
/*
* Description:
* The AppendString() function recieves file_name and a string to add to the end 
* of file_name. The characters held in string are than added to the end of 
* file_name.
*
* Return value:
* The AppendString() will return SUCCESS upon successfuly completing the job. 
* Otherwise it will return one of the error statements described in the 
* error section.
*
* Errors:
* The AppendString() function will return an error if one of the following 
* instances occurs:
* * Error while attempting to associete a stream to a file.
* * Error while attempting to close a file.
*/

int RemoveFile(const char *file_name, const char *string);
/*
* Description:
* The RemoveFile() function recieves file_name to be removed from the computer. 
* The function will than delete the file.
*
* Return value:
* The RemoveFile() will return SUCCESS upon successfuly removing file_name. 
* Otherwise it will return an error statement.
*
* Errors:
* The RemoveFile() function will return an error if while attempting to remove 
* file_name an error occure.
*/

int CountLines(const char *file_name, const char *string);
/*
* Description:
* The CountLines() function will count the number of lines in file_name and than
* print that number out.
*
* Return value:
* The CountLines() will return SUCCESS upon successfuly ending its task. 
* Otherwise it will return one of the error statements described in the 
* error section.
*
* Errors:
* The CountLines() function will return an error if one of the following 
* instances occurs:
* * Error while attempting to associete a stream to a file.
* * Error while attempting to close a file.
*/

int WriteAtBeginning(const char *file_name, const char *string);
/*
* Description:
* The WriteAtBeginning() function recieves file_name and a string to add at the 
* beginning of file_name. The characters held in string are than added to the 
* beginning of file_name.
*
* Return value:
* The WriteAtBeginning() will return SUCCESS upon successfuly completing the 
* job. Otherwise it will return one of the error statements described in the 
* error section.
*
* Errors:
* The WriteAtBeginning() function will return an error if one of the following 
* instances occurs:
* * Error while attempting to associete a stream to a file.
* * Error while attempting to close a file.
* * Error while attempting to write a string at the beginning of the file.
*/

int Exit(const char *file_name, const char *string);
/*
* Description:
* The Exit() function, when invoked, will signal the function calling it to stop
* its process and exit with the appropriate return value.
*/

/* enum for Exercise 2 - Logger */
enum logger_return_value
{
	SUCCESS = 0,
	ERROR_ASSOCIETING_STREAM,
	ERROR_CLOSING_FILE,
	ERROR_WRITE_AT_BEGINNING,
	ERROR_REMOVE,
	EXIT_PROGRAM
};

/* struct for Exercise 1 */
struct print_me
	{
		int number;
		void (*ptr_print)(int);
	};

/* struct for Exercise 2 - Logger */
struct logger
	{
		char string[MAX_LOG_MARK_SIZE];
		int (*compare)(const char *, const char *, size_t);
		int (*operate)(const char *, const char *);
	};
	
struct logger apply_input[MAX_APPLY_INPUT] = 
	{
		{"-exit", strncmp, Exit},
		{"-remove", strncmp, RemoveFile},
		{"-count", strncmp, CountLines},
		{"<", strncmp, WriteAtBeginning},
		{"", strncmp, AppendString}
	};

/* return message at the end of Exercise 2 - Logger run */
static char *logger_return_message[] = 
	{
		"Exit code 0: Logger successfuly terminated.",
		"Exit code 1: (Error) Failed associeting stream to file.",
		"Exit code 2: (Error) Failed to close file.",
		"Exit code 3: (Error) Failed while writing at the beginning of file.",
		"Exit code 4: (Error) Failed while removing file."
	};


#endif /*__WS5_H__*/
