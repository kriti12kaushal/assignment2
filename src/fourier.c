#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

long long n;
long number_of_thread;
double value=0.0;
sem_t semaphore;

double Serial_version(long long n);
void* Parallel_version(void* id);
void setValues(int argc, char* argv[]);

int main(int argc,char * argv[]) {
   long i; 
   pthread_t* thread_array;
   struct timeval begin1, end1,begin2, end2;
   double speedup,accuracy;
   FILE *fptr;
   
   printf("\n***FOURIER CALCULATION***\n\n");
   setValues(argc, argv);
   
   gettimeofday(&begin1, 0);
 
   thread_array = (pthread_t*) malloc (number_of_thread*sizeof(pthread_t)); 
   
   sem_init(&semaphore, 0, 1);

 
   for (i= 0; i < number_of_thread; i++) 
      pthread_create(&thread_array[i], NULL,
          Parallel_version, (void*)i);  

   for (i= 0; i < number_of_thread; i++) 
      pthread_join(thread_array[i], NULL);

    value = sqrt((value + 0.5)*8);
   printf("pi value for parallel_version = %.18f\n",value);
   gettimeofday(&end1, 0);
    long seconds1 = end1.tv_sec - begin1.tv_sec;
    long microseconds1 = end1.tv_usec - begin1.tv_usec;
    double totalTime1 = seconds1 + microseconds1*1e-6;
    printf("Time measured by parallel_version: %.6f seconds.\n", totalTime1);

   
   gettimeofday(&begin2, 0); 
   value = Serial_version(n);
   printf("pi value for Serial_version   = %.18f\n",value);
   gettimeofday(&end2, 0);
    long seconds2 = end2.tv_sec - begin2.tv_sec;
    long microseconds2 = end2.tv_usec - begin2.tv_usec;
    double totalTime2 = seconds2 + microseconds2*1e-6;
    printf("Time measured by serial_version: %.6f seconds.\n\n", totalTime2);
    speedup =totalTime2/totalTime1;
     printf("Fourier's Method speedup with n=%lli and numberOfThread=%li = %.3f\n",n,number_of_thread,speedup);
      
    //calculating accuracy
   double y =value - 3.141592653589793238;
   if (y<0.0)
   {
   	y =  (-1)*y;	
   }
   accuracy=100.0 - (y/3.141592653589793238)*100.0;
   printf("Fourier's Method accuracy with n=%lli = %.18f\n\n",n,accuracy);
   
   //Storing data for plotting
   fptr=fopen("/home/baadalvm/Documents/ssl/assignment2/gnuplot/data/fourier.txt", "a");
   if(fptr==NULL){
   printf("file does not exist\n");
   return 0;
   }
   fprintf(fptr,"%li "         " %.5f\n",number_of_thread,totalTime1);
   fclose(fptr);
   
   
   sem_destroy(&semaphore);
   free(thread_array);
   return 0;
}  


// serial Version implementation of Fourier series Method for pi calculation
double Serial_version(long long n) {
   

   double pi = 0.0;

   for (long long i = 1; i < n+1; i++) {
      pi += (2.0)/((2*i-1)*(2*i-1)*(2*i+1));
      
   }
   pi = sqrt((pi + 0.5)*8);
   return pi;

}



//parallel version implementation of Fourier series Method for pi calculation
void* Parallel_version(void* id) {
   long my_id = (long) id;
   long double i;
   long long my_count= n/number_of_thread;
   long long start = my_count*my_id+1;
   long long end = start + my_count ;
   double my_calculation= 0.0;
   
   for (i = start; i < end; i++)  
     {
       my_calculation += (2.0)/((2*i-1)*(2*i-1)*(2*i+1));
  
    }
   
   sem_wait(&semaphore);
   value += my_calculation;
   sem_post(&semaphore);

   return NULL;
}  

void setValues(int argc, char* argv[]) 
{
   number_of_thread= strtol(argv[1], NULL, 10);  
   n = strtoll(argv[2], NULL, 10);
}
