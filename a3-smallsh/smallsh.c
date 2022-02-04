/******************************************************************************
*   The purpose of this program is to run a small shell within a shell.   
*
*   Author: Liam Duncan
******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

// Function Declarations
int prompt_loop(); 
char **parse_input(char *input, int *token_count, char **input_arr); 
void free_all(char** arr, pid_t *pid_arr);
int cd(char **input_arr, int token_count); 
int handle_else(char **input_arr, int token_count, pid_t *pid_arr); 

int main(int argc, char* argv[])
{

    prompt_loop();

  
    return 0; 
}


int prompt_loop()
{
    char **input_arr; 

    // make an array for all the running process ids
    pid_t *pid_arr; 
    pid_arr = malloc(40 * sizeof(pid_t)); 

    // Initialize the array with NULLs 
    int i=0; 
    for(i; i<40; i++)
        pid_arr[i] = NULL; 
    

    while(1)
    {
        char input[2050]; 
        
        input_arr = malloc(512 * sizeof(char*)); 
        int i=0; 
        for(i; i<512; i++)
            input_arr[i] = malloc(2048 * sizeof(char)); 
        

        int token_count; 

        // Print prompt 
        printf("\n: ");
        fflush(stdout); 

        // Get the line
        fgets(input, 2050, stdin); 

        // // Print it back out for debugging
        // printf("\n%s", input);
        // fflush(stdout);

        // handle empty line or comment 
        if((input[0] == '\n') || (input[0] == '#'))
        {
            // printf("That string was empty or a comment\n\n");
            // fflush(stdout);
            continue; 
        }

        // parse the input into an array
        input_arr = parse_input(input, &token_count, input_arr); 

        // // print out how many items are in the array
        // printf("token count = %d\n", (token_count)); 
        // fflush(stdout); 

        // // print out the items in the array
        // int j=0; 
        // for(j; j < (token_count); j++)
        // {
        //     printf("%s ", input_arr[j]);
        //     fflush(stdout); 
        // }

        if(strcmp(input_arr[0], "exit") == 0)
        {
            printf("trying to exit\n"); 
            fflush(stdout); 
            break;
        } 
        else if(strcmp(input_arr[0], "cd") == 0)
            cd(input_arr, token_count); 
        // else if(strcmp(input_arr[0], "status"))
        //     status(input_arr, token_count); 
        else
            handle_else(input_arr, token_count, pid_arr); 


        // printf("\n\n"); 
        // fflush(stdout);

    }

    free_all(input_arr, pid_arr); 

    return 0; 
}


int handle_else(char **input_arr, int token_count, pid_t *pid_arr)
{
    int child_status; 

    input_arr[token_count] = NULL;

    pid_t spawnPid = fork(); 

    switch(spawnPid)
    {
    case -1:
            perror("fork()\n");
            return(1); 
            break; 
    case 0: 
            execvp(input_arr[0], input_arr); 
            perror("execvp"); 
            exit(2); 
            break; 
    default: 
            spawnPid = waitpid(spawnPid, &child_status, 0); 
            break; 
    }

    return 0; 
}


char **parse_input(char *input, int *token_count, char **input_arr)
{
    // char input_arr[512][2048];

    char delim[] = " \n"; 
    char *saveptr; 
    char *token; 

    int i=0; 

    token = strtok_r(input, delim, &saveptr); 
    strcpy(input_arr[0], token); 

    if(token != NULL)
        *token_count = 1; 

    while(token != NULL)
    {
        i++; 
        token = strtok_r(NULL, delim, &saveptr); 

        if(token == NULL)
            continue; 
        
        strcpy(input_arr[i], token);
        *token_count = *token_count+1; 
    }    

    return input_arr; 
}


int cd(char **input_arr, int token_count)
{
    char buf[2048]; 

    printf("cwd is : %s\n", getcwd(buf, 2048)); 
    fflush(stdout); 

    if(token_count == 1)
    {
        char *home_env = getenv("HOME"); 
        chdir(home_env); 
    }
    else if(token_count == 2)
    {
        chdir(input_arr[1]); 
    }
    else
    {
        printf("Looks like you input more than 2 arguments for cd. Command will be ran for the first argument and second will be ignored\n");
        fflush(stdout); 
        chdir(input_arr[1]); 
    }

    printf("cwd is : %s\n", getcwd(buf, 2048)); 
    fflush(stdout);   
   
    return 0; 
}

void free_all(char** arr, pid_t *pid_arr)
{
    int i=0; 
    for(i; i<512; i++)
        free(arr[i]);
    
    free(arr);
    free(pid_arr); 
}