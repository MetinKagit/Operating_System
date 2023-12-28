/* trmn.c
 * Name&Surname: Metin Kagit
 * Student_No: 1821221033
 * Mail: metin.kagit@stu.fsm.edu.tr
 * Lecture: Operating Systems - Project1
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>


int main() {
    //Variables
    int num_children = 4;
    pid_t child_pids[num_children];
    int process;
    int status;
    int num1, num2;
    char ad_fd0_str[12], ad_fd1_str[12], di_fd0_str[12], di_fd1_str[12], mu_fd0_str[12], mu_fd1_str[12], sb_fd0_str[12], sb_fd1_str[12];
    char ad_pfd0_str[12], ad_pfd1_str[12], di_pfd0_str[12], di_pfd1_str[12], mu_pfd0_str[12], mu_pfd1_str[12], sb_pfd0_str[12], sb_pfd1_str[12];
    char buf[1024];
    char sum_str[12], div_str[12], mul_str[12], sub_str[12];
    bool isProcessContinue = true;

    //Pipes
    int pipe_adder1[2];
    int pipe_adder2[2];

    int pipe_divider1[2];
    int pipe_divider2[2];

    int pipe_multiplier1[2];
    int pipe_multiplier2[2];

    int pipe_subtractor1[2];
    int pipe_subtractor2[2];

    // Initialize the pipes
    if (pipe(pipe_adder1) == -1 || pipe(pipe_adder2) == -1 || pipe(pipe_divider1) == -1 || pipe(pipe_divider2) == -1
        || pipe(pipe_multiplier1) == -1 || pipe(pipe_multiplier2) == -1 || pipe(pipe_subtractor1) == -1 || pipe(pipe_subtractor2) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    // Loop to fork 4 child processes
    for (int i = 0; i < num_children; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            // Handle fork error
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            // Executing the subprocesses by the index numver. (e.g: i == 0 -> ./adder)
            char *args[5];
            printf("Child process %d with PID %d is ready.\n", i + 1, getpid());

            if(i == 0){
                close(pipe_adder1[1]); // close the write end of the pipe
                //Stringify the pipes
                snprintf(ad_fd0_str, sizeof(ad_fd0_str), "%d", pipe_adder1[0]);
                snprintf(ad_fd1_str, sizeof(ad_fd1_str), "%d", pipe_adder1[1]);
                snprintf(ad_pfd0_str, sizeof(ad_pfd0_str), "%d", pipe_adder2[0]);
                snprintf(ad_pfd1_str, sizeof(ad_pfd1_str), "%d", pipe_adder2[1]);

                //Add them to args to send child process
                args[0] = ad_fd0_str;
                args[1] = ad_fd1_str;
                args[2] = ad_pfd0_str;
                args[3] = ad_pfd1_str;
                args[4] = NULL;

                //Execute the child process
                execve("./adder", args, NULL);
                

            }else if(i == 1){
                close(pipe_divider1[1]); // close the write end of the pipe
                snprintf(di_fd0_str, sizeof(di_fd0_str), "%d", pipe_divider1[0]);
                snprintf(di_fd1_str, sizeof(di_fd1_str), "%d", pipe_divider1[1]);
                snprintf(di_pfd0_str, sizeof(di_pfd0_str), "%d", pipe_divider2[0]);
                snprintf(di_pfd1_str, sizeof(di_pfd1_str), "%d", pipe_divider2[1]);

                args[0] = di_fd0_str;
                args[1] = di_fd1_str;
                args[2] = di_pfd0_str;
                args[3] = di_pfd1_str;
                args[4] = NULL;

                execve("./divider", args, NULL);
                
                
            }else if(i == 2){
                close(pipe_multiplier1[1]); // close the write end of the pipe
                snprintf(mu_fd0_str, sizeof(mu_fd0_str), "%d", pipe_multiplier1[0]);
                snprintf(mu_fd1_str, sizeof(mu_fd1_str), "%d", pipe_multiplier1[1]);
                snprintf(mu_pfd0_str, sizeof(mu_pfd0_str), "%d", pipe_multiplier2[0]);
                snprintf(mu_pfd1_str, sizeof(mu_pfd1_str), "%d", pipe_multiplier2[1]);

                args[0] = mu_fd0_str;
                args[1] = mu_fd1_str;
                args[2] = mu_pfd0_str;
                args[3] = mu_pfd1_str;
                args[4] = NULL;

                execve("./multiplier", args, NULL);
                
                
            }else{
                close(pipe_subtractor1[1]); // close the write end of the pipe
                snprintf(sb_fd0_str, sizeof(sb_fd0_str), "%d", pipe_subtractor1[0]);
                snprintf(sb_fd1_str, sizeof(sb_fd1_str), "%d", pipe_subtractor1[1]);
                snprintf(sb_pfd0_str, sizeof(sb_pfd0_str), "%d", pipe_subtractor2[0]);
                snprintf(sb_pfd1_str, sizeof(sb_pfd1_str), "%d", pipe_subtractor2[1]);

                args[0] = sb_fd0_str;
                args[1] = sb_fd1_str;
                args[2] = sb_pfd0_str;
                args[3] = sb_pfd1_str;
                args[4] = NULL;

                execve("./subtractor", args, NULL);
                
            }

            // Perform child process tasks

            exit(EXIT_SUCCESS);  // Terminate child process
        }else{
            //get child process id
            child_pids[i] = pid;
        }
    }


    //-----------------------------
    // Parent process area ↓

    sleep(1);
    printf("Parent process with PID %d is preparing, please wait...\n", getpid());
    sleep(4);
    printf("--------------Welcome----------------\n");
    while (isProcessContinue){
        //Select process
        printf("Enter '1' for - Addition\n"
               "Enter '2' for - Division\n"
               "Enter '3' for - Multiplication\n"
               "Enter '4' for - subtractor\n"
               "Enter '5' for - Exit\n");
        printf("----------------------------------------\n");
        printf("Please selecct a process: ");
        scanf("%d", &process);

        //If 5 is selected, terminate the program.
        if(process == 5){
            printf("Program Closing...\n");
            isProcessContinue = false;

            // Terminate all child processes
            for (int i = 0; i < num_children; ++i) {
                if (kill(child_pids[i], SIGTERM) == -1) {
                    perror("kill");
                    exit(EXIT_FAILURE);
                }else{
                    printf("Child process %d terminated \n", child_pids[i]);
                }
            }
            
            //close the read end and write end of the pipes
            close(pipe_adder1[0]);
            close(pipe_adder1[1]); 
            close(pipe_adder2[1]);
            close(pipe_adder2[0]);

            close(pipe_divider1[0]);
            close(pipe_divider1[1]);
            close(pipe_divider2[1]);
            close(pipe_divider2[0]);

            close(pipe_multiplier1[0]);
            close(pipe_multiplier1[1]);
            close(pipe_multiplier2[1]);
            close(pipe_multiplier2[0]);

            close(pipe_subtractor1[0]);
            close(pipe_subtractor1[1]);
            close(pipe_subtractor2[1]);
            close(pipe_subtractor2[0]);


        }else{
            if(process == 1){
                //Adder 
                //Ask for two number and combine them into a string
                printf("Please enter two numbers separated by a space: ");
                scanf("%d %d", &num1, &num2);
                snprintf(buf, sizeof(buf), "%d %d %d", num1, num2, process);

                //write the string into pipe
                write(pipe_adder1[1], buf, sizeof(buf));

                //read the result from pipe and print it 
                read(pipe_adder2[0], sum_str, sizeof(sum_str));
                printf("------------------\n");
                printf("Result: %d + %d = %s\n", num1, num2, sum_str);
                printf("------------------\n");
                
            }else if(process == 2){
                //Divide
                //Ask for two number and combine them into a string
                printf("Please enter divided and divisor numbers separated by a space: ");
                scanf("%d %d", &num1, &num2);
                //Handle zero devisor error
                if(num2 == 0){
                    printf("You cannot assign zero to the divisor number!\n");
                    printf("------------------\n");
                    continue;
                }else{
                    snprintf(buf, sizeof(buf), "%d %d %d", num1, num2, process);

                    //write the string into pipe
                    write(pipe_divider1[1], buf, sizeof(buf));

                    //read the result from pipe and print it 
                    read(pipe_divider2[0], div_str, sizeof(div_str));
                    printf("------------------\n");
                printf("Result: %d / %d = %s\n", num1, num2, div_str);
                printf("------------------\n");
                    
                }

            }else if(process == 3){
                //Multiplier
                //Ask for two number and combine them into a string
                printf("Please enter two numbers separated by a space: ");
                scanf("%d %d", &num1, &num2);
                snprintf(buf, sizeof(buf), "%d %d %d", num1, num2, process);

                //write the string into pipe
                write(pipe_multiplier1[1], buf, sizeof(buf));

                //read the result from pipe and print it 
                read(pipe_multiplier2[0], mul_str, sizeof(mul_str));
                printf("------------------\n");
                printf("Result: %d * %d = %s\n", num1, num2, mul_str);
                printf("------------------\n");
              
            }else if(process == 4){
                //Subtractor
                //Ask for two number and combine them into a string
                printf("Please enter two numbers separated by a space: ");
                scanf("%d %d", &num1, &num2);
                snprintf(buf, sizeof(buf), "%d %d %d", num1, num2, process);

                //write the string into pipe
                write(pipe_subtractor1[1], buf, sizeof(buf));

                //read the result from pipe and print it 
                read(pipe_subtractor2[0], sub_str, sizeof(sub_str));
                printf("------------------\n");
                printf("Result: %d - %d = %s\n", num1, num2, sub_str);
                printf("------------------\n");
                
            }else{
                //Preventing user to ender invalid input
                while (getchar() != '\n');
                printf("Please select a valid process!\n");
                sleep(1);
            }
        }

    }

    //wait for the child procecss
    for (int i = 0; i < num_children; ++i) {
        wait(&status);
    }

    printf("Parent process terminating... \n");
    printf("-------- ❤️  SEE YOU LATER ❤️  --------\n");
    return 0;
}