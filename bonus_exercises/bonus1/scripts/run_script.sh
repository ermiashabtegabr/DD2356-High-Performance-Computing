num=(500 1000 2000)
for i in ${num[@]}
do
	echo `srun -n 1 ./n_body_simple $i 0.05 100` 
done


