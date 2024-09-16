#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1000
#define MAX_ARGS_LENGTH 100

// Function to trim leading and trailing spaces
char* trim(char* str) 
{
    // Trim leading spaces
    while (*str == ' ') 
    {
        str++;
    }
    // Trim trailing spaces
    if (*str == 0) 
    {
        return str;
    }
    char* end = str + strlen(str) - 1;
    while (end > str && *end == ' ') 
    {
        end--;
    }
    *(end + 1) = 0;
    return str;
}

int main() 
{
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS_LENGTH + 1];  // +1 for NULL terminator
    char *path = getenv("PATH");
    pid_t pid;
    int stat;
    
    while (1) 
    {
        printf("Enter a command (or 'leave' to exit): ");
        fgets(command, MAX_COMMAND_LENGTH, stdin);
        
        // Remove the newline character from the command string
        char *newline = strchr(command, '\n');
        if (newline != NULL) 
        {
            *newline = '\0';
        }
        
        // Trim leading and trailing spaces
        char *trimmed_command = trim(command);
        if (strcmp(trimmed_command, "leave") == 0) 
        {
            printf("Exiting program.\n");
            exit(0);
        }
        
        // Parse the command into arguments
        int i = 0;
        char *token = strtok(trimmed_command, " ");
        while (token != NULL && i < MAX_ARGS_LENGTH) 
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;  // Set the last element to NULL for execv
        
        if ((pid = fork()) == 0) 
        {
            // Child process
            char *part;
            char full_path[MAX_COMMAND_LENGTH];
            char *path_copy = strdup(path);  
            
            part = strtok(path_copy, ":");
            while (part != NULL) 
            {
                strcpy(full_path, part);
                strcat(full_path, "/");
                strcat(full_path, args[0]);
                execv(full_path, args);
                part = strtok(NULL, ":");
            }
            
            // Free the copied path
            free(path_copy);
            
            // If execv returns, it must have failed
            printf("Error: Command '");
            for (int j = 0; j < i; j++) 
            {
                printf("%s", args[j]);
                if (j < i - 1) 
                {
                    printf(" ");
                }
            }
            printf("' not found.\n");
            exit(1);
        } 
        else 
        {
            // Parent process
            wait(&stat);
        } 
    }
    return 0;
}
