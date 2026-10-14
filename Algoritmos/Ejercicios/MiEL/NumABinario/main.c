#include <stdio.h>

void num_a_bin(int num)
{
	if (num > 1) {
		num_a_bin(num / 2);
	}
	printf("%d", num % 2);
}

int main()
{
	num_a_bin(1234567890);
	printf("\n");
	return 0;
}
