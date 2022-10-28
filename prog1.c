/* *********************************************************************
@author CryptidRegrex
@date September 19th 2022

This file uses pthreads to find the log value of a number between 0 < x < 2


Compile with:

	gcc -Wall prog1.c -o prog1 -lpthread -lm


********************************************************************** */





#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

double finalValue = 0;
pthread_mutex_t locker = PTHREAD_MUTEX_INITIALIZER;


struct dataIn {
	int threadIDVal;
	int iterationVal;
	int threadVal;
	double logVal;
};


void *computeLog(void *data);


void *computeLog(void *data) 
{
	//dereference struct values passed into function
	struct dataIn d = *(struct dataIn*)data;
	int i = d.threadIDVal;
	int gIterate = d.iterationVal;
	int gThreads = d.threadVal;
	double logValue = d.logVal;



	//printf("thread id: %d\n", i);


	for (int x = 0; x < gIterate; x++) {
		//Locking our global variable
		/*if (pthread_mutex_lock(&locker) != 0) {
			printf("Error locking\n");
		}*/

		//Determine what thread is processing data if odd then positive values if even negative values
		//Locking each time global variable is accessed. 
		if (i % 2 != 0) {
			if (i == 1 && x == 0) {
				if (pthread_mutex_lock(&locker) != 0) {
					printf("Error locking\n");
				}

				finalValue += (logValue - 1);

				if (pthread_mutex_unlock(&locker) != 0) {
					printf("Error unlocking thread\n");
				} 
				//printf("FIRST value: %f\n", logValue);
			}
			else {
				if (pthread_mutex_lock(&locker) != 0) {
					printf("Error locking\n");
				}

				finalValue += (pow((logValue - 1), ((gThreads * x) + i)) / ((gThreads * x) + i));

				if (pthread_mutex_unlock(&locker) != 0) {
					printf("Error unlocking thread\n");
				} 
				//printf("pow: %d\n", ((gThreads * x) + i));
			}
			
		}
		else {
			if (pthread_mutex_lock(&locker) != 0) {
				printf("Error locking\n");
			}

			finalValue -= (pow((logValue - 1), ((gThreads * x) + i)) / ((gThreads * x) + i));

			if (pthread_mutex_unlock(&locker) != 0) {
				printf("Error unlocking thread\n");
			} 
			//printf("pow: %d\n", ((gThreads * x) + i));
		}


		//Unlocking our global variable for access again
		/*if (pthread_mutex_unlock(&locker) != 0) {
			printf("Error unlocking thread\n");
		} */
	}

	//printf("Thread exiting \n");
	pthread_exit(&data);

	return NULL;

}

int main(int argc, char *argv[])
{
	int iterate;
	printf("Starting Program\n");

	char *userV = argv[1];
	char *totalT = argv[2];
	char *totalI = argv[3];

	double userLogValue = atof(userV);
	int totalThreads = atoi(totalT);
	int totalIterations = atoi(totalI);


	pthread_t threadID_table[totalThreads];

	//Only for Math.h value
	double result = 0;
	result = log(userLogValue);
	//Only for math.h value


	//printf("arg0 %f arg1 %d arg2 %d\n", userLogValue, totalThreads, totalIterations);

	//pthread intializing our lock
	if(pthread_mutex_init(&locker, NULL) != 0 )
	{
		printf("Mutex lock error\n");
	}

	//Setting up struct to pass data with
	struct dataIn args[totalThreads];

	//Initialize each thread pass by user arguement 2 of 3
	for (iterate = 0; iterate < totalThreads; iterate++)
	{
		//adding data to struct
		int t = iterate + 1;
		args[iterate].threadIDVal = t;
		args[iterate].iterationVal = totalIterations;
		args[iterate].threadVal = totalThreads;
		args[iterate].logVal = userLogValue;

		int threadStatus = 0;

		//Creating our threads
		threadStatus = pthread_create(&threadID_table[iterate], NULL, computeLog, (void*) &args[iterate]);

		//Determine if there is an error in our creation status
		if (threadStatus != 0) 
		{
			printf("Thread creation error");
		}
	}

	//Joining threads 
	for (int x = 0; x < totalThreads; x++) {
		int * threadRet;
		int joinRet;

		joinRet = pthread_join(threadID_table[x], (void**) (&threadRet));
		if (joinRet == 0) {
			printf("join threadID value: %ld thread retrieve: %d join retrieve: %d\n",threadID_table[x], *threadRet, joinRet);
		}
		else {
			printf("Join error: %d\n", joinRet);
		}
	}

	//printing out our data for math.h and my answer
	printf("My Calculated Answer: %0.14f\n", finalValue);
	printf("Math.h Function Answer: %0.14f\n", result);

	//destroy mutex lock and exit threads
	pthread_mutex_destroy(&locker);
	pthread_exit(NULL);
	return 0;
}
