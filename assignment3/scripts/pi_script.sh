num=(8 16 32 64 128)
for i in ${num[@]}
do
	echo `srun -n $i $1` 
done

