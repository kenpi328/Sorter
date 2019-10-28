#include "potDriver.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#define A2D_FILE_VOLTAGE0  "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define A2D_VOLTAGE_REF_V  1.8
#define A2D_MAX_READING    4095
#define PIECEWISE_NUM_POINTS 10

const float PIECEWISE_READING[] = {0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4100};//x-axis
const float PIECEWISE_ARRAYSIZE[] = {1, 20, 60, 120, 250, 300, 500, 800, 1200, 2100}; //y-axis


//Read the voltage of potentiomenter
int getVoltage0Reading()
{
// Open file
  FILE *f = fopen(A2D_FILE_VOLTAGE0, "r");
  if (!f) {
    printf("ERROR: Unable to open voltage input file. Cape loaded?\n");
    printf("       Check /boot/uEnv.txt for correct options.\n");
    exit(-1);
  }
  // Get reading
  int a2dReading = 0;
  int itemsRead = fscanf(f, "%d", &a2dReading);
  if (itemsRead <= 0) {
    printf("ERROR: Unable to read values from voltage input file.\n");
    exit(-1);
  }
  // Close file
  fclose(f);
  return a2dReading;

}
int getClosestIdx(int value){ //get the closest index from
  for(int i = 0; i < PIECEWISE_NUM_POINTS; i++){
    if(value < PIECEWISE_READING[i]){
      return i; //index of the second point
    }
  }
  return PIECEWISE_NUM_POINTS -1;
}
int PWL(){//calculate the current piece wise linear function
  int s = getVoltage0Reading();
  int idx = getClosestIdx(s); //idx will never be 0
  float a = PIECEWISE_READING[idx-1];
  float b = PIECEWISE_READING[idx];
  float n = PIECEWISE_ARRAYSIZE[idx];
  float m = PIECEWISE_ARRAYSIZE[idx-1];
  return floor(((s-a)/(b-a))*(n-m) + m);
}
/*
void* potRunner(void* arg)
{
  while (true) {
    int reading = getVoltage0Reading();
    //double voltage = ((double)reading / A2D_MAX_READING) * A2D_VOLTAGE_REF_V;
    int pwl = PWL();
    printf("Value %5d ==> Array Size %5d\n", reading, pwl);
    //printf("Value %5d ==> %5.2fV\n", reading, pwl);
  }
    //return 0;

    pthread_exit(0);
    return 0;
}
*/
