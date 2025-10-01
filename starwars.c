#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

// Starting shield power level
int shield_power = 50;  

int main() {
    // List of Starwars Characters
    const char *character[4] = {"Luke", "Han", "Chewbacca", "Leia"};
    // Array of adjustment values corresponding to character
    int adjustment[4] = {25, 20, 30, 15};
    pid_t pid;

    printf("Millennium Falcon: Initial shield power level: %d%%\n\n", shield_power);

    // Create 4 child processes - 4 different characters adjusting shield power

            // Luke increases the shield power by 25
            // Han increases the shield power by 20
            // Chewbacca increases the shield power by 30
            // Leia increases the shield power by 15
               
    // Loop to create 4 child processes
    for (int x = 0; x < 4; x++) {
        // Create child process
        pid = fork();

        // Check that child process creation worked
        if (pid < 0) {
            printf("Child process %d failed to create", x);
            return 1;
        }

        // Successful child creation code
        else if(pid == 0) {
            // Calculate and print each character's shield power
            int power = shield_power + adjustment[x];
            printf("%s: Adjusting shields...\n", character[x]);
            printf("%s: Shield power level now at %d%%\n", character[x], power);
            // Exit the child process, do not continue to loop and create additional child processes
            exit(0);
        }

    }

    // Make parent process wait for the child processes
    for (int m = 0; m < 4; m++) {
        wait(NULL);
    }

    // Parent process reports final state
    printf("\nFinal shield power level on the Millennium Falcon: %d%%\n", shield_power);
    printf("\nMay the forks be with you!\n");
    return 0;
}
