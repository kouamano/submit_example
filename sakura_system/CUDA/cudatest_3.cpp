#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <cuda.h>
#include <cuda_runtime_api.h>

int main(int argc,char *argv[])
{
 cudaEvent_t start, stop;
 float time;
 
 cudaEventCreate(&start);
 cudaEventCreate(&stop);

 cudaEventRecord( start, 0 );
 sleep(3);
 cudaEventRecord( stop, 0 );
 
 cudaEventSynchronize( stop );
 cudaEventElapsedTime( &time, start, stop );

 printf("time = %.3f\n",time/1000.0);

 cudaEventDestroy( start );
 cudaEventDestroy( stop );

}
