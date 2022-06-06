#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#define DIM 2
#define G 6.67408 * pow(10, -11)
typedef double vect_t[DIM];

double mysecond();

int main(int argc, char *argv[])
{
	if (argc < 4)
	{
		printf("missing arguments\nusage: ./n_body_simple n delta_t T\n");
		exit(1);
	}

	// user input
	int n, delta_t, T;
	int X, Y, q, k, iter;

	// position
	double x_diff, y_diff, dist, dist_cubed;

	// time measurement
	double start_time, end_time, elapsed_time, total_time, mean, std_deviation;

	n = atoi(argv[1]);
	delta_t = atoi(argv[2]);
	T = atoi(argv[3]);

	X = 0;
	Y = 1;
	iter = 10;

	vect_t *forces = malloc(n * sizeof(vect_t));
	vect_t *pos = malloc(n * sizeof(vect_t));
	vect_t *vel = malloc(n * sizeof(vect_t));
	double *masses = malloc(n * sizeof(double));
	double *measured_time = malloc(iter * sizeof(double));

	// initializing
	for (q = 0; q < n; q++)
	{
		pos[q][X] = (rand() / (double)(RAND_MAX)) * 2 - 1;
		pos[q][Y] = (rand() / (double)(RAND_MAX)) * 2 - 1;

		vel[q][X] = (rand() / (double)(RAND_MAX)) * 2 - 1;
		vel[q][Y] = (rand() / (double)(RAND_MAX)) * 2 - 1;

		masses[q] = fabs((rand() / (double)(RAND_MAX)) * 2 - 1);
	}

	// to avoid cold start
	for (q = 0; q < n; q++)
	{
		for (k = 0; k < n; k++)
		{
			if (k == q)
				continue;
			x_diff = pos[q][X] - pos[k][X];
			y_diff = pos[q][Y] - pos[k][Y];
			dist = sqrt(x_diff * x_diff + y_diff * y_diff);
			dist_cubed = dist * dist * dist;
			forces[q][X] -= G * masses[q] * masses[k] / dist_cubed * x_diff;
			forces[q][Y] -= G * masses[q] * masses[k] / dist_cubed * y_diff;
		}

		for (q = 0; q < n; q++)
		{
			pos[q][X] += delta_t * vel[q][X];
			pos[q][Y] += delta_t * vel[q][Y];
			vel[q][X] += delta_t / masses[q] * forces[q][X];
			vel[q][Y] += delta_t / masses[q] * forces[q][Y];
		}
	}

	for (int i = 0; i < iter; i++)
	{
		start_time = mysecond();
		for (int i = 0; i < T; i++)
		{
			for (q = 0; q < n; q++)
			{
				for (k = 0; k < n; k++)
				{
					if (k == q)
						continue;
					x_diff = pos[q][X] - pos[k][X];
					y_diff = pos[q][Y] - pos[k][Y];
					dist = sqrt(x_diff * x_diff + y_diff * y_diff);
					dist_cubed = dist * dist * dist;
					forces[q][X] -= G * masses[q] * masses[k] / dist_cubed * x_diff;
					forces[q][Y] -= G * masses[q] * masses[k] / dist_cubed * y_diff;
				}

				for (q = 0; q < n; q++)
				{
					pos[q][X] += delta_t * vel[q][X];
					pos[q][Y] += delta_t * vel[q][Y];
					vel[q][X] += delta_t / masses[q] * forces[q][X];
					vel[q][Y] += delta_t / masses[q] * forces[q][Y];
				}
			}
		}

		end_time = mysecond();
		elapsed_time = end_time - start_time;
		measured_time[i] = elapsed_time;
		total_time += elapsed_time;
	}

	mean = total_time / (double)iter;
	std_deviation = 0.0;
	for (int i = 0; i < iter; i++)
		std_deviation += pow(measured_time[i] - mean, 2);

	std_deviation = sqrt(std_deviation / iter);

	printf("%4d - Mean: %12.8f s\n", n, mean);
	printf(" - Standard deviation: %12.8f s\n", std_deviation);

	return 0;
}

double mysecond()
{
	struct timeval tp;
	struct timezone tzp;
	int i;

	i = gettimeofday(&tp, &tzp);
	return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
}
