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

for OUT in omptest_intel_2019update1.sh omptest_intelmpi_2019update1.sh omptest_mvapich2_gnu.sh omptest_openmpi_pgi1810.sh omptest_openmpi_gnu.sh omptest_mpich3_gnu.sh omptest.sh ;do
echo \#!/bin/bash > $OUT
N=${OUT/omptest_/}
N=${N/.sh/.out}
echo \#PBS -N OMPTEST.$N >> $OUT
echo \#PBS -q large >> $OUT
echo \#PBS -l walltime=5:49:00 >> $OUT
echo \#PBS -j oe >> $OUT
echo \#PBS -l select=${1}:ncpus=${2}:mpiprocs=${3}:ompthreads=${4} >> $OUT
echo " " >> $OUT
echo cd \$PBS_O_WORKDIR >> $OUT
echo module purge >> $OUT
case $OUT in
 "omptest_intel_2019update1.sh" ) echo module load intel/2019update1 >> $OUT ;;
 "omptest_intelmpi_2019update1.sh" ) echo module load intel/2019update1 intelmpi/2019update1 >> $OUT ;;
 "omptest_mvapich2_gnu.sh" ) echo module load mvapich2/gnu >> $OUT ;;
 "omptest_openmpi_pgi1810.sh" ) echo module load pgi openmpi/pgi1810 >> $OUT ;;
 "omptest_openmpi_gnu.sh" ) echo module load openmpi/gnu >> $OUT ;;
 "omptest_mpich3_gnu.sh" ) echo module load mpich3/gnu >> $OUT ;;
 "omptest.sh" ) echo " " >> $OUT ;;
esac 
echo _NPROCS=\`cat \$PBS_NODEFILE \| wc -l\` >> $OUT
echo OMP_NUM_THREADS=${4} >> $OUT
echo echo \"mpirun ./${OUT/.sh/.out} \" \$\{SIZE\} \$\{_NPROCS\} \${OMP_NUM_THREADS} >> $OUT 
case  $OUT in
 "omptest.sh") echo ./${OUT/.sh/.out}  \$\{SIZE\}>> $OUT ;;
 *) echo mpirun -n \$_NPROCS -machinefile \$PBS_NODEFILE ./${OUT/.sh/.out} \$\{SIZE\} >> $OUT ;; 
esac
done
