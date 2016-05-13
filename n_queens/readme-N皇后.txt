readme-N皇后
实现算法为爬山算法、模拟退火算法，参考了queen search 4算法的实现，时间复杂度为O(n)
编译(OS X El Captain编译通过)：
g++ -O3 -o sa simulated_annealing.cpp
g++ -O3 -o hc hill_climbing.cpp
执行：
./sa
./hc
输入文件为input.txt
输出文件分别为 output_simulated_annealing.txt 、output_hill_climbing.txt