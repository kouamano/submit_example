#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
//#include "mpi.h"


int main(int argc,char * argv[])
{
 int n=atoi(argv[1]);
 int pe,rank;
 int *a=(int *)malloc(sizeof(int)*n*n);
 int *b=(int *)malloc(sizeof(int)*n*n);
 int *c=(int *)malloc(sizeof(int)*n*n);
 int *as=(int *)malloc(sizeof(int)*n*n);
 int *bs=(int *)malloc(sizeof(int)*n*n);
 int *cs=(int *)malloc(sizeof(int)*n*n);
 int *ap=(int *)malloc(sizeof(int)*n*n);
 int *bp=(int *)malloc(sizeof(int)*n*n);
 int *cp=(int *)malloc(sizeof(int)*n*n);
 int i,j,k;
 double seq_time,seq_start,seq_end;
 double acc_time,acc_start,acc_end;

#pragma omp parallel
{
if(omp_get_thread_num()==0)
printf("OMP_NUM_THREADS=%d\n",omp_get_num_threads()); 
}
// MPI_Init(&argc,&argv);
 for(i=0;i<n;i++)
   for(j=0;j<n;j++)
   {
      a[i*n+j]=0;
      b[i*n+j]=rand()%n;
      c[i*n+j]=rand()%n;
   }
 for(i=0;i<n;i++)
   for(j=0;j<n;j++)
   {
      as[i*n+j]=a[i*n+j];
      bs[i*n+j]=b[i*n+j];
      cs[i*n+j]=c[i*n+j];
   }
 seq_start = omp_get_wtime();
 for(i=0;i<n;i++)
   for(j=0;j<n;j++)
    for(k=0;k<n;k++)
    {
      as[i*n+j]+=bs[i*n+k]*cs[k*n+j];
    }
 seq_end = omp_get_wtime();

 seq_time=seq_end-seq_start;

 printf("Seq elapsed_time=%.3f\n",seq_time);
 for(i=0;i<n;i++)
   for(j=0;j<n;j++)
   {
      ap[i*n+j]=a[i*n+j];
      bp[i*n+j]=b[i*n+j];
      cp[i*n+j]=c[i*n+j];
   }
 
 acc_start = omp_get_wtime();
#pragma acc data copy(ap[0:n*n]) copyin(bp[0:n*n],cp[0:n*n],n)
{
#pragma acc parallel
{
#pragma acc loop private(i,j,k) independent gangs workers vectors
 for(i=0;i<n;i++)
   for(j=0;j<n;j++)
    for(k=0;k<n;k++)
      ap[i*n+j]+=bp[i*n+k]*cp[k*n+j];
}
}
 acc_end = omp_get_wtime();

 acc_time=acc_end-acc_start;

 printf("Parallel elapsed_time=%.3f\n",acc_time);
 for(i=0;i<n;i++)
   for(j=0;j<n;j++)
    if(as[i*n+j]!=ap[i*n+j])
    printf("a[%d][%d] failed\n",i,j);

  printf("finished!! %.3f times speedup.\n",(seq_time/acc_time));
// MPI_Finalize();
exit(0);
}
