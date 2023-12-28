/* divider.c
 * Name&Surname: Metin Kagit
 * Student_No: 1821221033
 * Mail: metin.kagit@stu.fsm.edu.tr
 * Lecture: Operating Systems - Project1
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

//This function take the readed string and seperate it by integer numbers.
// Then Make the division process.
void extractAndDivide(const char *input, float *result) {
    float num1 = 0.0, num2 = 0.0, choice = 0.0;
    int isNum1 = 1, isNum2 = 0, isChoice = 0; // Flags to indicate which number we are currently parsing

    //The for loop traverses each element until it reaches the end of the string.
    for (int i = 0; input[i] != '\0'; i++) {
        if ((input[i] >= '0' && input[i] <= '9') || input[i] == '.') {
            // If the character is a digit or '.', update the the number
            if (isNum1) {
                if (input[i] != '.') {
                    //Multiply by 10, so that the new digit is ready to be added.
                    //"input[i] - '0'" Statement convert the value to int by The ASCII value of the character '0' is subtracted from the ASCII value.
                    num1 = num1 * 10 + (input[i] - '0');
                } else {
                    isNum1 = 0;
                    isNum2 = 1;
                }
            } else if (isNum2) {
                if (input[i] != '.') {
                    num2 = num2 * 10 + (input[i] - '0');
                } else {
                    isNum2 = 0;
                    isChoice = 1;
                }
            } else if (isChoice) {
                if (input[i] != '.') {
                    choice = choice * 10 + (input[i] - '0');
                }
            }
            // If the character is a space, 
            //switch to the value to determine which variable is next in order
        } else if (input[i] == ' ') {
            if (isNum1) {
                isNum1 = 0;
                isNum2 = 1;
            } else if (isNum2) {
                isNum2 = 0;
                isChoice = 1;
            }
        }
    }

    //Caclculate and store the results
    if (num2 != 0) {
        result[0] = num1;
        result[1] = num2;
        result[2] = num1 / num2;
    } else {
        // Handle division by zero
        result[0] = num1;
        result[1] = num2;
        result[2] = -1; //means error
    }
    result[3] = choice;
}

int main(int argc, char *argv[]) {
    float result[4];
    char div_str[12];
    char buf[1024];
    bool temp = true;
    int fd0 = atoi(argv[0]);
    int pfd0 = atoi(argv[2]); // read
    int pfd1 = atoi(argv[3]); // write

    while (temp) {
        //read from pipe
        read(fd0, buf, sizeof(buf));
        extractAndDivide(buf, result);

        //Convert result to string and writ it into pipe
        sprintf(div_str, "%.2f", result[2]);
        write(pfd1, div_str, sizeof(div_str));
        sleep(3);
    }

    close(fd0); // close the read end of the pipe
    close(pfd0);
    close(pfd1); // close the read end of the pipe

    return 0;
}
