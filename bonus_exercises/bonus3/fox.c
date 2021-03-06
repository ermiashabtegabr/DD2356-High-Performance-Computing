#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define N 8
#define TRUE 1
#define FALSE -1
//#define DEBUG

double rand_from(double min, double max)
{
	double range = (max - min);
	double div = RAND_MAX / range;
	return min + (rand() / div);
}

void initialize_matrices(double **matrix, int n, int min, int max)
{
	int i, j;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			matrix[i][j] = rand_from(min, max);
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

void get_submatrix(double **matrix, double **submatrix, int row, int col, int n)
{

#ifdef DEBUG
	printf("get_submatrix (%d, %d) n: %d\n", row, col, n);
#endif

	int i, j;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			submatrix[i][j] = matrix[i + (row * n)][j + (col * n)];
}

void set_to_zero(double **matrix, int n)
{
	int i, j;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			matrix[i][j] = 0;
}

void calculate_local(double **sub_A, double **sub_B, double **local_C, int n, int rank)
{
	int i, j, k;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			for (k = 0; k < n; k++)
				local_C[i][j] += sub_A[i][k] * sub_B[k][j];
}

int compare_to_sequential_result(double **fox, double **seq, int row, int col, int n)
{
	int i, j, k;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (fox[i][j] != seq[i + (row * n)][j + (col * n)])
				return FALSE;
		}
	}

	return TRUE;
}

int main(int argc, char *argv[])
{
	int rank, size, provided;

	// grid info
	int q, my_row, my_col;
	int dims[2];
	int coords[2];
	int periodic[2];
	int belonging_coords[2];
	MPI_Comm comm_cart, comm_row;

	// matrices
	double **A, **B, **C;

	/* --- setting up the grid --- */
	allocate_matrix(&A, N);
	allocate_matrix(&B, N);
	allocate_matrix(&C, N);

	srand(100);
	initialize_matrices(A, N, 1, 10);
	initialize_matrices(B, N, 1, 10);

	MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	q = (int)sqrt((double)size);
	dims[0] = dims[1] = q;
	periodic[0] = periodic[1] = 1;

	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periodic, 1, &comm_cart);
	MPI_Comm_rank(comm_cart, &rank);
	MPI_Cart_coords(comm_cart, rank, 2, coords);

	my_row = coords[0];
	my_col = coords[1];

	belonging_coords[0] = 0;
	belonging_coords[1] = 1;
	MPI_Cart_sub(comm_cart, belonging_coords, &comm_row);
	/* --------------------------- */

	/* --- setting up local and temp matrices --- */
	int submatrix_size = N / q;
	double **temp_A, **local_A, **local_B, **local_C;

	allocate_matrix(&local_A, submatrix_size);
	allocate_matrix(&local_B, submatrix_size);
	allocate_matrix(&local_C, submatrix_size);

	get_submatrix(A, local_A, my_row, my_col, submatrix_size);
	get_submatrix(B, local_B, my_row, my_col, submatrix_size);

#ifdef DEBUG
	if (rank == 0)
	{
		printf("------------- %d A matrix --------------\n", rank);
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				printf("%f ", A[i][j]);
			}
			printf("\n");
		}
		printf("----------------------------------------\n");
		printf("------------- %d B matrix --------------\n", rank);
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				printf("%f ", B[i][j]);
			}
			printf("\n");
		}
		printf("----------------------------------------\n");
	}
#endif

	int step, bcast_root, src, dst, up_rank, down_rank;
	MPI_Status status;

	set_to_zero(local_C, submatrix_size);
	MPI_Cart_shift(comm_cart, 0, 1, &up_rank, &down_rank);
	allocate_matrix(&temp_A, submatrix_size);
	/* ------------------------------------------ */

	/* --- fox algorithm --- */
	double start_time, end_time;

	MPI_Barrier(MPI_COMM_WORLD);
	start_time = MPI_Wtime();

	for (step = 0; step < q; step++)
	{

		bcast_root = (my_row + step) % q;
		if (bcast_root == my_col)
		{
#ifdef DEBUG
			printf("rank: %d - step: %d - coords: (%d, %d) broadcasting\n", rank, step, my_row, my_col);
			printf("------------- step: %d - %d broadcasting local_A -------------\n", step, rank);
			for (int i = 0; i < submatrix_size; i++)
			{
				for (int j = 0; j < submatrix_size; j++)
				{
					printf("%f ", local_A[i][j]);
				}
				printf("\n");
			}
			printf("--------------------------------------------------------------\n");
#endif
			for (int i = 0; i < submatrix_size; i++)
				MPI_Bcast(local_A[i], submatrix_size, MPI_DOUBLE, bcast_root, comm_row);

			calculate_local(local_A, local_B, local_C, submatrix_size, rank);
#ifdef DEBUG
			printf("------------- step: %d - %d current local_C -------------\n", step, rank);
			for (int i = 0; i < submatrix_size; i++) 
			{
				for (int j = 0; j < submatrix_size; j++) 
				{
					printf("%f ", local_C[i][j]);
				}
				printf("\n");
			}
			printf("---------------------------------------------------------\n");
#endif
		}
		else
		{

#ifdef DEBUG
			printf("rank: %d - step: %d - coords: (%d, %d) receiving\n", rank, step, my_row, my_col);
#endif
			for (int j = 0; j < submatrix_size; j++)
				MPI_Bcast(temp_A[j], submatrix_size, MPI_DOUBLE, bcast_root, comm_row);

#ifdef DEBUG
			printf("------------- step: %d - %d received local_A -------------\n", step, rank);
			for (int i = 0; i < submatrix_size; i++)
			{
				for (int j = 0; j < submatrix_size; j++)
				{
					printf("%f ", temp_A[i][j]);
				}
				printf("\n");
			}
			printf("----------------------------------------------------------\n");
#endif

			calculate_local(temp_A, local_B, local_C, submatrix_size, rank);

		}

		if (step != q - 1)
		{

#ifdef DEBUG
			printf("rank: %d - step: %d - coords: (%d, %d) - sending to: %d - receiving from: %d\n", rank, step, my_row, my_col, up_rank, down_rank);
			printf("------------- %d local B before sending -------------\n", rank);
			for (int i = 0; i < submatrix_size; i++)
			{
				for (int j = 0; j < submatrix_size; j++)
				{
					printf("%f ", local_B[i][j]);
				}
				printf("\n");
			}
			printf("-----------------------------------------------------\n");
#endif

			for (int i = 0; i < submatrix_size; i++)
				MPI_Sendrecv_replace(local_B[i], submatrix_size, MPI_DOUBLE, up_rank, 0, down_rank, 0, MPI_COMM_WORLD, &status);

#ifdef DEBUG
			printf("------------- %d local B after sending step: %d -------------\n", rank, step);
			for (int i = 0; i < submatrix_size; i++)
			{
				for (int j = 0; j < submatrix_size; j++)
				{
					printf("%f ", local_B[i][j]);
				}
				printf("\n");
			}
			printf("-------------------------------------------------------------\n");
#endif
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	end_time = MPI_Wtime();

	calculate_local(A, B, C, N, rank);
	if (!compare_to_sequential_result(local_C, C, my_row, my_col, submatrix_size))
	{
		printf("Rank: %d - matrix product not correct\n", rank);
	}
	else
	{
		printf("Rank: %d - matrix product correct\n", rank);
	}

	MPI_Finalize();
	if (rank == 0) printf("Elapsed time %f\n", end_time - start_time);

	return 0;
}
