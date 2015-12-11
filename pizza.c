// Matt Layman CS341 pizza.c

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef int bool;
#define true 1
#define false 0
//#define SLICES 3       //pizza slices
//#define STUDENTS 2     //number of students(threads)

long slices, SLICES;                     //shared between students,pizzapalace
bool havePizza = false;    //conditions for student threads

/*function prototypes*/
void pizzapalace(void *);      //executed by pizza palace thread
void *student(void *);         //executed by student thread

void make_pizza(long);          //used by pizzapalace to update slices
void study();                 //used by student after getting slice
void sleep(void);             //suspends thread for short (random) time
unsigned int seed = 17;       //random # seed for sleep()

sem_t mutex;                  //mutex sempahore for shared slices
sem_t freshpizza;         //full pizza with S slices
sem_t emptypizza;         //empty pizza with 0 slices

int main(int argc, char *argv[])
{
   pthread_t *stu;
   int rc, tc;
   pthread_attr_t attr;
   long t, students;

   if (argc < 3){
      perror("Please enter students (argv[1]) and slices (argv[2])\n");
      exit(EXIT_FAILURE);
   }

   students = strtol(argv[1], NULL, 10);
   slices = strtol(argv[2], NULL, 10);
   SLICES = slices;

   // Initialize semaphores
   sem_init(&mutex,0,1);       //mutex starts unlocked
   sem_init(&emptypizza,0,0);       //start with no empty pizza
   sem_init(&freshpizza,0, slices);        //start with one full pizza with S slices

   printf("There are %ld students studying\n", students);
   printf("There is one whole pizza with %ld slices available\n", slices);
   
   // Create thread for student
   stu = malloc(students*sizeof(pthread_t));
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   for (t=0; t < students; t++){
      rc = pthread_create(&stu[t], NULL, student, (void *)t);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
      }
   }

   // main thread is pizzaplace
   pizzapalace(NULL);
   // wait for student thread 
   for (t=0; t < slices; t++){
      tc = pthread_join(stu[t], NULL);
      if (tc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
      }
   }
   pthread_attr_destroy(&attr);
   sem_destroy(&mutex);
   sem_destroy(&freshpizza);
   sem_destroy(&emptypizza);
   free(stu);
   pthread_exit(NULL);
}

void pizzapalace(void *arg)
{
   //int pizzasmade = 0;

   while (1){

      int i = 0;

      sem_wait(&emptypizza);                //wait for empty pizza

      // begin critical region
      sem_wait(&mutex);
        make_pizza(slices);      //make a pizza
      sem_post(&mutex);
      // end critical region

      //sem_post(&freshpizza);                   //signal full pizza
      while(i<slices){
         sem_post(&freshpizza);
      }
      sleep();
   }

   printf("Pizza palace: We're closed!\n");
}

void *student(void *threadid)
{
   long tid;
   tid = (long) threadid;
   while (1){
      study();          // sleep thread for a few
      havePizza = false;
      printf("S%ld: I want pizza!\n", tid);
      sem_wait(&freshpizza);

      // begin critical region
      sem_wait(&mutex);
        while(!havePizza){
             if(slices>0){
               slices--;
               printf("S%ld: Got my pizza! - %ld slices left\n", tid, slices);
               if(slices == 0){
                  printf("S%ld: I ordered more pizza\n", tid);
               }
            }
            havePizza = true;
        } // end while !havePizza
      sem_post(&mutex);
      if(slices == 0){
         sem_post(&emptypizza);
      }
      sleep();        //sleep thread
   }
   printf("S%ld: Done studying!\n", tid);
   pthread_exit(NULL);
}

void make_pizza(long S){
   sleep();              //sleep random time
   slices = SLICES;
   printf("Pizza palace: Here's your whole pizza with %ld slices\n", slices);
}

void sleep(void)         //function to sleep threads
{
   struct timespec tspec;
   tspec.tv_sec = 0;
   tspec.tv_nsec = (long) (rand_r(&seed) % 1000)*1000000;

   nanosleep( &tspec, NULL);
}

void study()
{
   sleep();              //sleep random time
}
