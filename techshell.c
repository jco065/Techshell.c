#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 1024

int main(){
    char line[MAX_LINE];

    while (1){

        // Step 1: Print the prompt symbol
        printf("tech_shell> ");

        // Step 2: Force the text out of the buffer memory
        fflush(stdout);

        // Step 3: Wait aand grab the user's input line
        if (fgets(line, sizeof(line), stdin) == NULL){
            printf("\nExisting shell...\n");
            break; // Safely exit if user triggers EOF (Ctrl+D)
        }

        // Temporary check: Print what the user types
        printf("You entered: %s", line);
    }

    return 0;
}