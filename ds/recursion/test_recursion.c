#include <stdio.h> /* printf */
#include <stdlib.h>
#include <string.h> /* strcmp */

#include "stack.h"
#include "recursion.h"
#include "tests.h"

int TestFibonacciIter();
int TestFibonaccirec();
int TestFlipList();
int TestStackSort();
int TestStrlen();
int TestStrcmp();
int TestStrcpy();
int TestStrcat();
int TestStrstr();

int main(int argc, char *argv[])
{
	RunTest(TestFibonacciIter(),    "TestFibonacciIter");
	RunTest(TestFibonaccirec(),		"TestFibonaccirec");
	RunTest(TestFlipList(),         "TestFlipList");
	RunTest(TestStackSort(),        "TestStackSort");
	RunTest(TestStrlen(),          	"TestStrlen");
	RunTest(TestStrcmp(),           "TestStrcmp");
	RunTest(TestStrcpy(),           "TestStrcpy");
	RunTest(TestStrcat(),           "TestStrcat");
	RunTest(TestStrstr(),           "TestStrstr");

	(void) argc;
	(void) argv;

    return 0;
}

int TestFibonacciIter()
{
	int result = 0;
	
	result += (0 != FibonacciIter(0));
	result += (1 != FibonacciIter(1));
	result += (5 != FibonacciIter(5));
	
    return result;
}

int TestFibonaccirec()
{
	int result = 0;
	
	result += (0 != FibonacciRec(0));
	result += (1 != FibonacciRec(1));
	result += (5 != FibonacciRec(5));
	
    return result;
}

int TestFlipList()
{
    node_t node1 = {NULL};
    node_t node2 = {NULL};
    node_t node3 = {NULL};
    node_t node4 = {NULL};
	int result = 0;
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
	
    node1.data = &a;
    node1.next = NULL;
	result += (&node1 != FlipList(&node1));
    node1.next = &node2;
    node2.data = &b;
    node2.next = &node3;
    node3.data = &c;
    node3.next = &node4;
    node4.data = &d;
    node4.next = NULL;
	result += (&node4 != FlipList(&node1));
    result += (&node3 != node4.next);
    result += (&node2 != node3.next);
    result += (&node1 != node2.next);
    result += (NULL != node1.next);
	
	return result;
}

int TestStackSort()
{
    stack_t *stack = NULL;
	int result = 0;
	int a = 1;
    int a2 = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;
    int h = 8;
	
    stack = StackCreate(9);
	result += (stack != StackSort(stack, 0));
    StackPush(stack, &a);
	result += (stack != StackSort(stack, 1));
	result += (&a != (int *)StackPeek(stack));
    StackPop(stack);
    StackPush(stack, &b);
    StackPush(stack, &a);
    result += (stack != StackSort(stack, 2));
    result += (&b != (int *)StackPeek(stack));
    StackPop(stack);
    result += (&a != (int *)StackPeek(stack));
    StackPop(stack);
    StackPush(stack, &a);
    StackPush(stack, &a2);
    result += (stack != StackSort(stack, 2));
    result += (&a2 != (int *)StackPeek(stack));
    StackPop(stack);
    result += (&a != (int *)StackPeek(stack));
    StackPop(stack);
    StackPush(stack, &d);
    StackPush(stack, &b);
    StackPush(stack, &h);
    StackPush(stack, &c);
    StackPush(stack, &g);
    StackPush(stack, &f);
    StackPush(stack, &a);
    StackPush(stack, &e);
    result += (stack != StackSort(stack, 8));
	result += (&h != (int *)StackPeek(stack));
    StackPop(stack);
    result += (&g != (int *)StackPeek(stack));
    StackPop(stack);
    result += (&f != (int *)StackPeek(stack));
    StackPop(stack);
    result += (&e != (int *)StackPeek(stack));
    StackPop(stack);
    result += (&d != (int *)StackPeek(stack));
    StackPop(stack);
    result += (&c != (int *)StackPeek(stack));
    StackPop(stack);
    result += (&b != (int *)StackPeek(stack));
    StackPop(stack);
    result += (&a != (int *)StackPeek(stack));
    StackPop(stack);
	StackDestroy(stack);

	return result;
}

int TestStrlen()
{

	int result = 0;
	char *str1 = "";
	char *str2 = " ";
	char *str3 = "hello!";
	
	result += (0 != Strlen(str1));
	result += (1 != Strlen(str2));
	result += (6 != Strlen(str3));
	
	return result;
}

int TestStrcmp()
{
	int result = 0;
	char *str1 = "";
	char *str2 = "hello!";
	char *str3 = "hello!";
    char *str4 = "hello";
    char *str5 = "goodbye!";
	
	result += (-104 != Strcmp(str1, str2));
	result += (104 != Strcmp(str2, str1));
	result += (0 != Strcmp(str3, str2));
	result += (-33 != Strcmp(str4, str2));
    result += (-1 != Strcmp(str5, str2));
	
	return result;
}

int TestStrcpy()
{
	int result = 0;
	char *str1 = "";
	char *str2 = "hello!";
	char str3[10] = {0};
	
	result += (str3 != Strcpy(str3, str1));
	result += (0 != Strlen(str3));
	result += (str3 != Strcpy(str3, str2));
	result += (6 != Strlen(str3));
	
	return result;
}

int TestStrcat()
{
	int result = 0;
	char *str1 = "";
	char *str2 = "hello!";
	char str3[15] = {0};
    
    str3[0] = 'h';
    str3[1] = 'e';
    str3[2] = 'l';
    str3[3] = 'l';
    str3[4] = 'o';
    str3[5] = ' ';
    str3[6] = '\0';
	
	result += (str3 != Strcat(str3, str1));
	result += (6 != Strlen(str3));
	result += (str3 != Strcat(str3, str2));
	result += (12 != Strlen(str3));
	
	return result;
}

int TestStrstr()
{	
	int result = 0;
	char *str1 = "hello!";
	char *str2 = "hello!";
	char *str3 = "hello hello!";
    char *str4 = "goodbye!";
    char *str5 = "hello! hello";
	
	result += (str2 != Strstr(str2, str1));
	result += (&str3[6] != Strstr(str3, str1));
	result += (&str4[8] != Strstr(str4, str1));
	result += (str5 != Strstr(str5, str1));

	return result;
}