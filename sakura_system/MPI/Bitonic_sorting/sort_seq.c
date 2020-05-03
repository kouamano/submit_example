#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#define D 100

extern void check(int a[],int b[],int N);
extern void copy_column(int a[],int b[],int N);
extern void print_column(int a[],int N);
extern void selection_sort(int a[],int S,int N);
extern void insertion_sort(int a[],int S,int N);
extern void quick_sort_selection(int a[],int p,int q,int n);
extern void quick_sort_insertion(int a[],int p,int q,int n);
extern void quick_sort(int a[],int p,int q);
extern void partition(int a[],int p,int q,int *jp,int *ip);
extern void swap(int *u,int *v);

int main(int argc,char *argv[])
{
  int N=atoi(argv[1]);
  int size=N;
  int i,n; 
  
  int *unsort,*sorted,*answer;

  struct timeval startTime, endTime;
  time_t diffsec;
  double realsec; 
  suseconds_t diffsub; 
  printf("\n-----------------------------------------------------------\n"); 

  srand((unsigned) time(NULL));
  unsort=(int *)malloc(sizeof(int)*N);
  sorted=(int *)malloc(sizeof(int)*N);
  answer=(int *)malloc(sizeof(int)*N);

  for(i=0;i<N;i++)
     unsort[i]=rand()%N;

  copy_column(answer,unsort,N);

  gettimeofday(&startTime, NULL);
  quick_sort(answer,0,N-1);
  gettimeofday(&endTime, NULL);

  diffsec = difftime(endTime.tv_sec, startTime.tv_sec);
  diffsub = endTime.tv_usec - startTime.tv_usec; 
  realsec = diffsec+diffsub*1e-6; 

  printf("quick sort elapsed time=%e\n",realsec);
  printf("-----------------------------------------------------------\n"); 
  for(n=1;n<D;n++)
  {
  copy_column(sorted,unsort,N);

  gettimeofday(&startTime, NULL);
  quick_sort_selection(sorted,0,N-1,n);
  gettimeofday(&endTime, NULL);

  check(sorted,answer,N);
    
  diffsec = difftime(endTime.tv_sec, startTime.tv_sec);
  diffsub = endTime.tv_usec - startTime.tv_usec; 
  realsec = diffsec+diffsub*1e-6; 

  printf("quick sort(selection %d) elapsed time=%e\n",n,realsec);
  }
  printf("-----------------------------------------------------------\n"); 
  
 for(n=1;n<D;n++)
 {
  copy_column(sorted,unsort,N);

  gettimeofday(&startTime, NULL);
  quick_sort_insertion(sorted,0,N-1,n);
  gettimeofday(&endTime, NULL);

  check(sorted,answer,N);
   
  diffsec = difftime(endTime.tv_sec, startTime.tv_sec);
  diffsub = endTime.tv_usec - startTime.tv_usec; 
  realsec = diffsec+diffsub*1e-6; 

  printf("quick sort(insertion %d) elapsed time=%e\n",n,realsec);
 }
  printf("-----------------------------------------------------------\n"); 

  copy_column(sorted,unsort,N);

  gettimeofday(&startTime, NULL);
//  selection_sort(sorted,0,N);
  gettimeofday(&endTime, NULL);

  check(sorted,answer,N);

  diffsec = difftime(endTime.tv_sec, startTime.tv_sec);
  diffsub = endTime.tv_usec - startTime.tv_usec; 
  realsec = diffsec+diffsub*1e-6; 
  printf("selection sort elapsed time=%e\n",realsec);
  printf("-----------------------------------------------------------\n"); 
  
  copy_column(sorted,unsort,N);

  gettimeofday(&startTime, NULL);
//  insertion_sort(sorted,0,N);
  gettimeofday(&endTime, NULL);

  check(sorted,answer,N);
    
  diffsec = difftime(endTime.tv_sec, startTime.tv_sec);
  diffsub = endTime.tv_usec - startTime.tv_usec; 
  realsec = diffsec+diffsub*1e-6; 
  printf("insertion sort elapsed time=%e\n",realsec);
  printf("-----------------------------------------------------------\n"); 
}

void check(int a[],int b[],int N)
{
 int i;
 for(i=0;i<N;i++)
  if(a[i]!=b[i])
  {
    printf("failed %d\n",i);
    return;
  }
 printf("Check OK!\n");
}

void copy_column(int a[],int b[],int N)
{
  int i;
  for(i=0;i<N;i++)
     a[i]=b[i];
}

void print_column(int a[],int N)
{
 int i;
 for(i=0;i<N-1;i++)
   printf("%d,",a[i]);
 printf("%d\n",a[i]);
}

void selection_sort(int a[],int S,int N)
{
 int i,j,min;
 for(i=S;i<N;i++)
 {
  min=i;
  for(j=i;j<N;j++)
    if(a[min]>a[j])
      min=j;
  if(i!=min)
    swap(&a[i],&a[min]);
 }
}

void insertion_sort(int a[],int S,int N)
{
 int i,j,tmp;
 for(i=S+1;i<N;i++)
 {
   for(j=i ; j>0 && a[j-1]>a[j];j--)
   {
     swap(&a[j],&a[j-1]);
   }
 }
}

void quick_sort_insertion(int a[],int p,int q,int n)
{
   int i,j;
   if(q-p<=n)
   {
     insertion_sort(a,p,q+1);
     return ;
   }
   if(p < q)
   {
     partition(a,p,q,&j,&i);
     quick_sort_insertion(a,p,j,n);
     quick_sort_insertion(a,i,q,n);
   }
}

void quick_sort_selection(int a[],int p,int q,int n)
{
   int i,j;
   if(q-p<=n)
   {
     selection_sort(a,p,q+1);
     return ;
   }
   if(p < q)
   {
     partition(a,p,q,&j,&i);
     quick_sort_selection(a,p,j,n);
     quick_sort_selection(a,i,q,n);
   }
}

void quick_sort(int a[],int p,int q)
{
   int i,j;
   if(p < q)
   {
     partition(a,p,q,&j,&i);
     quick_sort(a,p,j);
     quick_sort(a,i,q);
   }
}

void partition(int a[],int p,int q,int *jp,int *ip)
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
