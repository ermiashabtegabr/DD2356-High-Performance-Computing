for t in 1 2 4 8 12 16 20 24 28 32
do
	export OMP_NUM_THREADS=$t

	sum=0
	for i in {1..5}
	do
		exec_time=`srun ./DFTW_1 | grep seconds | awk '{print $4}'`
		sum=`echo $sum $exec_time | awk '{print $1 + $2}'`
		echo "$i - $exec_time"
	done

	average=`echo $sum | awk '{print $1 / 5.0}'`
	echo "Average for $t - $average"
done
