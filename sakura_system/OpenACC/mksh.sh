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

for OUT in acctest.sh sample.sh ;do
echo \#!/bin/bash > $OUT
N=${OUT/acctest_/}
N=${N/.sh/.out}
echo \#PBS -N ACCTEST.$N >> $OUT
echo \#PBS -q large >> $OUT
echo \#PBS -l walltime=5:49:00 >> $OUT
echo \#PBS -j oe >> $OUT
echo \#PBS -l select=${1}:ncpus=${2}:mpiprocs=${3}:ompthreads=${4} >> $OUT
echo " " >> $OUT
echo cd \$PBS_O_WORKDIR >> $OUT
echo module purge >> $OUT
case $OUT in
 "acctest.sh" ) echo module load pgi/18.10 >> $OUT ;;
 "sample.sh" ) echo module load pgi/18.10 >> $OUT ;;
esac 
echo _NPROCS=\`cat \$PBS_NODEFILE \| wc -l\` >> $OUT
echo OMP_NUM_THREADS=${4} >> $OUT
echo echo \"mpirun ./${OUT/.sh/.out} \" \$\{SIZE\} \$\{_NPROCS\} \${OMP_NUM_THREADS} >> $OUT 
case  $OUT in
 "acctest.sh") echo ./${OUT/.sh/.out}  \$\{SIZE\}>> $OUT ;;
 "sample.sh") echo ./${OUT/.sh/.out}  \$\{SIZE\}>> $OUT ;;
 *) echo mpirun -n \$_NPROCS -machinefile \$PBS_NODEFILE ./${OUT/.sh/.out} \$\{SIZE\} >> $OUT ;; 
esac
done
