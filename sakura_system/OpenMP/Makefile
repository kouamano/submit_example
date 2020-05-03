CC=mpicc
GCC=gcc -O5
MODULE=/usr/bin/modulecmd bash
 
all: omptest_intel_2019update1.out omptest_intelmpi_2019update1.out omptest_mvapich2_gnu.out omptest_openmpi_pgi1810.out omptest_openmpi_gnu.out omptest_mpich3_gnu.out omptest.out

omptest_intel_2019update1.out: omptest.c
	eval `${MODULE} purge`; eval `$(MODULE) load intel/2019update1`;  ${CC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

omptest_intelmpi_2019update1.out: omptest.c
	eval `${MODULE} purge`; eval `$(MODULE) load intelmpi/2019update1`; ${CC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

omptest_mvapich2_gnu.out: omptest.c
	eval `${MODULE} purge`; eval `$(MODULE) load mvapich2/gnu`; ${CC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

omptest_openmpi_pgi1810.out: omptest.c
	eval `${MODULE} purge`; eval `$(MODULE) load pgi openmpi/pgi1810`; ${CC} $^ -o $@; eval `${MODULE} purge`;

omptest_openmpi_gnu.out: omptest.c
	eval `${MODULE} purge`; eval `$(MODULE) load openmpi/gnu`; ${CC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

omptest_mpich3_gnu.out: omptest.c
	eval `${MODULE} purge`; eval `$(MODULE) load mpich3/gnu`; ${CC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

omptest.out: omptest.c
	eval `${MODULE} purge`; ${GCC} -fopenmp $^ -o $@;eval `${MODULE} purge`;

clean: 
	-@rm *.out omptest_*.sh omptest.sh
