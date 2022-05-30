#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mpi.h>


void initialize_matrices(double **matrix, int N)
{
	int i, j;
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			matrix[i][j] = (double)rand() / RAND_MAX;
}

void free_allocated_matrix(double **matrix, int N)
{
	int i;
	for (i = 0; i < N; i++)
		free(matrix[i]);
	free(matrix);
}

void allocate_matrix(double ***matrix, int N)
{
	int i;
	*matrix = malloc(N * sizeof(double *));
	for (i = 0; i < N; i++)
		(*matrix)[i] = malloc(N * sizeof(double));
}


int main(int argc, char *argv[])
{
	int i, N, rank, size, provided, iter;
	double **A, **B, **C;
	MPI_Comm new_comm, comm_row, comm_column;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

	N = sqrt(size);
	if ((N*N) != size) {
		printf("Number of processes is not a square\n");
		MPI_Finalize();
		return EXIT_FAILURE;
	}

	allocate_matrix(&A, N);
	allocate_matrix(&B, N);
	allocate_matrix(&C, N);

	initialize_matrices(A, N);
	initialize_matrices(B, N);

	const int ndims = 2;
	const int dims[2] = {N, N};
	const int periodic[2] = {1, 1};

	int coords[2];
	MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periodic, 0, &new_comm);
	MPI_Comm_rank(new_comm, &rank);
	MPI_Cart_coords(new_comm, rank, 2, coords);

	iter = N;
	int counter = 0;

	for (i = 0; i < iter; i++) {

		if (coords[0] == (coords[1] + counter)) {
			printf("Proc %d - (%d, %d)\n", rank, coords[0], coords[1]);
		}

		counter = (counter + 1) >= N ? 0 : counter + 1;
	}


	free_allocated_matrix(A, N);
	free_allocated_matrix(B, N);
	free_allocated_matrix(C, N);

	return 0;
}
