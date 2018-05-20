#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h> /*for mmap() function*/
#include <string.h> /*for memcpy function*/
#include <fcntl.h> /*for file descriptors*/
#include <unistd.h>
#include <math.h>

//sorting algorithms
void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// A function to implement bubble sort
void bubbleSort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n-1; i++)

        // Last i elements are already in place
        for (j = 0; j < n-i-1; j++)
            if (arr[j] > arr[j+1])
                swap(&arr[j], &arr[j+1]);
}


//FIRST COME FIRST SERVE
void FCFS(int cylinders[], int times[] , int req_size)
{

    int head = 1;
    int headmov = abs(cylinders[0] - head);
    //printf("CYlinder being served is %d\n",cylinders[0]);
    double waiting=0;
    //create an array whee we will keep the waiting time for each   requesgt
    double waiting_array[req_size];
    int INDEX = 0;
    waiting_array[INDEX] = 0; //waiting time of first req is always 0
    INDEX++;
    int cur_time = headmov + times[0];
    
    
    for(int i = 0; i < req_size-1; i++)
    {
        //in the case that the next cylinder has not arrived yet 
        if(cur_time<times[i+1]){
          
          waiting_array[INDEX] = 0;
          INDEX++;
          cur_time = times[i+1];
        
        }
    	else{
			waiting_array[INDEX] = cur_time - times[i+1]; //insert waiting time of each req in array
		    waiting += cur_time -times[i+1];
		    INDEX++;
		    }

        headmov += abs(cylinders[i+1] - cylinders[i]);
        //printf("CYlinder being served is %d\n",cylinders[i+1]);
        cur_time += abs(cylinders[i+1] - cylinders[i]);
        
    }
    
    printf("FCFS : ");
     
    double mean_wait = waiting/req_size;
    printf("%d	", headmov);
    printf("%lf	",mean_wait);
    
    
        //STANDART DEVIATION
    int sum =0;
    for (int i = 0; i<INDEX; i++)
    {
        waiting_array[i] = waiting_array[i] - mean_wait;
        waiting_array[i]  = waiting_array[i] * waiting_array[i];
        sum += waiting_array[i];
    }


    double a = sum/(req_size-1);
    double std = sqrt(a);
    printf("%lf\n",std);
}


//SHORTEST SEEK TIME FIRST
void SSTF(int cylinders[], int times[], int req_size)
{


    int head = 1;
    double waiting=0;
    //create an array whee we will keep the waiting time for each   requesgt
    double waiting_array[req_size];
    int INDEX = 0;
    waiting_array[INDEX] = 0; //waiting time of first req is always 0
    INDEX++;

    //check the beginning of the time array if there are more then one req coming at the same time
    int count =1;

    for( int i =1; i<req_size; i++)
    {
        if (times[i] == times[0])
        {
            count++;
        }
        //printf("Countt is %d\n", count);
    }

    //find the closest
    int current = 0;
    int smallest_hm = abs( cylinders[0] - head);
    for(int i =1; i<count; i++)
    {
        int hm = abs( cylinders[i] - head);
        if(hm<smallest_hm)
        {
            smallest_hm = hm;
            current = i;
        }
    }


    int headmov = abs(cylinders[current] - head);
    int LAST_VISITED = cylinders[current];
    //printf("The visited cylinder is: %d\n",LAST_VISITED);
    cylinders[current] = 100000;
    int cur_time = headmov + times[current];


    for (int i = 0; i< req_size-1; i++)
    {
        //in the case that the next cylinder has not arrived yet 
        if(cur_time<times[i+1]){
          
          waiting_array[INDEX] = 0;
          INDEX++;
          cur_time = times[i+1];
        
        }
    	else{
        
		    waiting_array[INDEX] = cur_time - times[i+1]; //insert waiting time of each req in array
		    waiting += cur_time -times[i+1];
		    INDEX++;
		    }

        count = 0;
        for(int j = 0; j< req_size; j++)
        {
            //check the time - to see which req has arrived
            if (times[j] <= cur_time)
            {
                count ++; //number of req that have arrived
            }
        }

        current = 0;
        //go to cylinder and check
        smallest_hm = abs( cylinders[0] - LAST_VISITED);
        //printf("######### %d\n",smallest_hm);
        for(int k = 1; k<count; k++)
        {
            int hm1 = abs( cylinders[k] - LAST_VISITED);
            if(hm1<smallest_hm)
            {
                smallest_hm = hm1;
                current = k;
            }
        }
        
       
        headmov += abs(cylinders[current] - LAST_VISITED);
        cur_time += abs(cylinders[current] - LAST_VISITED);
        LAST_VISITED = cylinders[current];
        //printf("The visited cylinder is: %d\n",LAST_VISITED);
        //printf("Current time %d\n", cur_time);
        cylinders[current] = 100000;
        //printf("======================================\n");
    }

    printf("SSTF : ");
    //printf("Total head movement is: %d\n",headmov);
    double mean_wait = waiting/req_size;
    //printf("Total time is: %lf\n",mean_wait);
    printf("%d	", headmov);
    printf("%lf	",mean_wait);

    //STANDART DEVIATION
    int sum =0;
    for (int i = 0; i<INDEX; i++)
    {
        waiting_array[i] = waiting_array[i] - mean_wait;
        waiting_array[i]  = waiting_array[i] * waiting_array[i];
        sum += waiting_array[i];
    }


    double a = sum/(req_size-1);
    double std = sqrt(a);
    printf("%lf\n",std);
}


//LOOk Algorithm
void LOOK(int cylinders[], int times[], int req_size)
{
    int head = 1;

    //declare 2 arrayse where we will keep the greater and smaller values
    int greater[req_size];
    int smaller[req_size];


    double waiting=0;
    //create an array whee we will keep the waiting time for each   requesgt
    double waiting_array[req_size];
    int INDEX = 0;
    waiting_array[INDEX] = 0; //waiting time of first req is always 0
    INDEX++;

    //check the beginning of the time array if there are more then one req coming at the same time
    int count =1;

    for( int i =1; i<req_size; i++)
    {
        if (times[i] == times[0])
        {
            count++;
        }
    }

    //find the smallest
    int current = 0;
    int smallest = cylinders[0] ;
    for(int i =1; i<count; i++)
    {
        int c = cylinders[i];
        if(c<smallest)
        {
            smallest = c;
            current = i; //current is where we should go
        }
    }

    int headmov = abs(cylinders[current] -head);

    int LAST_VISITED = cylinders[current];
    //printf("The visited cylinder is: %d\n",LAST_VISITED);
    int cur_time = headmov + times[current];
    cylinders[current] = 100000; //put some large value to current


    for(int i =0; i<req_size-1;  i++)
    {
    
        //in the case that the next cylinder has not arrived yet 
        if(cur_time<times[i+1]){
          
          waiting_array[INDEX] = 0;
          INDEX++;
          cur_time = times[i+1];
        
        }
    	else{
        
        waiting_array[INDEX] = cur_time - times[i+1]; //insert waiting time of each req in array
        waiting += cur_time -times[i+1];
        INDEX++;
        }
        
        count = 0;
        for(int j = 0; j< req_size; j++)
        {
            //check the time - to see which req has arrived
            if (times[j] <= cur_time)
            {
                count ++; //number of req that have arrived
            }
        }

        //go to cylinder and check
        smallest = LAST_VISITED;
        current = 0;
        int j =0;
        int k = 0;
        for(int m = 0; m<count; m++)
        {
            int c =  cylinders[m];
            if(c>smallest && c!=100000)
            {
                greater[j] = c;
                j++;
            }
            else if(c<smallest)
            {
                //insert the element into smaller array
