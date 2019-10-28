
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include "potDriver.h"

// Thread variable and synchronization
pthread_mutex_t locker = PTHREAD_MUTEX_INITIALIZER;
void lock()
{
	pthread_mutex_lock(&locker);
}
void unlock(){
	pthread_mutex_unlock(&locker);
}

// Array initalization and size tracker variables
int* arr;
static int currSize = 100;
static long long numSorted = 0;
static int numSortedOneSec = 1;

// Variable to keep track if sorting is currently allowed
bool isSorting = true;

// Returns an int of the number of arrays sorted per second
int numSortedperSec(){
	return numSortedOneSec;
}

//

/*
// Used to randomly fill an array with ints with no repeat values (slower)
void fillArray(int size){
	int numFilled=0, fillNum;
	while (numFilled != size){
		fillNum= (rand()%size)+1;
		if (!foundElement(fillNum, size)){
			arr[numFilled] = fillNum;
			numFilled++;
		}
	}
}

// Used to see if an element is within in an array
bool foundElement(int target, int n){
	for (int i=0;i<n;i++){
		if (arr[i]==target){
			return true;
		}
	}
	return false;
}
*/

// Used to fill an array with random permutations according to the length of the array
void fillArray(int size){
	// Fill inital array with non repeating numbers
	for (int i=0;i<size;i++){
		arr[i]=i+1;
	}
	// Randomly change order of the elements of the array
	for (int j=0;j<size;j++){
		int indexOne = rand()%size;
		int indexTwo = 0;
		int tmp = arr[indexOne];
		arr[indexOne] = arr[indexTwo];
		arr[indexTwo] = tmp;
	}
}
// Used for bubbleSort, swps the values of two intergers
void swap(int *x, int *y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
}

// Basic bubblesort algorithm to sort the integers elements within an array
void bubbleSort(int arr[], int n)
{
	for (int i=0; i<n-1; i++){
		for (int j=0; j<n-i-1; j++){
			if (arr[j] > arr[j+1]){
				swap(&arr[j], &arr[j+1]);
			}
		}
	}

}

// Main thread function that calls the main sorting functions
void* sorterThread(void* arg){
	// The loop is locked before the sort such that the data cannot be affected by other sources
	pthread_mutex_lock(&locker);
	clock_t last = clock();

	while(isSorting){

		// Used to keep track of the time and track each second to calculate number of arrays sorted per second
		clock_t current = clock();
		numSortedOneSec = 1;
		if (current >= (last + 1 * CLOCKS_PER_SEC)){
			numSortedOneSec = 1;
		}
		sleep(1);
		// gets the current size of the array
		printf("PWL: %d \n", PWL());
		int pwl = PWL();

		// creates the memory needed for the array and updates the size
		arr = (int *)malloc(sizeof (int) * (pwl+1));
		currSize = PWL();

		// fill the array with a random permutation
		fillArray(currSize);
		printf("Before sort: ");
		for (int i=0; i<currSize; i++){
			printf("%d ", arr[i]);
		}
		printf("\n");

		// sorts the array
		bubbleSort(arr, currSize);

		// increments the number of arrays sorted
		numSorted++;
		numSortedOneSec++;


		printf("After sort: ");
		for (int i=0; i<currSize; i++){
			printf("%d ", arr[i]);
		}
		printf("\n");
		printf("Number sorted: %llu \n", numSorted);
		printf("Number sorted per second: %d", numSortedOneSec);
		free(arr);
		//nextSize++;
		arr = NULL;
	}
	pthread_mutex_unlock(&locker);
	return NULL;
}


/*
void startSorting(){
	pthread_create(&tid, NULL, sorterThread, NULL);
}
*/
/*
int main()
{
	startSorting();
	pthread_join(tid, NULL);
	exit(0);
}
*/
