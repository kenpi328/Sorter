#ifndef SEGDISPLAY_H_
#define SEGDISPLAY_H_
#define _GNU_SOURCE
#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

#define I2C_DEVICE_ADDRESS 0x20

#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x14
#define REG_OUTB 0x15

//TESTTESTTEST
#define EXPORT "/sys/class/gpio/export"
#define LEFT_DIGIT_DIR "/sys/class/gpio/gpio61/direction"
#define RIGHT_DIGIT_DIR "/sys/class/gpio/gpio44/direction"
#define LEFT_DIGIT_VAR "/sys/class/gpio/gpio61/value"
#define RIGHT_DIGIT_VAR "/sys/class/gpio/gpio44/value"

void naSleep(); //function used to nanoseconds
void writeToFile(char* fileName, char* var); //used to write vars on filename( Export values)
void initDigit(); //initialize the segDisplay so make both digits are exported and both directions are "out"
void leftDigit(); //Turn on left digit
void rightDigit(); //turn on right digit
void digitPatterns(int digit, int* displayArr); //digit pattern from 0 to 9
void* displayRunner(void* arg); //Display thread
int initI2cBus(char* bus, int address);
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr);

#endif
