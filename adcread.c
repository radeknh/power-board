#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> //for usleep


#define RTOP 113000
#define RBOTTOM 4700
#define MULT_VBAT ((RTOP+RBOTTOM)/RBOTTOM)
#define MULT_IDIG 4
#define MULT_ICH 2
#define MULT_TEMP 1

char values[7][6] = {     "VBAT1",         "VBAT2",         "IDIG ",          "TEMP1",
                          "TEMP2",         "ICH1 ",         "ICH2 " };
char fileNames[7][14] = { "1/in_voltage2", "1/in_voltage0", "1/in_voltage1", "0/in_voltage0",
                          "0/in_voltage1", "0/in_voltage3", "0/in_voltage2" };
float multipliers[7] = {  MULT_VBAT,       MULT_VBAT,       MULT_IDIG,        MULT_TEMP,
                          MULT_TEMP,       MULT_ICH,        MULT_ICH };
float scale[7] = {0};
int raw[7] = {0};
float results[7] = {0};

int main(void)
{
        char fn[100];
        for(int i=0; i<7; i++)
        {
                sprintf(fn, "/sys/bus/iio/devices/iio\:device%s_scale", fileNames[i]);
                FILE* fp = fopen(fn, "rb");
                if(!fp)
                {
                        perror("File opening failed");
                        return EXIT_FAILURE;
                }
                fscanf(fp, "%f", scale+i);
                fclose(fp);
                printf("%s: %f \n", values[i], scale[i]);
        }

        //read raw values
        while(1){
        //while(getc(stdin) != EOF){
        system("clear"); //in linux bash this clears the screen
                for(int i=0; i<7; i++)
                {
                        sprintf(fn, "/sys/bus/iio/devices/iio\:device%s_raw", fileNames[i]);
                        FILE* fp = fopen(fn, "rb");
                        if(!fp)
                        {
                                perror("File opening failed");
                                return EXIT_FAILURE;
                        }
                        fscanf(fp, "%d", raw+i);  //read value from device file
                        fclose(fp);
                        printf("%s: %d \n", values[i], raw[i]);  //print the raw values to screen
                        results[i] = raw[i]*multipliers[i]*scale[i];
                        printf("%s: %f \n", values[i], results[i]); //print the final results
                }
        usleep(200000);
        }

        //int status = system("echo test");
        //return status;
}
