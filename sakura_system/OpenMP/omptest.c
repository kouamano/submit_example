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
 double seq_elapsed,seq_start,seq_end;
 double omp_elapsed,omp_start,omp_end;

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

 seq_elapsed=seq_end-seq_start;

 printf("Seq elapsed_time=%.3f\n",seq_elapsed);
 for(i=0;i<n;i++)
   for(j=0;j<n;j++)
   {
      ap[i*n+j]=a[i*n+j];
      bp[i*n+j]=b[i*n+j];
      cp[i*n+j]=c[i*n+j];
   }
 
 omp_start = omp_get_wtime();
#pragma omp parallel for private(i,j,k) shared(ap,bp,cp)
 for(i=0;i<n;i++)
   for(j=0;j<n;j++)
    for(k=0;k<n;k++)
      ap[i*n+j]+=bp[i*n+k]*cp[k*n+j];
 omp_end = omp_get_wtime();

 omp_elapsed=omp_end-omp_start;

 printf("Parallel elapsed_time=%.3f\n",omp_elapsed);

 printf("%.3f times speedup\n",(seq_elapsed/omp_elapsed));

 for(i=0;i<n;i++)
   for(j=0;j<n;j++)
    if(as[i*n+j]!=ap[i*n+j])
    printf("a[%d][%d] failed\n",i,j);

  printf("finish!!\n");
// MPI_Finalize();

}
