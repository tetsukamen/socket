#!/bin/sh

# 書き込みファイルのリセット
: > eval_cpu.txt
: > eval_vmhwm.txt

# 100回計測して記録
for i in `seq 100`
do
  ./server.o &
  sleep 0.1
  ./client.o
  sleep 0.1
done

# 平均の計算
echo "\n"
echo "=========結果========="
# cpu
cpu_sum=0
cpu_count=0
while read value
do
  cpu_sum=$(($cpu_sum+$value))
  cpu_count=$(($cpu_count+1))
done < ./eval_cpu.txt
echo cpu_time_avg: $(($cpu_sum/$cpu_count))
# maxrss
maxrss_sum=0
maxrss_count=0
while read value
do
  maxrss_sum=$(($maxrss_sum+$value))
  maxrss_count=$(($maxrss_count+1))
done < ./eval_vmhwm.txt
echo maxrss_avg: $(($maxrss_sum/$maxrss_count))


