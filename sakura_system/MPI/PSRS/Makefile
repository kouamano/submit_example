CC=mpicc
ICC=mpiicc
GCC=gcc -O3
MODULE=/usr/bin/modulecmd bash
 
all: psrs_intel_2019update1.out psrs_intelmpi_2019update1.out test_intelmpi_2019update1.out psrs_mvapich2_gnu.out psrs_openmpi_pgi1810.out psrs_openmpi_gnu.out psrs_mpich3_gnu.out sort_seq.out
	$(./mksh.sh)

psrs_intel_2019update1.out: psrs.c
	eval `${MODULE} purge`; eval `$(MODULE) load intel/2019update1`;  ${ICC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

test_intelmpi_2019update1.out: test.c
	eval `${MODULE} purge`; eval `$(MODULE) load intel/2019update1 intelmpi/2019update1`; ${ICC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

psrs_intelmpi_2019update1.out: psrs.c
	eval `${MODULE} purge`; eval `$(MODULE) load intel/2019update1 intelmpi/2019update1`; ${ICC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

psrs_mvapich2_gnu.out: psrs.c
	eval `${MODULE} purge`; eval `$(MODULE) load mvapich2/gnu`; ${CC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

psrs_openmpi_pgi1810.out: psrs.c
	eval `${MODULE} purge`; eval `$(MODULE) load pgi openmpi/pgi1810`; ${CC} $^ -o $@; eval `${MODULE} purge`;

psrs_openmpi_gnu.out: psrs.c
	eval `${MODULE} purge`; eval `$(MODULE) load openmpi/gnu`; ${CC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

psrs_mpich3_gnu.out: psrs.c
	eval `${MODULE} purge`; eval `$(MODULE) load mpich3/gnu`; ${CC} -fopenmp $^ -o $@; eval `${MODULE} purge`;

sort_seq.out: sort_seq.c
	${GCC} $^ -o $@

clean: 
	@rm *.out test_*.sh psrs_*.sh sort_seq.sh
