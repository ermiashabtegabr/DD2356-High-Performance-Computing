#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define SEED     921
#define NUM_ITER 1000000000

int main(int argc, char* argv[])
{
	double x, y, z, pi;
	int local_count = 0;
	int rank, size, provided, i;

	MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double start_time, stop_time, elapsed_time;
	start_time = MPI_Wtime();

	srand(SEED * rank); // Important: Multiply SEED by "rank" when you introduce MPI!

	int flip = NUM_ITER / size;

	if (rank == 0) {
		int counts[size - 1];
		MPI_Request requests[size - 1];

		for (i = 1; i < size; i++) 
		{
			MPI_Irecv(&counts[i-1], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &requests[i-1]);
		}

		// Calculate PI following a Monte Carlo method
		for (int iter = 0; iter < flip; iter++)
		{
			// Generate random (X,Y) points
			x = (double)random() / (double)RAND_MAX;
			y = (double)random() / (double)RAND_MAX;
			z = sqrt((x*x) + (y*y));

			// Check if point is in unit circle
			if (z <= 1.0) local_count++;
		}

		MPI_Waitall(size - 1, requests, MPI_STATUSES_IGNORE);
		int global_count = local_count;

		for (i = 0; i < size - 1; i++) 
		{
			global_count += counts[i];
			
		}
		// Estimate Pi and display the result
		pi = ((double)global_count / (double) (flip * size)) * 4.0;

	} else {
		// Calculate PI following a Monte Carlo method
		for (int iter = 0; iter < flip; iter++)
		{
			// Generate random (X,Y) points
			x = (double)random() / (double)RAND_MAX;
			y = (double)random() / (double)RAND_MAX;
			z = sqrt((x*x) + (y*y));

			// Check if point is in unit circle
			if (z <= 1.0) local_count++;
		}

		MPI_Send(&local_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	stop_time = MPI_Wtime();
	elapsed_time = stop_time - start_time;

	if (rank == 0) {
		printf("%d Processes - The result is %f\n", size, pi);
		printf("Execution Time: %f\n", elapsed_time);
	}

	MPI_Finalize();
	return 0;
}

