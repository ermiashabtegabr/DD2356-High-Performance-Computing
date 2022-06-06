num=(4 9 16 36 64 81 144)
for i in ${num[@]}
do
	echo `srun -n $i $1` 
done
