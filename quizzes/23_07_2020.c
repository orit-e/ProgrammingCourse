int CountCoupleBits(long n)
{
	int counter = 0;
	
	n = n & (n << 1);
	while(0 != n)
	{
		n &= (n - 1);
		++counter;
	}
	
	return counter;
}
