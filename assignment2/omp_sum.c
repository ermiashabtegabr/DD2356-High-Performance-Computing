#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double omp_sum(double *x, size_t size)
{
	double sum_val = 0.0;

	#pragma omp parallel for
		for (size_t i = 0; i < size; i++) 
			sum_val += x[i];

	return sum_val;
}

double serial_sum(double *x, size_t size)
{
	double sum_val = 0.0;

	for (size_t i = 0; i < size; i++) {
		sum_val += x[i];
	}

	return sum_val;
}

void generate_random(double *input, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		input[i] = rand() / (double)(RAND_MAX);
	}
}

int main(int argc, char *argv[])
{
	double start, end;
	int array_size = 10000000;
	double *input = (double*)malloc(array_size*sizeof(double));
	generate_random(input, array_size);
	
	double serial_sum_res = serial_sum(input, array_size);
	start = omp_get_wtime();
	double omp_sum_res = omp_sum(input, array_size);
	end = omp_get_wtime() - start;
	
	printf("Time for OpenMp sum, array size=%d, T=%f\n", array_size, end);
	printf("Serial sum result=%f, OpenMp sum result=%f\n", serial_sum_res, omp_sum_res);
	free(input);
	return 0;
}

