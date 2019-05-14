.PHONY: build

CXX=gcc

build:
	mkdir -p build
	$(CXX) src/demo.cpp -o build/demo --std=c++11 -lstdc++
	$(CXX) src/benchmark.cpp -o build/benchmark --std=c++11 -lstdc++

rundemo:
	build/demo

runbenchmark:
	mkdir -p benchmark
	taskset -c 0 nice -n 19 build/benchmark

plot:
	mkdir fig -p
	dot fig/demo.dot -Teps > fig/demo.eps
	python3 plot.py
