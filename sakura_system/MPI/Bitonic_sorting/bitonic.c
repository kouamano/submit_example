#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mpi.h"

#define ASCE 1
#define DESC 0
#define NONE -1

extern void print_column(char *c,int a[],int n);
extern void compare_min(int m1[],int m2[],int b[],int size);
extern void compare_max(int m1[],int m2[],int b[],int size);
extern void recursion(int n,int m,int N);
extern int pred(int i,int rank);
extern void sorting(int i,int j,int rank,int p,int a[],int b[],int c[],int asce,int local_size);
extern void scan_and_merge_asce(int local_sort[],int bitonic[],int local_size);
extern void scan_and_merge_desc(int local_sort[],int bitonic[],int local_size);
extern void merge_asce(int local_sort[],int n,int local_size);
extern void merge_desc(int local_sort[],int n,int local_size);
extern void quick_sort_desc(int a[],int p,int q);
extern void partition_desc(int a[],int p,int q,int *jp,int *ip);
extern void quick_sort_asce(int a[],int p,int q);
extern void partition_asce(int a[],int p,int q,int *jp,int *ip);
extern void swap(int *u,int *v);
extern void check(int *answer,int *sorted,int n);
extern int pow_2(int n);
extern int log_2(int n);

int main(int argc,char *argv[])
{
  int N=atoi(argv[1]);
  int size=N;
  int local_size;
  int i,j,n; 
  int p;
  int rank;
  int asce;  

  int *unsort,*sorted,*local_unsort;
  int *compare,*bitonic,*tmp;
  MPI_Status Status;

  double seq_start,seq_end;
  double start,end;


MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD,&p);
MPI_Comm_rank(MPI_COMM_WORLD,&rank);

local_size = N/p;
local_unsort=(int *)malloc(sizeof(int)*local_size);
compare=(int *)malloc(sizeof(int)*local_size);
bitonic=(int *)malloc(sizeof(int)*local_size);

  if(rank==0)
    printf("\n-----------------------------------------------------------\n"); 
  if(rank==0)
    printf("nodes=%d size=%d(%d per node)\n",p,N,local_size); 

srand((unsigned) time(NULL)*rank);

unsort=(int *)malloc(sizeof(int)*N);
sorted=(int *)malloc(sizeof(int)*N);

   if(rank==0)
     for (i=0;i<N;i++)
      unsort[i]=rand()%N;

  MPI_Scatter(unsort,local_size,MPI_INT,local_unsort,local_size,MPI_INT,0,MPI_COMM_WORLD);
   
seq_start=MPI_Wtime();
  if (rank==0){
     quick_sort_asce(unsort,0,N-1);
   }
seq_end=MPI_Wtime();


start=MPI_Wtime();

   if(rank%2==0)
     quick_sort_asce(local_unsort,0,local_size-1);
   else
     quick_sort_desc(local_unsort,0,local_size-1);

  for(i=0;i<log_2(p);i++)
  {
      asce=pred(i,rank);
      sorting(i,i,rank,p,local_unsort,compare,bitonic,asce,local_size);
  }

   MPI_Gather(bitonic,local_size,MPI_INT,sorted,local_size,MPI_INT,0,MPI_COMM_WORLD);
   
end=MPI_Wtime();
   

   if(rank==0)
   {
      check(unsort,sorted,N);
      printf("elapsed time=%e (seqential %e)  %3.2f times speedup.\n",(end-start),(seq_end-seq_start),(seq_end-seq_start)/(end-start));
      printf("-----------------------------------------------------------\n"); 
   }
   MPI_Finalize();
}

int pred(int i,int rank)
{
return rank%pow_2(i+2)<pow_2(i+1)?1:0;
}

void sorting(int i,int j,int rank,int p,int local_sort[],int compare[],int bitonic[],int asce,int local_size)
{
 int src=rank,dst_c,dst_n;
 int n;
 MPI_Status Status;
 if(i==0)
 {
   dst_c=rank%pow_2(1)<pow_2(0)?rank+pow_2(0):rank-pow_2(0);
  if(src<dst_c)
    {
      MPI_Send(local_sort,local_size,MPI_INT,dst_c,dst_c,MPI_COMM_WORLD);
      MPI_Recv(compare,local_size,MPI_INT,dst_c,src,MPI_COMM_WORLD,&Status);
    }
    else
    {
      MPI_Recv(compare,local_size,MPI_INT,dst_c,src,MPI_COMM_WORLD,&Status);
      MPI_Send(local_sort,local_size,MPI_INT,dst_c,dst_c,MPI_COMM_WORLD);
    }
    if(asce==1)
    {
      if(src<dst_c)
        compare_min(local_sort,compare,bitonic,local_size);
      else
        compare_max(local_sort,compare,bitonic,local_size);
     scan_and_merge_asce(local_sort,bitonic,local_size);
   }
   else
   {
     if(src<dst_c)
       compare_max(local_sort,compare,bitonic,local_size);
     else
       compare_min(local_sort,compare,bitonic,local_size);
     scan_and_merge_desc(local_sort,bitonic,local_size);
  }
  for(n=0;n<local_size;n++)
    bitonic[n]=local_sort[n];
  return ;
 }

  dst_c=rank%pow_2(i+1)<pow_2(i)?rank+pow_2(i):rank-pow_2(i);
  dst_n=rank%pow_2(i)<pow_2(i-1)?rank+pow_2(i-1):rank-pow_2(i-1);

  if(src<dst_c)
  {
    MPI_Send(local_sort,local_size,MPI_INT,dst_c,dst_c,MPI_COMM_WORLD);
    MPI_Recv(compare,local_size,MPI_INT,dst_c,src,MPI_COMM_WORLD,&Status);
  }
  else
  {
    MPI_Recv(compare,local_size,MPI_INT,dst_c,src,MPI_COMM_WORLD,&Status);
    MPI_Send(local_sort,local_size,MPI_INT,dst_c,dst_c,MPI_COMM_WORLD);
  }
  if(asce==1)
    if(src<dst_c)
      compare_min(local_sort,compare,bitonic,local_size);
    else
      compare_max(local_sort,compare,bitonic,local_size);
   else
    if(src<dst_c)
      compare_max(local_sort,compare,bitonic,local_size);
    else
      compare_min(local_sort,compare,bitonic,local_size);

  if(src<dst_n)
    scan_and_merge_asce(local_sort,bitonic,local_size);
  else    
    scan_and_merge_desc(local_sort,bitonic,local_size);
  for(n=0;n<local_size;n++)
     bitonic[n]=local_sort[n];
  
  sorting(i-1,j,rank,p,local_sort,compare,bitonic,asce,local_size);	
}

void scan_and_merge_asce(int local_sort[],int bitonic[],int local_size)
{
 int i,j,k,l;
 for(i=0;i<local_size-1;i++)
  if(bitonic[i]==bitonic[i+1])
    continue;
  else
    break;
 for(j=0;j<i;j++)
    local_sort[j]=bitonic[j];

 if(i==local_size-1)
 {
   local_sort[i]=bitonic[i];
   return;
 }
 j=i;
 if(bitonic[i]<bitonic[i+1])
 {
    while( j < local_size-1  && bitonic[j]<=bitonic[j+1] )
    {
      local_sort[j]=bitonic[j];
      j++;
    }
    if(j==local_size-1)
      local_sort[j]=bitonic[j];
    for(k=j;k<local_size;k++)
      local_sort[k]=bitonic[local_size-1+j-k];
 }
 else 
 {
    while(j < local_size - 1 && bitonic[j]>=bitonic[j+1])
      j++;
    if(j==local_size-1)
      local_sort[j]=bitonic[j];
    for(k=0;k<j;k++)
      local_sort[k]=bitonic[j-k-1];
    for(k=j;k<local_size;k++)
      local_sort[k]=bitonic[k];
 }

 merge_asce(local_sort,j,local_size);
}

void merge_asce(int local_sort[],int n,int local_size)
{
  int i,j=0,k=n;
  int *local=(int *)malloc(sizeof(int)*local_size);

  for(i=0;i<local_size;i++)
  {
   if((local_sort[j]<=local_sort[k] && j<n) || k==local_size)
   {
     local[i]=local_sort[j];
     j++;
   }
   else
   {
     local[i]=local_sort[k];
     k++;
   }
  }
  for(i=0;i<local_size;i++)
    local_sort[i]=local[i];
}

void scan_and_merge_desc(int local_sort[],int bitonic[],int local_size)
{
 int i,j,k,l;
 for(i=0;i<local_size-1;i++)
  if(bitonic[i]==bitonic[i+1])
    continue;
  else
    break;
 for(j=0;j<i;j++)
    local_sort[j]=bitonic[j];

 if(i==local_size-1)
 {
   local_sort[i]=bitonic[i];
   return;
 }

 j=i;
 if(bitonic[i]>bitonic[i+1])
 {
    while( j < local_size-1  && bitonic[j]>=bitonic[j+1] )
    {
      local_sort[j]=bitonic[j];
      j++;
    }
    if(j==local_size-1)
      local_sort[j]=bitonic[j];
    for(k=j;k<local_size;k++)
      local_sort[k]=bitonic[local_size-1+j-k];
 }
 else 
 {
    while(j < local_size - 1 && bitonic[j]<=bitonic[j+1])
      j++;
    if(j==local_size-1)
      local_sort[j]=bitonic[j];
    for(k=0;k<j;k++)
      local_sort[k]=bitonic[j-k-1];
    for(k=j;k<local_size;k++)
      local_sort[k]=bitonic[k];
 }
 merge_desc(local_sort,j,local_size);
}

void merge_desc(int local_sort[],int n,int local_size)
{
  int i,j=0,k=n;
  int *local=(int *)malloc(sizeof(int)*local_size);

  for(i=0;i<local_size;i++)
  {
   if((local_sort[j]>=local_sort[k] && j<n) || k==local_size)
   {
     local[i]=local_sort[j];
     j++;
   }
   else
   {
     local[i]=local_sort[k];
     k++;
   }
  }
  for(i=0;i<local_size;i++)
    local_sort[i]=local[i];
}

void check(int *answer,int *sorted,int n)
{
   int i;
   
   for(i=0;i<n;i++)
     if(answer[i]!=sorted[i])
     {
       printf("failed position %d\n",i);
       return;
     }
    printf("Check OK!\n");
}

void compare_min(int m1[],int m2[],int b[],int size)
{
 int i;
 for(i=0;i<size;i++)
 {
   if(m1[i]>=m2[i])
     b[i]=m2[i];
   else
     b[i]=m1[i];
 }
}

void compare_max(int m1[],int m2[],int b[],int size)
{
 int i;
 for(i=0;i<size;i++)
 {
   if(m1[i]<m2[i])
     b[i]=m2[i];
   else
     b[i]=m1[i];
 }
}

void print_column(char *c,int a[],int n)
{
   int i;
 printf(c);
   for (i=0;i<n-1;i++)
   {
      printf("%ld,",a[i]);
   }
      printf("%ld\n",a[i]);
}


void quick_sort_desc(int a[],int p,int q)
{
   int i,j;
   
   if(p < q)
   {
     partition_desc(a,p,q,&j,&i);
     quick_sort_desc(a,p,j);
     quick_sort_desc(a,i,q);
   }
}

void partition_desc(int a[],int p,int q,int *jp,int *ip)
{
   int i,j,s;
   
   i=p;
   j=q;
   s=a[p];
   while(i <= j){
     while(a[i]>s) ++i;
     while(a[j]<s) --j;
     if(i <= j){
       swap(&a[i],&a[j]);
       ++i;
       --j;
     }
   }
   *ip=i;
   *jp=j;
}

void quick_sort_asce(int a[],int p,int q)
{
   int i,j;
   
   if(p < q)
   {
     partition_asce(a,p,q,&j,&i);
     quick_sort_asce(a,p,j);
     quick_sort_asce(a,i,q);
   }
}

void partition_asce(int a[],int p,int q,int *jp,int *ip)
{
   int i,j,s;
   
   i=p;
   j=q;
   s=a[p];
   while(i <= j){
     while(a[i]<s) ++i;
     while(a[j]>s) --j;
     if(i <= j){
       swap(&a[i],&a[j]);
       ++i;
       --j;
     }
   }
   *ip=i;
   *jp=j;
}


void swap(int *u,int *v)
{
   int tmp;
   tmp=*u;
   *u=*v;
   *v=tmp;
}

int log_2(int n)
{
return (int)(floor(log10((double)n)/log10((double)2)));
}

int pow_2(int n)
{
return (int)(floor(pow((double)2,(double)n)));
}
