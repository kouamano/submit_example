#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

int main(int argc,char *argv[])
{
int N=atoi(argv[1]);
int i,j,k;
int *a=(int *)malloc(sizeof(int)*N);
int *b=(int *)malloc(sizeof(int)*N);
int *c=(int *)malloc(sizeof(int)*N);
int *as=(int *)malloc(sizeof(int)*N);
int *bs=(int *)malloc(sizeof(int)*N);
int *cs=(int *)malloc(sizeof(int)*N);
int seq_start,seq_end,seq_time;
int cuda_start,cuda_end,cuda_time;

for(i=0;i<N;i++)
   b[i]=rand()%N;
for(i=0;i<N;i++)
   c[i]=rand()%N;

for(i=0;i<N;i++)
   bs[i]=b[i];
for(i=0;i<N;i++)
   cs[i]=c[i];


//seq_start=clock64();
   for(i=0;i<N;i++)
     a[i]=b[i]+c[i];
//seq_end=clock64();

//seq_time=seq_end-seq_start;

//printf("seq time=%d\n",seq_time);

//cuda_start=clock64();
//cuda_end=clock64();
//cuda_time=cuda_end-cuda_start;

//printf("cuda time=%d\n",cuda_time);

}
