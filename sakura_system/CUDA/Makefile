CC=nvcc  -Xcompiler -fopenmp 
CC_OPT= -lm -lcublas 
MODULE=/usr/bin/modulecmd bash
 
all:cudatest_0.out cudatest_1.out cudatest_2.out cudatest_3.cpp mm_cublasxt.out

cudatest_0.out: cudatest_0.cpp
	eval `${MODULE} purge`;eval `${MODULE} load cuda/10.0`; ${CC} $^ -o $@ ${CC_OPT};eval `${MODULE} purge`;

cudatest_1.out: cudatest_1.cpp
	eval `${MODULE} purge`;eval `${MODULE} load cuda/10.0`; ${CC} $^ -o $@ ${CC_OPT};eval `${MODULE} purge`;

cudatest_2.out: cudatest_2.cpp
	eval `${MODULE} purge`;eval `${MODULE} load cuda/10.0`; ${CC} $^ -o $@ ${CC_OPT};eval `${MODULE} purge`;

cudatest_3.out: cudatest_3.cpp
	eval `${MODULE} purge`;eval `${MODULE} load cuda/10.0`; ${CC} $^ -o $@ ${CC_OPT};eval `${MODULE} purge`;

mm_cublasxt.out: mm_cublasxt.cpp
	eval `${MODULE} purge`;eval `${MODULE} load cuda/10.0`; ${CC} $^ -o $@ ${CC_OPT};eval `${MODULE} purge`;

clean: 
	-@rm *.out cudatest_*.sh
