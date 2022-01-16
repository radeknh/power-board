#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> //for usleep()
#include <time.h>   // not used yet - for timestamps in the future?
#include <string.h> //for strcmp()


#define RTOP        113000
#define RBOTTOM     4700
#define MULT_VBAT   ((RTOP+RBOTTOM)/RBOTTOM)    // = 25,04255319148936
#define MULT_IDIG   4
#define MULT_ICH    2
#define MULT_TEMP   1
#define NO_OF_FILES 7
#define FILEPATH_MAX_LENGTH 100
#define FILENAME_MAX_LENGTH 14
#define DEFAULT_CONV_DELAY  300000   //us
#define DEFAULT_CONV_NUMBER 1

const char values[NO_OF_FILES][11] =  //maximum value name length
        {     "VBAT1 [mV]",         "VBAT2 [mV]",         "IDIG [mA] ",          "TEMP1     ",  //names should have the same length (why?)
              "TEMP2     ",         "ICH1 [mA] ",         "ICH2 [mA] " };
const char fileNames[NO_OF_FILES][FILENAME_MAX_LENGTH] = 
        { "1/in_voltage2", "1/in_voltage0", "1/in_voltage1", "0/in_voltage0",
          "0/in_voltage1", "0/in_voltage3", "0/in_voltage2" };
const int multipliers[NO_OF_FILES] = 
        {  MULT_VBAT,       MULT_VBAT,       MULT_IDIG,        MULT_TEMP,
           MULT_TEMP,       MULT_ICH,        MULT_ICH };
float scale[NO_OF_FILES] = {0};
int raw[NO_OF_FILES] = {0};
float calculatedResults[NO_OF_FILES] = {0};
FILE* fp_scale[NO_OF_FILES];
FILE* fp_raw[NO_OF_FILES];
        
int readScales(void){
    char fileNameAndPath[FILEPATH_MAX_LENGTH];
    for(int i=0; i<NO_OF_FILES; i++){   
        sprintf(fileNameAndPath, "/sys/bus/iio/devices/iio\:device%s_scale", fileNames[i]);
        fp_scale[i] = fopen(fileNameAndPath, "rb");  // rb - binary files for read only
        if(!fp_scale[i]){ perror("File opening failed");  return EXIT_FAILURE; }
        fscanf(fp_scale[i], "%f", scale+i);
        printf("%s scale: %.1f \n", values[i], scale[i]);
        fclose(fp_scale[i]); //close each file after read
    }
}

int main(int argc, char *argv[]){  
    //set default values
    int conversionNumber = DEFAULT_CONV_NUMBER;
    int conversionDelay = DEFAULT_CONV_DELAY;  //ms
    
    // check flags
    for(int i=1; i<argc; i++){  //start from i=1 because argv[0] contains program name
        if(strcmp(argv[i-1], "-n") == 0){
            conversionNumber = atoi(argv[i]);
        }
        if(strcmp(argv[i-1], "-d") == 0){
            conversionDelay = 1000*atoi(argv[i]);
        }   
    }
    if((conversionNumber < 1) || ( conversionNumber > 2000)){
        printf("Please provide n between 1 and 2000 \n");
        return EXIT_FAILURE;
    }
    if((conversionDelay < 20000) || ( conversionDelay > 1000000)){
        printf("Please provide d between 20 and 1000 [in miliseconds]\n");
        return EXIT_FAILURE;
    }
    printf("Number of conversions: %d, delay: %dms\n", conversionNumber, conversionDelay);
    
    
    if(readScales() == EXIT_FAILURE) return EXIT_FAILURE;
    char fileNameAndPath[FILEPATH_MAX_LENGTH];
    //read raw values - "k" times
    for(int k=0; k<conversionNumber; k++){
        for(int i=0; i<NO_OF_FILES; i++){
            sprintf(fileNameAndPath, "/sys/bus/iio/devices/iio\:device%s_raw", fileNames[i]);
            fp_raw[i] = fopen(fileNameAndPath, "rb");  // rb - binary files for read only
            if(!fp_raw[i]){ perror("File opening failed");  return EXIT_FAILURE; }
            fscanf(fp_raw[i], "%d", raw+i);  //read value from device file
            calculatedResults[i] = raw[i]*multipliers[i]*scale[i];
            fclose(fp_raw[i]);
        }
        for(int i=0; i<NO_OF_FILES; i++){
             printf("%s: %5.2f | raw: %d\n", values[i], calculatedResults[i], raw[i]);  //print the values to the screen
        }
        printf("====================\n");
        usleep(conversionDelay);
    }
    //int status = system("echo test");
    //return status;
}
