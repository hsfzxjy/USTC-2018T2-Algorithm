.PHONY: build
build:
	mkdir build -p
	mkdir benchmarks -p
	mkdir trees -p
	g++ src/demo.cpp -o build/demo --std=c++11
	g++ src/benchmark_insert.cpp -o build/benchmark_insert --std=c++11
	g++ src/benchmark_create.cpp -o build/benchmark_create --std=c++11
	g++ src/benchmark_search.cpp -o build/benchmark_search --std=c++11
	g++ src/benchmark_ostree.cpp -o build/benchmark_ostree --std=c++11
plot:
	mkdir figs -p
	dot trees/ostree.dot -Teps > figs/ostree.eps
	dot trees/rbtree.dot -Teps > figs/rbtree.eps
	dot trees/big.dot -Teps > figs/big.eps
	python3 plot.py benchmarks/insert.dat figs/insert.png
	python3 plot.py benchmarks/search.dat figs/search.png
	python3 plot.py benchmarks/create.dat figs/create.png
	python3 plot.py benchmarks/max.dat figs/max.png
	python3 plot.py benchmarks/min.dat figs/min.png
	python3 plot.py benchmarks/succ.dat figs/succ.png
	python3 plot.py benchmarks/pred.dat figs/pred.png
	python3 plot.py benchmarks/rank.dat figs/rank.png
	python3 plot.py benchmarks/select.dat figs/select.png
bundle:
	zip PB16010580_谢璟毅_Code2.zip *pdf Makefile src figs benchmarks build trees plot.py -r -9
