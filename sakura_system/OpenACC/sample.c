#include <stdio.h>
#include <stdlib.h>

#define n 65535

int main()
{
float x[n],y[n];
float a;
int i;

for(i=0;i<n;i++)
  x[i]=(float)rand();

a=(float)rand();
#pragma acc kernels
for(i=0;i<n;i++)
{
  y[i]=a*x[i];
}

for(i=0;i<10;i++)
{
 printf("y[%d]=%.10f\n",i,y[i]);
}
exit(0);
}
