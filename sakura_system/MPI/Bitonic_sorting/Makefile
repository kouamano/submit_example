CC=mpicc
ICC=mpiicc
GCC=gcc -O3
MODULE=/usr/bin/modulecmd bash
 
all: bitonic_intel_2019update1.out bitonic_intelmpi_2019update1.out bitonic_mvapich2_gnu.out bitonic_openmpi_pgi1810.out bitonic_openmpi_gnu.out bitonic_mpich3_gnu.out sort_seq.out
	$(./mksh.sh)

bitonic_intel_2019update1.out: bitonic.c
	eval `${MODULE} purge`; eval `$(MODULE) load intel/2019update1`;  ${CC} -fopenmp $^ -o $@ -lm ; eval `${MODULE} purge`;

bitonic_intelmpi_2019update1.out: bitonic.c
	eval `${MODULE} purge`; eval `$(MODULE) load intel/2019update1 intelmpi/2019update1`; ${ICC} -fopenmp $^ -o $@ -lm; eval `${MODULE} purge`;

bitonic_mvapich2_gnu.out: bitonic.c
	eval `${MODULE} purge`; eval `$(MODULE) load mvapich2/gnu`; ${CC} -fopenmp $^ -o $@ -lm; eval `${MODULE} purge`;

bitonic_openmpi_pgi1810.out: bitonic.c
	eval `${MODULE} purge`; eval `$(MODULE) load pgi openmpi/pgi1810`; ${CC} $^ -o $@ -lm; eval `${MODULE} purge`;

bitonic_openmpi_gnu.out: bitonic.c
	eval `${MODULE} purge`; eval `$(MODULE) load openmpi/gnu`; ${CC} -fopenmp $^ -o $@ -lm; eval `${MODULE} purge`;

bitonic_mpich3_gnu.out: bitonic.c
	eval `${MODULE} purge`; eval `$(MODULE) load mpich3/gnu`; ${CC} -fopenmp $^ -o $@ -lm; eval `${MODULE} purge`;

sort_seq.out: sort_seq.c
	${GCC} $^ -o $@ -lm

clean: 
	@rm *.out bitonic_*.sh sort_seq.sh
