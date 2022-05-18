#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

double mysecond(){
	struct timeval tp;
	struct timezone tzp;
	int i;

	i = gettimeofday(&tp,&tzp);
	return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
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

	double ts, t;

	int array_size = 10000000;
	double *input = (double*)malloc(array_size*sizeof(double));
	generate_random(input, array_size);
	
	ts = mysecond();
	double serial_sum_res = serial_sum(input, array_size);
	t = mysecond() - ts;
	
	printf("Time for serial sum, array size=%d, T=%f\n", array_size, t);
	
	free(input);
	return 0;
}

