#include <stdio.h>  /* printf, getchar */
#include <stdlib.h> /* system() */

void IfEleseFunc();
/*
* Description:
* The IfEleseFunc() asks the user to press a key, than according to the key 
* pressed starts an if/else test.
* If 'A' is pressed the program will print "A pressed".
* If 'T' is pressed the program will print "T pressed".
* If the 'esc' button is pressed the loop in the program will end and the 
* program will exit.
*/

void SwitchCaseFunc();
/*
* Description:
* The SwitchCaseFunc() asks the user to press a key, than according to the key 
* pressed starts a switch/case test.
* If 'A' is pressed the program will print "A pressed".
* If 'T' is pressed the program will print "T pressed".
* If the 'esc' button is pressed the loop in the program will end and the 
* program will exit.
*/


int AFunc();
/*
* Description:
* The AFunc() function, when called, will print "A pressed", Than will exit with
* a return value of 0.
*/


int TFunc();
/*
* Description:
* The TFunc() function, when called, will print "T pressed", Than will exit with
* a return value of 0.
*/

int ZerroFunc();
/*
* Description:
* The ZerroFunc() function, when called, will exit with a return value of 0.
*/

int ESCFunc();
/*
* Description:
* The ESCFunc() function, when called, will exit with a return value of 1.
*/

void LUTFunc();
/*
* Description:
* The LUTFunc() asks the user to press a key, than according to the key 
* pressed goes to the appropriate function pointed to by the LUT array created 
* inside the program.
* If 'A' is pressed the LUT array will point to AFunc() function.
* If 'T' is pressed the LUT array will point to TFunc() function.
* If the 'esc' button is pressed LUT array will point to ESCFunc() function.
* This will cause the program loop to end and the program will exit.
* In all other cases LUT array will point to ZerroFunc() function.
*/



int main(int argc, char *argv[], char *envp[])
{
	IfEleseFunc();
	SwitchCaseFunc();
	LUTFunc();
	
	(void)argc;
	(void)argv;
	(void)envp;
	
	return 0;
}

void IfEleseFunc()
{
	unsigned char button = 0;
	
	system("stty -icanon -echo");
		
	while(27 != button)
	{
		printf("Press any key, to exit press 'ESC' button:\n");
		button = getchar();
		if('A' == button)
		{
			printf("A pressed\n");
		}
		else if('T' == button)
		{
			printf("T pressed\n");
		}
	}
	
	system("stty icanon echo");
}

void SwitchCaseFunc()
{
	unsigned char button = 0;
	
	system("stty -icanon -echo");
		
	while(27 != button)
	{
		printf("Press any key, to exit press 'ESC' button:\n");
		button = getchar();
		switch(button)
		{
			case 'A':
				printf("A pressed\n");
				break;
			case 'T':
				printf("T pressed\n");
				break;
			default:
				break;
		}
	}
	
	system("stty icanon echo");
}

int AFunc()
{
	printf("A pressed\n");
	
	return 0;
}

int TFunc()
{
	printf("T pressed\n");
	
	return 0;
}

int ZerroFunc()
{
	return 0;
}

int ESCFunc()
{
	return 1;
}

void LUTFunc()
{
	int (*lut[256])() = {0};
	unsigned int button = 0;
	int out_of_loop = 0;
	int i = 0;
	
	for(i = 0; i < 256; ++i)
	{
		lut[i] = &ZerroFunc;
	}
	lut[27] = &ESCFunc;
	lut[65] = &AFunc;
	lut[84] = &TFunc;
	
	system("stty -icanon -echo");
	
	while(1 != out_of_loop)
	{
		printf("Press any key, to exit press 'ESC' button:\n");
		button = getchar();
		out_of_loop = (*lut[button])();
	}
	
	system("stty icanon echo");
}
