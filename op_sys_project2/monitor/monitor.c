/* monitor.c
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

#define ADDER_PATH "/tmp/pipe_adder"
#define DIVIDER_PATH "/tmp/pipe_divider"
#define MULTIPLIER_PATH "/tmp/pipe_multiplier"
#define SUBTRACTOR_PATH "/tmp/pipe_subtractor"
#define BUFFER_SIZE 128

int fifo_adder, fifo_divider, fifo_multiplier, fifo_subtractor;
bool isProgramRunning = true;

struct ThreadData {
    float fifo_fd;
    char symbol;
    const char* name;
};

pthread_mutex_t lock;

void* monitoring_function(void* arg) {
    struct ThreadData *data = (struct ThreadData *)arg;
    printf("%s is ready.\n", data->name);
    ssize_t bytesRead;
    float number;
    float array[3];
    int count = 0;
    bool isProcessContinue = true;
    bool flag = true;
    
    while (isProcessContinue) {
        //read from pipe and if it writtened enter the critical region
        bytesRead = read(data->fifo_fd, &number, sizeof(number));
        if (bytesRead > 0) {
            //Lock the critical section
            if(flag){
                pthread_mutex_lock(&lock);
                printf("%s entered the critical region\n", data->name);
                printf("-------------------------------------\n");
                flag = false;
            }
            //print received numbers and increase the count by 1
            printf("Received float number: %.2f\n", number);
            array[count] = number;
            count++;
            // if count equal 3 it means process done, so print the result
            if (count == 3) {
                printf("%.2f %c %.2f = %.2f\n", array[0], data->symbol, array[1], array[2]);
                count = 0;
                printf("-------------------------------------\n");
                printf("%s left the critical region\n", data->name);
                flag = true;
                //Unlock the critical section
                pthread_mutex_unlock(&lock);
            }
        }
    }

    return NULL;
}


int main() {

    printf("Hello, This is Monitor!\n");
    pthread_t thread_adder, thread_divider, thread_multiplier, thread_subtractor;

    if(pthread_mutex_init(&lock, NULL) != 0){
        printf("mutex init error \n");
        return 1;
    }

    //Initialize thread data
    printf("Pipes are initializing...\n");

    //Criating pipes
    mkfifo(ADDER_PATH, 0666);
    mkfifo(DIVIDER_PATH, 0666);
    mkfifo(MULTIPLIER_PATH, 0666);
    mkfifo(SUBTRACTOR_PATH, 0666);

    //Open pipes
    fifo_adder = open(ADDER_PATH, O_RDONLY);
      if (fifo_adder == -1) {
        perror("Error opening named pipe for reading");
        return 1;
    }
    fifo_divider = open(DIVIDER_PATH, O_RDONLY);
      if (fifo_divider == -1) {
        perror("Error opening named pipe for reading");
        return 1;
    }
    fifo_multiplier = open(MULTIPLIER_PATH, O_RDONLY);
      if (fifo_multiplier == -1) {
        perror("Error opening named pipe for reading");
        return 1;
    }
    fifo_subtractor = open(SUBTRACTOR_PATH, O_RDONLY);
      if (fifo_subtractor == -1) {
        perror("Error opening named pipe for reading");
        return 1;
    }


    //Creating threads data structure and assign the their values
    struct ThreadData data_adder;
    data_adder.fifo_fd = fifo_adder;
    data_adder.symbol = '+';
    data_adder.name = "Adder";

    struct ThreadData data_divider;
    data_divider.fifo_fd = fifo_divider;
    data_divider.symbol = '/';
    data_divider.name = "Divider";

    struct ThreadData data_multiplier;
    data_multiplier.fifo_fd = fifo_multiplier;
    data_multiplier.symbol = '*';
    data_multiplier.name = "Multiplier";

    struct ThreadData data_subtroctor;
    data_subtroctor.fifo_fd = fifo_subtractor;
    data_subtroctor.symbol = '-';
    data_subtroctor.name = "Subtractor";
   

    // Create thread for monitoring the named pipe
    printf("Threads are being created...\n");
    if (pthread_create(&thread_adder, NULL, monitoring_function, (void *)&data_adder) != 0) {
        perror("pthread_create thread_adder");
        close(fifo_adder);
        return 1;
    }

    if (pthread_create(&thread_divider, NULL, monitoring_function, (void *)&data_divider) != 0) {
        perror("pthread_create thread_adder");
        close(fifo_divider);
        return 1;
    }

    if (pthread_create(&thread_multiplier, NULL, monitoring_function, (void *)&data_multiplier) != 0) {
        perror("pthread_create thread_adder");
        close(fifo_multiplier);
        return 1;
    }

    if (pthread_create(&thread_subtractor, NULL, monitoring_function, (void *)&data_subtroctor) != 0) {
        perror("pthread_create thread_adder");
        close(fifo_subtractor);
        return 1;
    }

    
    //Wait for the monitor thread to finish (This will never be reached in this example)
    pthread_join(thread_adder, NULL);
    pthread_join(thread_divider, NULL);
    pthread_join(thread_multiplier, NULL);
    pthread_join(thread_subtractor, NULL);
    
    // Close the named pipe
    close(fifo_adder);
    close(fifo_divider);
    close(fifo_multiplier);
    close(fifo_subtractor);
     
    // Remove the named pipe   
    unlink(ADDER_PATH); 
    unlink(DIVIDER_PATH); 
    unlink(MULTIPLIER_PATH); 
    unlink(SUBTRACTOR_PATH);

    pthread_mutex_destroy(&lock);

    printf("Program is terminating...\n");
    
    return 0;
}
