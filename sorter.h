#ifndef SORTER_H_
#define SORTER_H_

void lock(); // Thread synchronization - so that the array cannot be manipulated by outside functions
void unlock();
int numSortedperSec(); // Returns the number of sorted arrays in the past second
void fillArray(int size); // Fills an array with a random permutation of ints
void swap(int *x, int *y); // Swaps the values of two ints
void bubbleSort(int arr[], int n); // Sorting algorithm used to sort ints in an array
void* sorterThread(void* arg); // Main sorting thread that is called by main and maintains the sorting of the array
#endif
