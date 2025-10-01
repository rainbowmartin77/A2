// convert sequential sums to parallel

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_PROCESSES 4

int main(int argc, char *argv[]) 
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int *arr = malloc(N * sizeof(int));
    if (!arr) {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        arr[i] = i + 1;
    }

    // complete this
    // Global variable to track total
    long long total = 0;
    int divide;

    //Check for even division of arr
    divide = (N + NUM_PROCESSES - 1) / NUM_PROCESSES;


    // Arrays used to hold the divided values
    long long **dividedArray = malloc(NUM_PROCESSES * sizeof(long long *));
    for (int v = 0; v < NUM_PROCESSES; v++) {
        dividedArray[v] = malloc(divide * sizeof(long long));
    }

    //Loop to break up array of values into smaller arrays
    for (int i = 0; i < NUM_PROCESSES; i++) {
        for(int j = 0; j < divide; j++) {
            int index = i * divide + j;
            dividedArray[i][j] = (index < N) ? arr[index] : 0;
        }
    }

    // Create one pipe for each child process
    int pipefd[NUM_PROCESSES][2];

    for (int x = 0; x < NUM_PROCESSES; x++) {
        
        if (pipe(pipefd[x]) == -1) {
            perror("pipe");
            return 1;
        }

        // Create child process
        pid_t pid = fork();
        long long thisSum = 0;

        if (pid < 0) {
            printf("Child process %d failed to create", x);
            return 1;
        }
        else if (pid == 0) {
            // Close the read end of the pipe
            close(pipefd[x][0]);

            // Sum all the values in the x array
            for (int n = 0; n < divide; n++) {
                thisSum += dividedArray[x][n];
            }

            // Write thisSum to the pipe
            write(pipefd[x][1], &thisSum, sizeof(thisSum));
            close(pipefd[x][1]);  // Close write end
            exit(0);
        }
        else {
            // Parent closes the write end of the pipe
            close(pipefd[x][1]);
        }
    }

    while(wait(NULL) > 0);

    for (int q = 0; q < NUM_PROCESSES; q++) {
        long long currentSum;
        read(pipefd[q][0], &currentSum, sizeof(currentSum));
        close(pipefd[q][0]);
        total += currentSum;
    }

    printf("Total sum = %lld\n", total);

    for (int v = 0; v < NUM_PROCESSES; v++) {
        free(dividedArray[v]);
    }
    free(dividedArray);
    
    return 0;
}
