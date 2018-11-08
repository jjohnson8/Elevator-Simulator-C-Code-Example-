#include <unistd.h>          //Provides API for POSIX(or UNIX) OS for system calls
#include <stdio.h>           //Standard I/O Routines
#include <stdlib.h>          //For exit() and rand()
#include <pthread.h>         //Threading APIs
#include <semaphore.h>
#define MAX_PERSONS 49

sem_t maxcapacity = {7}; // max capcity of the people to enter
sem_t door; // door to open and close
sem_t entry; // person entry
sem_t floorr; // arrival and departure of the floor
sem_t leave; // leaving the floor
sem_t mutex;  // unique ID 


sem_t finished[49]; // Finished 
static int boardID[MAX_PERSONS];// the numner of people to enter the elevator 
static int count=0; 
int k;




void *PersonThread(void *tmp); // function for the person thread
void *ElevatorThread(void *tmp); // function for the elevator thread
int done = 0;  // flag to show has finished the simulation 
int main(){
    int i;
 /*intialize the thread*/
   pthread_t elevator; 
   pthread_t person[MAX_PERSONS];

   // Initialize the boardID array.
   for (i=0; i<MAX_PERSONS; i++) {
	boardID[i] = i;
    }
  /* initalize each semaphore*/
   sem_init(&door,0,0);
  

   sem_init(&entry,0,0);
    
   sem_init(&floorr,0,0);
   
   sem_init(&leave,0,0);
  
   sem_init(&mutex,0,1);
  


   for(i=0;i<MAX_PERSONS;i++)                     //Creation of person Threads
   {
   pthread_create(&person[i],NULL,(void*)PersonThread,(void*)&boardID[i]);
  }

   pthread_create(&elevator,NULL,(void*)ElevatorThread,NULL); // creation of elevator threads

  for(i=0;i<MAX_PERSONS;i++){
   pthread_join(person[i],NULL); // join person threads
  }

   done = 1;
   printf("simulation ends \n");
   return 0;
   

}
 void *PersonThread(void *tmp){
     
     int pno = *(int *)tmp; // take the person number 
       int j=1;

       int arr[]={1,2,3,4,5,6,7}; // array of people 
       for(k=0;k <7;k++){ 
        int floor = rand()%(10-2+1)+1;   // random value 
        arr[k] =floor; // assign each floor to the array of people 
       }

       sem_wait(&maxcapacity); // wait for max capacity 
      sem_wait(&mutex);  // unique ID 
      boardID[count] = pno; 
      count++;
      sem_post(&mutex);
     
      sem_wait(&door); //wait for the door to open 
     
       printf("The person %d enters the Elevator to go to floor %d \n",pno,arr[j]);
      
       sem_post(&entry); // wait for the person to enter 
      
        

       sem_wait(&floorr); // wait for the floor 
    
       printf("the person %d leaves the elevator at floor %d \n",pno,arr[j]);
      
       sem_post(&leave); // wait to leave the floor
       sem_post(&maxcapacity); // signal the capacity has reached maximum
        

    
     

}

void *ElevatorThread(void *tmp){
     int j;
    
    while(!done) 
     {
    printf(" Elevator door opens at floor 1 \n ");
     
    sem_post(&door); // signal door oopen
    
    sem_wait(&entry); // wait for the entry of person 


  printf("The door has closed \n");
      
     
     
     printf("The elevator has opened at floor \n");
     
      
      sem_post(&floorr); // signal door open
      sem_wait(&leave); // wait for person to leave 
     
    
    printf("The elevator closes door at the floor \n");
     }
   
}

