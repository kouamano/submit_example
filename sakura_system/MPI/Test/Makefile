CC=mpicc
MODULE=/usr/bin/modulecmd bash
 
all: mpitest_intel_2019update1.out mpitest_intelmpi_2019update1.out mpitest_mvapich2_gnu.out mpitest_openmpi_pgi1810.out mpitest_openmpi_gnu.out mpitest_mpich3_gnu.out

mpitest_intel_2019update1.out: mpitest.c
	eval `${MODULE} purge`; eval `$(MODULE) load intel/2019update1`;  ${CC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

mpitest_intelmpi_2019update1.out: mpitest.c
	eval `${MODULE} purge`; eval `$(MODULE) load intelmpi/2019update1`; ${CC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

mpitest_mvapich2_gnu.out: mpitest.c
	eval `${MODULE} purge`; eval `$(MODULE) load mvapich2/gnu`; ${CC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

mpitest_openmpi_pgi1810.out: mpitest.c
	eval `${MODULE} purge`; eval `$(MODULE) load pgi openmpi/pgi1810`; ${CC} $^ -o $@; eval `${MODULE} purge`;

mpitest_openmpi_gnu.out: mpitest.c
	eval `${MODULE} purge`; eval `$(MODULE) load openmpi/gnu`; ${CC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

mpitest_mpich3_gnu.out: mpitest.c
	eval `${MODULE} purge`; eval `$(MODULE) load mpich3/gnu`; ${CC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

clean: 
	-@rm *.out mpitest_*.sh
