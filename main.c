#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include <pthread.h>
#include <stdbool.h>
#include "segDisplay.h"
//#include "segDisplay.c"
#include "potDriver.h"
#include "sorter.h"
//POT gets array sizeof
//segDis display how much elements are sorted so far

// Assume pins already configured for I2C:
//   # config-pin P9_18 i2c
//   # config-pin P9_17 i2c
//void* displayRunner(void* arg);
long long ArrSize = 0;// global var that saves what to display

int main()
{
  //Start the threads
  pthread_t disTid, sorTid;
  pthread_attr_t disAttr;
  pthread_attr_init(&disAttr);
  //startSorting(); //From sorter


//  pthread_attr_init(&potAttr);
  pthread_create(&disTid, &disAttr, (void*)displayRunner, NULL);
  pthread_create(&sorTid, NULL, sorterThread, NULL);
//  pthread_create(&potTid, &potAttr, (void*)potRunner, NULL);
//do what ever we want here

 //End the threads
  pthread_join(disTid, NULL);
  pthread_join(sorTid, NULL); //From sorter
//  pthread_join(potTid, NULL);
  return 0;

}
