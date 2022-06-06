#!/bin/csh

foreach n (`seq 1 2 4 8 16`)
    env OMP_NUM_THREADS=$n srun -n 1 ./shwater2d
end
