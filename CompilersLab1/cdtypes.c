#include <stdio.h>
#include <math.h>
#include <limits.h>

int main() {
	printf("The size of type int is: %u\n", sizeof(int));	
	printf("The size of type unsigned int is: %u\n", sizeof(unsigned int));
	printf("The size of type long int is: %u\n", sizeof(long int));
	printf("The size of type unsigned long int is: %u\n", sizeof(unsigned long int));
	printf("The size of type unsigned short int is: %u\n", sizeof(unsigned short int));
	printf("The size of type short int is: %u\n", sizeof(short int));
	printf("The size of type long is: %u\n", sizeof(long));	
	printf("The size of type unsigned long is: %u\n", sizeof(unsigned long));
	printf("The size of type long double is: %u\n", sizeof(long double));
	printf("The size of type double is: %u\n", sizeof(double));
	printf("The size of type float is: %u\n", sizeof(float));
	printf("The size of type char is: %u\n", sizeof(char));




	int max_value;

	max_value = (int) pow(2, (sizeof(short int) * 8) - 1) - 1;
	printf("The last positive value is: %d\n", max_value);
	
	max_value = (int) pow(2, (sizeof(unsigned short int) * 8)) - 1;
	printf("The last positive value is: %d\n", max_value);
	
	printf("Value of SHRT_MAX: %d\nValue of USHRT_MAX: %d\n", SHRT_MAX, USHRT_MAX);

	int iov = 0; 

	while (iov >= 0) {iov += 10000;};
	iov -= 10000;
	printf("The last positive integer is: %d\n", iov);

	return 0;
}