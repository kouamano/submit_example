#!/bin/bash
if [ -z "${1}" ];then
echo Please set select number!!
exit;
fi

if [ -z "${2}" ];then
echo Please set ncpus number!!
exit;
fi

if [ -z "${3}" ];then
echo Please set mpiprocs number!!
exit;
fi

if [ -z "${4}" ];then
echo Please set ompthreads number!!
exit;
fi

for OUT in psrs_intel_2019update1.sh psrs_intelmpi_2019update1.sh test_intelmpi_2019update1.sh  psrs_mvapich2_gnu.sh psrs_openmpi_pgi1810.sh psrs_openmpi_gnu.sh psrs_mpich3_gnu.sh sort_seq.sh ;do
echo \#!/bin/bash > $OUT
N=${OUT/psrs_/}
N=${N/.sh/.out}
echo \#PBS -N PSRS.$N >> $OUT
echo \#PBS -q large >> $OUT
echo \#PBS -l walltime=5:49:00 >> $OUT
echo \#PBS -j oe >> $OUT
echo \#PBS -l select=${1}:ncpus=${2}:mpiprocs=${3}:ompthreads=${4} >> $OUT
echo " " >> $OUT
echo cd \$PBS_O_WORKDIR >> $OUT
echo _NPROCS=\`cat \$PBS_NODEFILE \| wc -l\` >> $OUT
echo module purge >> $OUT
case $OUT in
 "psrs_intel_2019update1.sh" ) echo module load intel/2019update1 >> $OUT ;;
 "psrs_intelmpi_2019update1.sh" ) echo module load intel/2019update1 intelmpi/2019update1 >> $OUT ;;
 "test_intelmpi_2019update1.sh" ) echo module load intel/2019update1 intelmpi/2019update1 >> $OUT ;;
 "psrs_mvapich2_gnu.sh" ) echo module load mvapich2/gnu >> $OUT ;;
 "psrs_openmpi_pgi1810.sh" ) echo module load pgi openmpi/pgi1810 >> $OUT ;;
 "psrs_openmpi_gnu.sh" ) echo module load openmpi/gnu >> $OUT ;;
 "psrs_mpich3_gnu.sh" ) echo module load mpich3/gnu >> $OUT ;;
 "sort_seq.sh" ) echo " " >> $OUT ;;
esac 
echo OMP_NUM_THREADS=${4} >> $OUT
echo echo \"mpirun ./${OUT/.sh/.out} \" \$\{SIZE\} \$\{_NPROCS\} >> $OUT 
case  $OUT in
 "sort_seq.sh" ) echo ./${OUT/.sh/.out}  \$\{SIZE\}>> $OUT ;;
 *) echo mpirun -n \$_NPROCS -machinefile \$PBS_NODEFILE ./${OUT/.sh/.out}  \$\{SIZE\}  >> $OUT ;; 
esac
done
