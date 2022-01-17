# power-board

Simple program for testing Pth Power Board hardware - ADC (ADS1015) and GPIOs.


# Platform info
This code is being tested on the following configuration:

Platform:   Raspberry Pi 4B+
OS:         Ubuntu Server 20.04.3 LTS 64-bit
OS version: 5.4.0-1044-raspi

# Dependencies

1. WiringPi (gpio) version 2.50 or higher. The official WiringPi library is unsupported, the active fork can be found here: https://github.com/WiringPi/WiringPi
2. g++ compiler and make:
   > apt-get install g++ make

# Notes

The documentation for WiringPi I2C library can be found here: http://wiringpi.com/reference/i2c-library/

# Build

To build the executable code, enter the following command:
> g++ power-board.c -l wiringPi -o power-board.o

# Usage

Run the program for the n numbers of ADC conversion and d delay:
> sudo ./power-board -n 10 -d 100

Run the program in the background without ADC printfs:
> sudo ./power-board -b
