#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc,char *argv[])
{

MPI_Init(&argc,&argv);

int *c1=(int *)malloc(sizeof(int)*8);
int *c2=(int *)malloc(sizeof(int)*8);
int *sizes=(int *)malloc(sizeof(int)*3);
int *strides=(int *)malloc(sizeof(int)*3);
int *recvpositions=(int *)malloc(sizeof(int)*3);
int *recvcounts=(int *)malloc(sizeof(int)*3);
int i,p,n;

MPI_Comm_rank(MPI_COMM_WORLD,&n);
MPI_Comm_size(MPI_COMM_WORLD,&p);

for(i=0;i<8;i++)
 *(c1+i)=i+8*n;

printf("rank=%d ",n);
for(i=0;i<8;i++)
 printf("%3d ",*(c1+i));
printf("\n");

MPI_Barrier(MPI_COMM_WORLD);

if(n==0)
{
*(sizes+0)=6;
*(sizes+1)=2;
*(strides+0)=0;
*(strides+1)=6;
*(recvpositions+0)=0;
*(recvpositions+1)=6;
*(recvcounts+0)=6;
*(recvcounts+1)=2;
}

if(n==1)
{
*(sizes+0)=2;
*(sizes+1)=6;
*(strides+0)=0;
*(strides+1)=2;
*(recvpositions+0)=0;
*(recvpositions+1)=2;
*(recvcounts+0)=2;
*(recvcounts+1)=6;
}

for(i=0;i<2;i++)
MPI_Scatterv(c1,sizes,strides,MPI_INT,c2+(*(recvpositions+i)),*(recvcounts+i),MPI_INT,i,MPI_COMM_WORLD);

if(n==0)
{
for(i=0;i<8;i++)
 printf("%3d ",*(c2+i));
printf("\n");
}
}
