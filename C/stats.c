#include <stdio.h>
#include <math.h>
#define SIZE 10
int main()
{
	int array[SIZE];
	int length;
	extern int fillarray(int* array);
	extern void stats(int* array, int length);
	length = fillarray(array);
	int first_half_length = length / 2;
	int second_half_length = length - first_half_length;
	int first_half[first_half_length];
	int second_half[second_half_length];
	for(int i = 0; i < first_half_length; i++)
		first_half[i] = array[i];
	for(int i = 0; i < second_half_length; i++)
		second_half[i] = array[i + first_half_length];
	printf("Full array:\n");
	stats(array, length);
	printf("First half of the array:\n");
	stats(first_half, first_half_length);
	printf("Second half of the array:\n");
	stats(second_half, second_half_length);
}

int fillarray(int* array)
{
	int length = 0;
	while (scanf("%d", &array[length]) == 1){
		length++;
		if (length > 9)
			break;
	}
	return(length);
}

void stats(int* array, int length)
{
	int max = array[0];
	int min = array[0];
	int sum = 0;
	double mean = 0;
	for(int i = 0; i < length; i++){
		sum += array[i];
		if (array[i] > max)
			max = array[i];
		if (array[i] < min)
			min = array[i];
	}
	mean = (double) sum / (double) length;
	printf("min: %d\n", min);
	printf("max: %d\n", max);
	printf("mean: %f\n", mean);
}
