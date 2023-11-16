#include <stdio.h>  /* printf , all of the f*() for files... */
#include <assert.h> /* assert */
#include <string.h> /* strncmp */
#include "ws5.h"

int main(int argc, char *argv[], char *envp[])
{
	int logger_return;
	
	PrintNumbersFromStruct();
	
	logger_return = LoggerFunc("newtestfile");
	if(0 != logger_return)
	{
		printf("%s\n", logger_return_message[logger_return]);
	}
	
	
	(void)argc;
	(void)argv;
	(void)envp;
	
	return 0;
}

/* Exercise 1 */
int PrintNumbersFromStruct()
{
	int i = 0;
	
	struct print_me print_numbers_array[10];
	
	for(i = 0; i < 10; ++i)
	{
		print_numbers_array[i].number = i;
		print_numbers_array[i].ptr_print = &Print;
	}
	for(i = 0; i < 10; ++i)
	{
		print_numbers_array[i].ptr_print(print_numbers_array[i].number);
	}
	
	return 0;
}

void Print(int number)
{
	printf("%d\n", number);
}

/* Exercise 2 - Logger */
int LoggerFunc(const char *file_name)
{
	char input[MAX_CHARACTERS_PER_LINE] = {0};
	char *ptr_input = input;
	int function_return = 0;

	printf("***To exit: enter \"-exit\".\n");
	printf("***To count the number of lines entered so far: enter \"-count\".");
	printf("\n");
	printf("***To remove the file: enter \"-remove\".\n");
	
	while(EXIT_PROGRAM != function_return)
	{
		printf("Please enter your next line of text:\n");
		fgets(ptr_input, MAX_CHARACTERS_PER_LINE, stdin);
		function_return = ProcessInput(file_name, ptr_input);
		if(SUCCESS != function_return && EXIT_PROGRAM != function_return)
		{
			return function_return;
		}
	}
	
	return SUCCESS;
}

int ProcessInput(const char *file_name, const char *input)
{
	int i = 0;
	int function_return = 0;
	size_t mark_size = 0;
	
	assert(NULL != input);
	
	for(i = 0; i < MAX_APPLY_INPUT; ++i)
	{
		mark_size = strlen(apply_input[i].string);
		if(0 == apply_input[i].compare(apply_input[i].string, input, mark_size))
		{
			function_return = apply_input[i].operate(file_name, input);
			i = MAX_APPLY_INPUT;
		}
	}
	
	return function_return;
}

int AppendString(const char *file_name, const char *string)
{
	FILE *stream = NULL;
	
	assert(NULL != file_name);
	assert(NULL != string);
	
	stream = fopen(file_name, "a");
	if(NULL == stream)
	{
		return ERROR_ASSOCIETING_STREAM;
	}
	
	fputs(string, stream);
	if(0 != fclose(stream))
	{
		return ERROR_CLOSING_FILE;
	}
	
	return SUCCESS;
}

int RemoveFile(const char *file_name, const char *string)
{
	assert(NULL != file_name);
	
	if(0 != remove(file_name))
	{
		return ERROR_REMOVE;
	}
	
	(void)string;
	
	return SUCCESS;
}

int CountLines(const char *file_name, const char *string)
{
	FILE *stream = NULL;
	int num_lines = 0;
	int ch = 0;
	
	assert(NULL != file_name);
	
	stream = fopen(file_name, "r");
	if(NULL == stream)
	{
		return ERROR_ASSOCIETING_STREAM;
	}
	
	while(EOF != (ch = fgetc(stream)))
	{
		if('\n' == ch)
		{
			++num_lines;
		}
	}
	printf("The number of lines in the file is: %d.\n", num_lines);
	if(0 != fclose(stream))
	{
		return ERROR_CLOSING_FILE;
	}
	
	(void)string;
	
	return SUCCESS;
}
	
int Exit(const char *file_name, const char *string)
{
	(void)file_name;
	(void)string;
	
	return EXIT_PROGRAM;
}

int WriteAtBeginning(const char *file_name, const char *string)
{
	char copy[MAX_CHARACTERS_PER_LINE] = {0};
	char *ptr_copy = copy;
	char dummy_file[] = "./.temp";
	FILE *stream1 = NULL;
	FILE *stream2 = NULL;
	
	assert(NULL != file_name);
	assert(NULL != string);
	
	stream1 = fopen(file_name, "r");
	stream2 = fopen(dummy_file, "a");
	if(NULL == stream1 || NULL == stream2)
	{
		return ERROR_ASSOCIETING_STREAM;
	}
	
	++string;
	fputs(string, stream2);
	while(NULL != fgets(ptr_copy, MAX_CHARACTERS_PER_LINE, stream1))
	{
		fputs(ptr_copy, stream2);
	}
	if(0 != fclose(stream1) || 0 != fclose(stream2))
	{
		return ERROR_CLOSING_FILE;
	}
	if(0 != rename(dummy_file, file_name))
	{
		return ERROR_WRITE_AT_BEGINNING;
	}
	
	return SUCCESS;
}
