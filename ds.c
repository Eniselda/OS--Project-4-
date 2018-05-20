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
	     	//insert the element into smaller array
                smaller[k] =c;
                k++;
            }
        }

        //sort the arrays


        bubbleSort(greater, j);
        bubbleSort(smaller, k);

        //if there are elements greater we continue right

        if(j!=0)
        {

           //fprintf(stderr,"The visited cylinder is: %d\n", greater[0]);
            headmov += abs(greater[0] - LAST_VISITED);
            cur_time += abs(greater[0] - LAST_VISITED);
            LAST_VISITED = greater[0];
            //make the visted value 10000
            for(int i=0; i<req_size; i++)
            {
                if(greater[0]== cylinders[i])
                    cylinders[i]=100000;
            }

        }

        else if(j==0 && k!=0)
        {

            //printf("The visiteddd cylinder is: %d\n", smaller[k-1]);
            headmov += abs(smaller[k-1] - LAST_VISITED);
            cur_time += abs(smaller[k-1] - LAST_VISITED);
            LAST_VISITED = smaller[k-1];

            //make the visted value 10000
            for(int i=0; i<req_size; i++)
            {
                if(smaller[k-1]== cylinders[i])
                    cylinders[i]=100000;
            }
        }
    }
    
    printf("LOOK : ");
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

//CLOOK Algorithm
void CLOOK(int cylinders[], int times[], int req_size)
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
                //insert the element into greater array
                greater[j] = c;
                j++;
            }
            else if(c<smallest)
            {
                //insert the element into smaller array
                smaller[k] =c;
                k++;
            }
        }

        //sort the arrays


        bubbleSort(greater, j);
        bubbleSort(smaller, k);


        //if there are elements greater we continue right

        if(j!=0)
        {

            //fprintf(stderr,"The visited cylinder is: %d\n", greater[0]);
            headmov += abs(greater[0] - LAST_VISITED);
            cur_time += abs(greater[0] - LAST_VISITED);
            LAST_VISITED = greater[0];
            //make the visted value 10000
            for(int i=0; i<req_size; i++)
            {
                if(greater[0]== cylinders[i])
                    cylinders[i]=100000;
            }

        }

        else if(j==0 && k!=0)
        {

            //printf("The visiteddd cylinder is: %d\n", smaller[0]);
            headmov += abs(smaller[0] - LAST_VISITED);
            cur_time += abs(smaller[0] - LAST_VISITED);
            LAST_VISITED = smaller[0];

            //make the visted value 10000
            for(int i=0; i<req_size; i++)
            {
                if(smaller[0]== cylinders[i])
                    cylinders[i]=100000;
            }
        }
    }
    
    
    printf("CLOOK: ");
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

int main(int argc, char **argv)
{

    //take arguments
    int N = atoi(argv[1]);
    char* filename = argv[2];


    //find line number of file
    FILE *fp;
    fp = fopen( filename, "r");
    int linenum =0;
    char ch;
    if(fp)
    {
        while(!feof(fp))
        {
            ch = fgetc(fp);
            if(ch == '\n')
            {
                linenum ++;

            }
        }

    }
    //printf("line num is %d\n", linenum);
    
    //create arrays
    
    
    int  times [linenum];
    int cylinders [linenum];
    int  times1 [linenum];
    int cylinders1 [linenum];
    int  times2 [linenum];
    int cylinders2 [linenum];
    int  times3 [linenum];
    int cylinders3 [linenum];
    
    
    int time;
    int cylinder;

    rewind(fp);
    //read from file and put in arrays
    int i =0;
    while( fscanf(fp, "%d %d", &time, &cylinder)>0)
    {
       // printf("%d   %d\n", time, cylinder);
        
        times[i] = time;
        cylinders[i] = cylinder;
        
        times1[i] = time;
        cylinders1[i] = cylinder;
        
        times2[i] = time;
        cylinders2[i] = cylinder;
        
        times3[i] = time;
        cylinders3[i] = cylinder;
        i++;
    }


    //Call each algorithm
    FCFS(cylinders, times, linenum);
    SSTF( cylinders1,  times1, linenum);
    LOOK( cylinders2, times2, linenum);
    CLOOK( cylinders3, times3, linenum);



    return(0);
}

