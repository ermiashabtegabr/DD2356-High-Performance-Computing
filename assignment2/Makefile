MPFLAGS = gcc -fopenmp 
DFTFLAGS = gcc -fopenmp -lm

hello: hello_world_openmp.c
	$(MPFLAGS) hello_world_openmp.c -o hello_world_openmp
clean: hello_world_openmp.c
	rm hello_world_openmp
run: hello
	srun -n 1 ./hello_world_openmp

local:
	$(MPFLAGS) omp_local_sum.c -o omp_local_sum
run_local:
	srun -n 1 ./omp_local_sum
clean_local:
	rm omp_local_sum

opt: 
	$(MPFLAGS) opt_local_sum.c -o opt_local_sum
run_opt:
	srun -n 1 ./opt_local_sum
clean_opt: 
	rm opt_local_sum

dft:
	$(DFTFLAGS) DFTW_1.c -o DFTW_1
run_dft: 
	srun -n 1 ./DFTW_1 10000
clean_dft: 
	rm DFTW_1
	
