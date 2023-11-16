int IsSumeFound(const int *numbers, const int num_arr_size, const int sum, 
					int *num_that_sum)
{
	int i = 0;
	int j = 0;
	int rest = 0;
	
	for(i = 0; i < num_arr_size; ++i)
	{
		rest = num_arr_size - i;
		for(j = i + 1; j < rest; ++j)
		{
			if(sum == numbers[i] + numbers[j])
			{
				*num_that_sum = numbers[i];
				++num_that_sum;
				*num_that_sum = numbers[j];
				return 1;
			}
		}
	}
	
	return 0;
}
