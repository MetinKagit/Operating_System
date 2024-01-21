/* multiplier.c
 * Name&Surname: Metin Kagit
 * Student_No: 1821221033
 * Mail: metin.kagit@stu.fsm.edu.tr
 * Lecture: Operating Systems - Project2
 * */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>

#define MULTIPLIER_PATH "/tmp/pipe_multiplier"
int fifo_multiplier;

int main() {
    printf("Hello, This is Multiplier!\n");

    float number1, number2, result;
    int process;
    bool isProcessContinuous = true;
   
   //open the pipe file
    fifo_multiplier = open(MULTIPLIER_PATH, O_WRONLY);
    if (fifo_multiplier == -1) {
        perror("Error opening named pipe for writing");
        close(fifo_multiplier);
        exit(EXIT_FAILURE);
     }

    while(isProcessContinuous){

        printf("Select a process (1: Adder, 2: Exit process): ");
        scanf("%d", &process);
           
        if (process == 1) {
            //---------------------------
            // Take the firs number from user and write it to pipe.
            printf("Please enter the first number: ");
            scanf("%f", &number1);

            //Write on pipe file
            write(fifo_multiplier, &number1, sizeof(float));
            

            //---------------------------
            // Take the second number from user and write it to pipe.
            printf("Please enter the second number: ");
            scanf("%f", &number2);

            //Write on pipe file
            write(fifo_multiplier, &number2, sizeof(float));
            
            //---------------------------
            // Calculating the results and write it to pipe.
            result = number1 * number2;

            //Write on pipe file
            write(fifo_multiplier, &result, sizeof(float));
            

        }else if(process == 2){
            printf("exit...\n");
            isProcessContinuous = false;
        }else{
            printf("Please select a valid process!\n");
            continue;;
        }

    }
     // Close the named pipe
    close(fifo_multiplier);
   
    return 0;
}
