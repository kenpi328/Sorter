#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include "segDisplay.h"
#include "potDriver.h"
#include "sorter.h"
void naSleep(){
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = 5000000;
	nanosleep(&ts, NULL);
}

void writeToFile(char* fileName, char* var) //export the files
{
	//Use fopen() to open the file for write access.
	FILE *pfile = fopen(fileName, "w");
	if (pfile == NULL){
		printf("ERROR: Unable to open file.\n");
		exit(1);
	}
	//Write tot data to the file using fprintf():
	fprintf(pfile, "%s", var);
	//Close the file using fclose():
	fclose(pfile);

}
//initialize the digits export and make sure the directios are "out"
void initDigit(){
	writeToFile(EXPORT, "61");
	writeToFile(EXPORT, "44");
	writeToFile(LEFT_DIGIT_DIR,"out");
	writeToFile(RIGHT_DIGIT_DIR,"out");

}
//turn only the left digit on
void leftDigit(){
	writeToFile(LEFT_DIGIT_VAR,"1");
	writeToFile(RIGHT_DIGIT_VAR,"0");
}

//turn only the right digit on
void rightDigit(){
	writeToFile(LEFT_DIGIT_VAR,"0");
	writeToFile(RIGHT_DIGIT_VAR,"1");

}
//digit patterns to get digits from 0 to 9
void digitPatterns(int digit, int* displayArr){ //first argument digit you want to display

	if(digit == 0){
		displayArr[0] = 0xA1;
		displayArr[1] = 0x86;

	}
	else if(digit == 1){
		displayArr[0] = 0x80;
		displayArr[1] = 0x02;

	}
	else if (digit == 2){
		displayArr[0] = 0x31;
		displayArr[1] = 0x0E;

	}
	else if (digit == 3){
		displayArr[0] = 0xB0;
		displayArr[1] = 0x0E;

	}
	else if (digit == 4){
		displayArr[0] = 0x90;
		displayArr[1] = 0x8A;

	}
	else if (digit == 5){
		displayArr[0] = 0xB0;
		displayArr[1] = 0x8c;

	}
	else if (digit == 6){
		displayArr[0] = 0xB1;
		displayArr[1] = 0x8C;

	}
	else if (digit == 7){
		displayArr[0] = 0x80;
		displayArr[1] = 0x06;

	}
	else if (digit == 8){
		displayArr[0] = 0xB1;
		displayArr[1] = 0x8E;

	}
	else{
		displayArr[0] = 0x90;
		displayArr[1] = 0x8E;

	}
}


//initialize the buses
int initI2cBus(char* bus, int address)
{
	int i2cFileDesc = open(bus, O_RDWR);
	if (i2cFileDesc < 0) {
		printf("I2C DRV: Unable to open bus for read/write (%s)\n", bus);
		perror("Error is:");
		exit(-1);
	}

	int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
	if (result < 0) {
		perror("Unable to set I2C device to slave address.");
		exit(-1);
	}
	return i2cFileDesc;
}

void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
	unsigned char buff[2];
	buff[0] = regAddr;
	buff[1] = value;
	int res = write(i2cFileDesc, buff, 2);
	if (res != 2) {
		perror("Unable to write i2c register");
		exit(-1);
	}
}

unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr)
{
	// To read a register, must first write the address
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
	if (res != sizeof(regAddr)) {
		perror("Unable to write i2c register.");
		exit(-1);
	}

	// Now read the value and return it
	char value = 0;
	res = read(i2cFileDesc, &value, sizeof(value));
	if (res != sizeof(value)) {
		perror("Unable to read i2c register");
		exit(-1);
	}
	return value;
}
void* displayRunner(void *arg)
{
	printf("Drive display (assumes GPIO #61 and #44 are output and 1\n");
	int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
	initDigit();//TESTTESTTEST

		while(1){

		writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
		writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);


		int* numDisplayFir = (int*) malloc(2*sizeof(int));
		int* numDisplaySec = (int*) malloc(2*sizeof(int));
		//int pwl = PWL();
		int pwl = PWL();
		if (pwl > 99){ // if the value is larget than 99 set it to 99
			pwl = 99;
		}
		else if(pwl < 0){ //if the value is less than 0 set it to 0
			pwl = 0;
		}
		int tenDig = pwl / 10;
		int firDig = pwl % 10;
		digitPatterns(tenDig, numDisplayFir);
		digitPatterns(firDig, numDisplaySec);
		leftDigit();
		writeI2cReg(i2cFileDesc, REG_OUTA, numDisplayFir[0]);
		writeI2cReg(i2cFileDesc, REG_OUTB, numDisplayFir[1]);

		naSleep(); //sleep for a short time so the digit is visible by human elys

		rightDigit();
		writeI2cReg(i2cFileDesc, REG_OUTA, numDisplaySec[0]);
		writeI2cReg(i2cFileDesc, REG_OUTB, numDisplaySec[1]);
		naSleep();

		free(numDisplayFir);
		free(numDisplaySec);
	}
  //writeToFile(LEFT_DIGIT_VAR,"1");
	//writeToFile(RIGHT_DIGIT_VAR,"1");
	// Read a register:
	unsigned char regVal = readI2cReg(i2cFileDesc, REG_OUTA);
	printf("Reg OUT-A = 0x%02x\n", regVal);

	// Cleanup I2C access;
	close(i2cFileDesc);
  pthread_exit(0);
//	return 0;
}
