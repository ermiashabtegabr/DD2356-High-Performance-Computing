export OMP_SCHEDULE=$1
export OMP_NUM_THREADS=32

sum=0
for i in {0..5}
do
	bandwidth=`srun ./stream_omp | grep Copy: | awk '{print $2}'`
	sum=`echo $sum $bandwidth | awk '{print $1 + $2}'`
done
average=`echo $sum | awk '{print $1 / 5.0}'`
echo "Average for $OMP_SCHEDULE - $average"
