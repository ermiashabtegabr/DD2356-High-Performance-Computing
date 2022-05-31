#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#define N 100

typedef struct {
	int p;
	MPIComm comm_cart;
	MPIComm comm_row;
	MPIComm comm_col;
	int q;
	int my_row;
	int my_col;
	int my_rank;
} GRID;

typedef struct {
	int n_bar;
} LOCAL_MATRIX;

void initialize_matrices(double **matrix, int n)
{
	int i, j;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			matrix[i][j] = (double)rand() / RAND_MAX;
}

void free_allocated_matrix(double **matrix, int n)
{
	int i;
	for (i = 0; i < n; i++)
		free(matrix[i]);
	free(matrix);
}

void allocate_matrix(double ***matrix, int n)
{
	int i;
	*matrix = malloc(n * sizeof(double *));
	for (i = 0; i < n; i++)
		(*matrix)[i] = malloc(n * sizeof(double));
}

void calculate_local(double **A, double **B, double **C, int n) 
{
	int i, j, k;
	for (i = 0; i < n; i++) 
		for (j = 0; j < n; j++) 
			for (k = 0; k < n; k++) 
				C[i][j] += A[i][j] * B[j][k];

}

void setup_grid(GRID * grid) {
	int old_rank;
	int dims[2];
	int periodic[2];
	int coords[2];
	int belonging_coords[2];

	MPI_Comm_size(MPI_COMM_WORLD, &(grid -> p));
	MPI_Comm_rank(MPI_COMM_WORLD, &old_rank);

	grid -> q = (int) sqrt((double) grid -> p);
	dims[0] = dims[1] = 1;

	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periodic, 1, &(grid -> comm_cart));
	MPI_Comm_rank(grid -> comm_cart, &(grid -> my_rank));
	MPI_Cart_coords(grid -> comm_cart, grid -> my_rank, 2, coords);

	grid -> my_row = coords[0];
	grid -> my_col = coords[1];

	belonging_coords[0] = 0; belonging_coords[1] = 1;
	MPI_Cart_sub(grid -> comm_cart, belonging_coords, &(grid -> comm_row));

	belonging_coords[0] = 1; belonging_coords[1] = 0;
	MPI_Cart_sub(grid -> comm_cart, belonging_coords, &(grid -> comm_col));
}

void fox_algorithm(int n, GRID* grid, double** local_A, double** local_B, double** local_C) {
	double** temp_A;
	int step;
	int bcast_root; 
	int n_bar;
	int source;
	int dest;
	MPI_Status status;

	n_bar = n/grid -> q;
	//TODO set to zero
	

	source = (grid -> my_row+ 1) % grid -> q;
	dest = (grid -> my_row + grid -> q - 1) % grid -> q;

	//TODO local matrix allocate

	for (step = 0; step < grid -> q; step++) {
		bcast_root = (grid -> my_row+ step) % grid -> q;
		if (bcast_root == grid -> my_col) {
			MPI_Bcast(local_A, 1, MPI_FLOAT, bcast_root, grid->comm_row);

		}
	}

}

int main(int argc, char *argv[])
{
	int size, provided, iter, nr_processes;
	// current process rank in 
	int rank, rank_row, rank_col;
	// matrices  
	double **A, **B, **C;

	allocate_matrix(&A, N);
	allocate_matrix(&B, N);
	allocate_matrix(&C, N);

	initialize_matrices(A, N);
	initialize_matrices(B, N);

	MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	printf("number of processes %d\n", size);

	int nr_processes = sqrt(size);
	if (N % nr_processes != 0) {
		MPI_Finalize();
		fprintf(stderr, "Square Root of Processes does not divide Number of elements.\n");
		return 0;
	}

	free_allocated_matrix(A, N);
	free_allocated_matrix(B, N);
	free_allocated_matrix(C, N);

	return 0;
}
