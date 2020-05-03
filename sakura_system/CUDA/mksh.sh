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

for OUT in cudatest_0.sh cudatest_1.sh cudatest_2.sh cudatest_3.sh mm_cublasxt.sh;do
echo \#!/bin/bash > $OUT
N=${OUT/acctest_/}
N=${N/.sh/.out}

case $OUT in
 mm_cudablasxt.sh ) echo \#PBS -N MM_CUDABLASXT.$N >> $OUT;;
 cudatest_0.sh ) echo \#PBS -N CUDATEST.$N >>$OUT;;
 cudatest_1.sh ) echo \#PBS -N CUDATEST.$N >>$OUT;;
 cudatest_2.sh ) echo \#PBS -N CUDATEST.$N >>$OUT;;
 cudatest_3.sh ) echo \#PBS -N CUDATEST.$N >>$OUT;;
esac

echo \#PBS -q large >> $OUT
echo \#PBS -l walltime=5:49:00 >> $OUT
echo \#PBS -j oe >> $OUT
echo \#PBS -l select=${1}:ncpus=${2}:mpiprocs=${3}:ompthreads=${4} >> $OUT
echo " " >> $OUT
echo cd \$PBS_O_WORKDIR >> $OUT
echo module purge >> $OUT

case $OUT in
 * ) echo module load cuda/10.0 >> $OUT ;;
esac 

echo _NPROCS=\`cat \$PBS_NODEFILE \| wc -l\` >> $OUT
echo OMP_NUM_THREADS=${4} >> $OUT
echo echo \"./${OUT/.sh/.out} \" \$\{SIZE\} \$\{_NPROCS\} \${OMP_NUM_THREADS} \${TILE} \${DEVICES}>> $OUT 

case  $OUT in
 mm_cudablasxt.sh ) echo ./${OUT/.sh/.out}  \$\{SIZE\} \$\{TILE\} \$\{DEVICES\} >> $OUT ;;
 cudatest_1.sh ) echo ./${OUT/.sh/.out}  \$\{SIZE\} \$\{TILE\} \$\{DEVICES\} >> $OUT ;;
 cudatest_2.sh ) echo ./${OUT/.sh/.out}  \$\{SIZE\} \$\{TILE\} \$\{DEVICES\} >> $OUT ;;
 cudatest_3.sh ) echo ./${OUT/.sh/.out}  \$\{SIZE\} \$\{TILE\} \$\{DEVICES\} >> $OUT ;;
 * ) echo ./${OUT/.sh/.out}  \$\{SIZE\} \$\{TILE\} \$\{DEVICES\} >> $OUT ;;
# *) echo mpirun -n \$_NPROCS -machinefile \$PBS_NODEFILE ./${OUT/.sh/.out} \$\{SIZE\} >> $OUT ;; 
esac

done
