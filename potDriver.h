#ifndef POTDRIVER_H_
#define POTDRIVER_H_

int getVoltage0Reading(); //read analog voltage from pot
int getClosestIdx(int value); //get the closest value in the array
int PWL(); //calculate piece wise liner to get the digital balue
//void* potRunner(void* arg); //pot Thread

#endif
