#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "omp.h"
#include <cublasXt.h>
#include <cuda.h>
#include <cuda_runtime_api.h>

extern void mm_cpu(const double *A,const double *B,double *C,int hA,int wA,int wB);
extern void mm_omp(const double *A,const double *B,double *C,int hA,int wA,int wB);
extern void mm_gpu(const double *A,const double *B,double *C,int hA,int wA,int wB,int tile,int devices);
extern void check_result(double *Ca,double *Cb,int hA,int wA,int wB,double e);
extern void print_matrix(const double *A,const double *B,const double *C,int hA,int wA,int wB);

int main(int argc,char *argv[])
{
   int SIZE=atoi(argv[1]);
   int tile = atoi(argv[2]);   
   int devices = atoi(argv[3]);   
   int hA=SIZE;
   int wA=SIZE;
   int wB=SIZE;

   double *A=(double *)malloc(sizeof(double)*SIZE*SIZE);
   double *B=(double *)malloc(sizeof(double)*SIZE*SIZE);
   double *CC=(double *)malloc(sizeof(double)*SIZE*SIZE);
   double *OC=(double *)malloc(sizeof(double)*SIZE*SIZE);
   double *GC=(double *)malloc(sizeof(double)*SIZE*SIZE);

   int i,j,k;
   double sum=0.0;
   double cpu_start,cpu_end,cpu_wtime; 
   double omp_start,omp_end,omp_wtime; 
   double gpu_start,gpu_end,gpu_wtime;
   
   for(i=0;i<hA;i++)
     for(j=0;j<wA;j++)
       A[i*wA+j]=(double)(rand()%SIZE);
 
   for(i=0;i<hA;i++)
     for(j=0;j<wB;j++)
       B[i*wB+j]=(double)(rand()%SIZE);

   for(i=0;i<hA;i++)
     for(j=0;j<wB;j++)
     {
      CC[i*wA+j]=0.0;
      OC[i*wA+j]=0.0;
      GC[i*wA+j]=0.0;
     }

   cpu_start=omp_get_wtime();
//   mm_cpu(A,B,CC,hA,wA,wB);
   cpu_end=omp_get_wtime();
   cpu_wtime=cpu_end-cpu_start;
   printf("CPU time = %.3f\n",cpu_wtime);

   omp_start=omp_get_wtime();
   mm_omp(A,B,OC,hA,wA,wB);
   omp_end=omp_get_wtime();
   omp_wtime=omp_end-omp_start;
//   check_result(CC,OC,hA,wA,wB,1e-05);
   printf("OMP time = %.3f(%3f times speedup)\n",omp_wtime,(cpu_wtime/omp_wtime));

   gpu_start=omp_get_wtime();
   mm_gpu(A,B,GC,hA,wA,wB,tile,devices);
   gpu_end=omp_get_wtime();
   gpu_wtime=gpu_end-gpu_start;
   check_result(OC,GC,hA,wA,wB,1e-05);
   printf("GPU time = %.3f(%3f times speedup)\n",gpu_wtime,(omp_wtime/gpu_wtime));

}

void mm_cpu(const double *A,const double *B,double *C,int hA,int wA,int wB)
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
//print_matrix(A,B,C,hA,wA,wB);
}

void mm_omp(const double *A,const double *B,double *C,int hA,int wA,int wB)
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
//print_matrix(A,B,C,hA,wA,wB);
}

void mm_gpu(const double *A,const double *B,double *C,int hA,int wA,int wB,int tile,int num_of_devices)
{
 cublasXtHandle_t handle;
 cublasStatus_t status;
 const double alpha=1.0;
 const double beta=0.0;
 int devices[num_of_devices];
 int cub=tile;
 int i,j,k;
 
 for(i=0;i<num_of_devices;i++)
   devices[i]=i;
 printf("device %d \n",num_of_devices);

 cublasXtCreate(&handle);
 cublasXtDeviceSelect(handle, num_of_devices, devices);

 cublasXtSetBlockDim(handle, cub);

 status = cublasXtDgemm(handle,
                        CUBLAS_OP_N,CUBLAS_OP_N,
                        hA,wB,wA,
                        &alpha,
                        B,wA,
                        A,wB,
                        &beta,
                        C,wB);
 cudaDeviceSynchronize();
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
//print_matrix(A,B,C,hA,wA,wB);
}

void print_matrix(const double *A,const double *B,const double *C,int hA,int wA,int wB)
{
   int i,j;
   printf("hA=%d,wA=%d,wB=%d\n",hA,wA,wB);
   for(i=0;i<wA;i++)
   {
     printf("|");
     for(j=0;j<wB;j++)
       printf("A[%3d]=%3.2f",i*wA+j,A[i*wA+j]);
     printf("| |");
     for(j=0;j<wB;j++)
       printf("B[%3d]=%3.2f",i*wB+j,B[i*wB+j]);
     printf("| |");
     for(j=0;j<wB;j++)
       printf("C[%3d]=%3.2f",i*wA+j,C[i*wA+j]);
    printf("|\n");
   }


}

void check_result(double *Ca,double *Cb,int hA,int wA,int wB,double e)
{
  int i,j;
  for(i=0;i<hA;i++)
   for(j=0;j<wB;j++)
   {
     if(fabs(fabs(*(Ca+wA*i+j))-fabs(*(Cb+wA*i+j)))>e)
     {
        printf("error\n");
        return;
     }
  }
}
