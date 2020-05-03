#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "omp.h"
#include <cublasXt.h>
#include <cuda_runtime.h>
#include "cublas_v2.h"

extern void mm_cpu(double *A,double *B,double *C,int hA,int wA,int wB);
extern void mm_omp(double *A,double *B,double *C,int hA,int wA,int wB);
extern void mm_gpu(double *A,double *B,double *C,int hA,int wA,int wB);
extern void check_result(double *Ca,double *Cb,int hA,int wA,int wB,double e);

int main(int argc,char *argv[])
{
   int  SIZE=atoi(argv[1]);
   
   int hA=SIZE;
   int wA=SIZE;
   int wB=SIZE;

   double *A=(double *)malloc(sizeof(double)*SIZE*SIZE);
   double *B=(double *)malloc(sizeof(double)*SIZE*SIZE);
   double *C=(double *)malloc(sizeof(double)*SIZE*SIZE);
   double *OC=(double *)malloc(sizeof(double)*SIZE*SIZE);
   double *GC=(double *)malloc(sizeof(double)*SIZE*SIZE);

   int i,j,k;
   double sum=0;
   double cpu_start,cpu_end,cpu_wtime; 
   double omp_start,omp_end,omp_wtime; 
   double gpu_start,gpu_end,gpu_wtime;
   
   for(i=0;i<hA;i++)
     for(j=0;j<wA;j++)
       A[i*wA+j]=(double)(rand()%SIZE);
 
   for(i=0;i<wA;i++)
     for(j=0;j<wB;j++)
       B[i*wB+j]=(double)(rand()%SIZE);

   for(i=0;i<wA;i++)
     for(j=0;j<wB;j++)
     {
       C[i*wB+j]=0.0;
      OC[i*wB+j]=0.0;
      GC[i*wB+j]=0.0;
     }

   for(i=0;i<wA;i++)
   {
     printf("|");
     for(j=0;j<wB;j++)
       printf("A[%d]=%.2f ",i*wB+j,A[i*wB+j]);
     printf("| |");
     for(j=0;j<wB;j++)
       printf("B[%d]=%.2f ",i*wB+j,B[i*wB+j]);
    printf("|\n");
   }


   cpu_start=omp_get_wtime();
   mm_cpu(A,B,C,SIZE,SIZE,SIZE);
   cpu_end=omp_get_wtime();
   cpu_wtime=cpu_end-cpu_start;
   printf("CPU time = %.3f\n",cpu_wtime);

   omp_start=omp_get_wtime();
   mm_omp(A,B,OC,SIZE,SIZE,SIZE);
   omp_end=omp_get_wtime();
   omp_wtime=omp_end-omp_start;
   check_result(C,OC,SIZE,SIZE,SIZE,1e-05);
   printf("OMP time = %.3f(%3f times speedup)\n",omp_wtime,(cpu_wtime/omp_wtime));

   gpu_start=omp_get_wtime();
   mm_gpu(A,B,GC,SIZE,SIZE,SIZE);
   gpu_end=omp_get_wtime();
   gpu_wtime=gpu_end-gpu_start;
   check_result(C,GC,SIZE,SIZE,SIZE,1e-05);
   printf("GPU time = %.3f(%3f times speedup)\n",gpu_wtime,(cpu_wtime/gpu_wtime));

}

void mm_cpu(double *A,double *B,double *C,int hA,int wA,int wB)
{
 int i,j,k;
 double sum=0.0;
 for(i=0;i<hA;i++)
 {
   for(j=0;j<wB;j++)
   {
     sum=0.0;
     for(k=0;k<wA;k++)
       sum+=A[i*wA+k]*B[k*wB+j];
     C[i*wB+j]=sum;
   }
  }
}

void mm_omp(double *A,double *B,double *C,int hA,int wA,int wB)
{
 int i,j,k;
 double sum=0.0;
#pragma omp parallel for private(i,j,k,sum) firstprivate(hA,wA,wB) shared(A,B,C)
 for(i=0;i<hA;i++)
 {
   for(j=0;j<wB;j++)
   {
     sum=0.0;
     for(k=0;k<wA;k++)
       sum+=A[i*wA+k]*B[k*wB+j];
     C[i*wB+j]=sum;
   }
  }
}

void mm_gpu(double *A,double *B,double *C,int hA,int wA,int wB)
{
 cublasXtHandle_t handle;
 cublasStatus_t status;
 double alpha=1.0;
 double beta=0.0;
 cublasXtCreate(&handle);

 int devices[1] = {0};
 cublasXtDeviceSelect(handle, 1, devices);
 int cub=hA;
 cublasXtSetBlockDim(handle, cub);

 int i,j;
   for(i=0;i<wA;i++)
   {
     printf("|");
     for(j=0;j<wB;j++)
       printf("A[%d]=%.2f ",i*wB+j,A[i*wB+j]);
     printf("| |");
     for(j=0;j<wB;j++)
       printf("B[%d]=%.2f ",i*wB+j,B[i*wB+j]);
     printf("| |");
     for(j=0;j<wB;j++)
       printf("C[%d]=%.2f ",i*wB+j,C[i*wB+j]);
    printf("|\n");
   }
   for(i=0;i<wA;i++)
     for(j=0;j<wB;j++)
       C[i*wB+j]=0.0;

 status = cublasXtDgemm(handle,CUBLAS_OP_N,CUBLAS_OP_N,hA,wB,wA,&alpha,A,hA,B,wB,&beta,C,wA);
 printf("status =%d\n",status);
 switch(status)
 {
  case CUBLAS_STATUS_SUCCESS:
      printf("SUCCESS\n");
      break;
  case CUBLAS_STATUS_NOT_INITIALIZED:
      printf("NOT INITIALIZED\n");
      break;
  case CUBLAS_STATUS_INVALID_VALUE:
      printf("INVALID VALUE\n");
      break;
  case CUBLAS_STATUS_ARCH_MISMATCH:
      printf("ARCH MISMATCH\n");
      break;
  case CUBLAS_STATUS_EXECUTION_FAILED:
      printf("EXECUTION FAILED\n");
      break;
 }
status = cublasXtDestroy(handle);

printf("hA=%d,wA=%d,wB=%d\n",hA,wA,wB);
   for(i=0;i<wA;i++)
   {
     printf("|");
     for(j=0;j<wB;j++)
       printf("A[%d]=%.2f ",i*wB+j,A[i*wB+j]);
     printf("| |");
     for(j=0;j<wB;j++)
       printf("B[%d]=%.2f ",i*wB+j,B[i*wB+j]);
     printf("| |");
     for(j=0;j<wB;j++)
       printf("C[%d]=%.2f ",i*wB+j,C[i*wB+j]);
    printf("|\n");
   }
}

void check_result(double *Ca,double *Cb,int hA,int wA,int wB,double e)
{
  int i,j;
  for(i=0;i<hA;i++)
   for(j=0;j<wB;j++)
   {
     printf("Ca=%lf,Cb=%lf\n",(*(Ca+wA*i+j)),(*(Cb+wA*i+j)));
/*
     if(fabs(fabs(*(Ca+wA*i+j))-fabs(*(Cb+wA*i+j)))>e)
     {
        printf("error\n");
        return;
     }
*/
  }
}
