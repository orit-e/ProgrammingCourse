#include<stdio.h>

void HelloWorld();
float PowerOfTen(int power);
int FlipOrder(int number);
void Swap(int *num_1, int *num_2);

int main()
{
	int power;		/* The number that 10 will raise to the power of in the function PowerOfTen. */
	int number_to_flip;	/* The nuber to flip it's order in the function FlipOrder. */
	int num_2, num_1;	/* The values of the numberd will be swapped in the function Swap. */

	printf("Q. 3 test:\n");
	HelloWorld();
	printf("\n");
	
	printf("Q. 4 tests:\n");
	power = 2;
	printf("Test 1: 10 to the power of %d result is %5.2f\n", power, PowerOfTen(power));
	power = 0;
	printf("Test 2: 10 to the power of %d result is %5.2f\n", power, PowerOfTen(power));
	power = -1;
	printf("Test 3: 10 to the power of %d result is %5.2f\n\n", power, PowerOfTen(power));
	
	printf("Q. 5 tests:\n");
	number_to_flip = 12340;
	printf("After flipping the order of the number %d we get: %d.\n\n", number_to_flip, FlipOrder(number_to_flip));
	
	printf("Q. 6 tests:\n");
	num_1 = 1, num_2 = 2;
	printf("Test 1: The value before swapping: val_a = %d, val_b = %d.\n", num_1, num_2);
	Swap(&num_1, &num_2);
	printf("The value after swapping:          val_a = %d, val_b = %d\n", num_1, num_2);
	num_1 = 1, num_2 = 1;
	printf("Test 2: The value before swapping: val_a = %d, val_b = %d.\n", num_1, num_2);
	Swap(&num_1, &num_2);
	printf("The value after swapping:          val_a = %d, val_b = %d\n", num_1, num_2);
	num_1 = 1, num_2 = 0;
	printf("Test 3: The value before swapping: val_a = %d, val_b = %d.\n", num_1, num_2);
	Swap(&num_1, &num_2);
	printf("The value after swapping:          val_a = %d, val_b = %d\n", num_1, num_2);
	num_1 = 1, num_2 = -1;
	printf("Test 4: The value before swapping: val_a = %d, val_b = %d.\n", num_1, num_2);
	Swap(&num_1, &num_2);
	printf("The value after swapping:          val_a = %d, val_b = %d\n\n", num_1, num_2);
	return 0;
}

void HelloWorld()
{
	printf("\x22\x68\x65\x6c\x6c\x6f\x20\x77\x6f\x72\x6c\x64\x21\x22\x0a");
}

float PowerOfTen(int power)
{
	int i;
	float result = 1.0, base = 10.0;

	if (power < 0)
	{
		power = -power;
		base = 1 / base;
	}
	for (i = 0; i < power; i++)
	{
		result *= base;
	}
	return result;
}

int FlipOrder(int number)
{
	int flipped = 0;

	while (number != 0)
	{
		flipped *= 10;
		flipped += number % 10;
		number /= 10;
	}
	return flipped;
}

void Swap(int *num_1, int *num_2)
{
	int temp = *num_1;

	*num_1 = *num_2;
	*num_2 = temp;
}
