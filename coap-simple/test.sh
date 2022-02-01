cpu_sum=0
for i in `seq 10`
do
  cpu_sum=$(($cpu_sum+1))
done
echo $cpu_sum