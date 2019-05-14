.PHONY: build

CXX=g++

build:
	mkdir build -p
	$(CXX) src/matmul.cpp --std=c++11 -o build/matmul -O3
	$(CXX) src/subarray.cpp --std=c++11 -o build/subarray -O2
bm_array1:
	mkdir -p benchmarks
	bm=1 taskset -c 0 nice -n 19 build/subarray
bm_matmul:
	mkdir -p benchmarks
	bm=1 taskset -c 0 nice -n 19 build/matmul
bm_array2:
	mkdir -p benchmarks
	bm=2 taskset -c 0 nice -n 19 build/subarray
plot:
	mkdir -p figs
	python3 plot.py benchmarks/matmul.dat figs/matmul.eps
	python3 plot.py benchmarks/subarray1.dat figs/subarray1.eps
	python3 plot.py benchmarks/subarray2.dat figs/subarray2.eps
bundle:
	zip PB16010580_谢璟毅_Code3.zip src/ build/ benchmarks/ figs/ *pdf Makefile -r -9
