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
int redirect_out(char **input_arr, int i);
char **expansion(char **input_arr, int token_count, pid_t smallsh_pid); 

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

    // // Initialize the array with NULLs 
    // int i=0; 
    // for(i; i<40; i++)
    //     strcpy(pid_arr[i], "\0"); 

    pid_t smallsh_pid = getppid(); 
    

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
            continue; 

        
        

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

        printf("\n\nExpansion Test: \n"); 
        fflush(stdout); 

        // print out the items in the array
        int j=0; 
        for(j; j < (token_count); j++)
        {
            printf("%s ", input_arr[j]);
            fflush(stdout); 
        }
        printf("\n");
        fflush(stdout); 

        input_arr = expansion(input_arr, token_count, smallsh_pid); 

        // print out the items in the array
        j=0; 
        for(j; j < (token_count); j++)
        {
            printf("%s ", input_arr[j]);
            fflush(stdout); 
        }

        find_special(input_arr, token_count); 

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

int find_special(char **input_arr, int token_count)
{
    int j=0; 
    for(j; j<token_count; j++)
    {
        printf("%s ", input_arr[j]); 
        fflush(stdout); 
    }
    int i = 0; 

    int strcm = strcmp(input_arr[i], ">"); 

    for(i; i<token_count; i++)
    {
        if(strcmp(input_arr[i], ">") == 0)  // redirect output
        {
            // Open target file
            int targetFD = open(input_arr[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (targetFD == -1) { 
                perror("target open()"); 
                exit(1); 
            }
            printf("targetFD == %d\n", targetFD); // Written to terminal
        
            // Redirect stdout to target file
            int result = dup2(targetFD, 1);
            if (result == -1) { 
                perror("target dup2()"); 
                // exit(2); 
            }
            input_arr[i] == '\0'; 
        }

        // if(strcmp(input_arr[i], "<") == 0)  // Output redirection 

        if(strcmp(input_arr[i], "$$") == 0)  // Variable expansion
        {

        }

        // // background command
        // if((i == (token_count-1)) && (strcmp(input_arr[i], "&") == 0))  


    }
}

char **expansion(char **input_arr, int token_count, pid_t smallsh_pid)
{
    int i, j, z; 

    char str_pid[6]; 
    sprintf(str_pid, "%d", smallsh_pid); 

    printf("pid = %s", str_pid);
    fflush(stdout);

    int pid_len = strlen(str_pid); 

  

    for(i=0; i<token_count; i++)
    {
        char new_str[2048]; 
        z=0; 

        for(j=0; j<strlen(input_arr[i]); j++)
        {     
            // if we find 2 $ in a row which is our case for replacing
            if((input_arr[i][j] == '$') && (input_arr[i][j+1] == '$'))
            {
                strcpy(&new_str[z], str_pid);
                j++; 
                z += pid_len; 
            }
            else
                new_str[z++] = input_arr[i][j]; 
        }
        new_str[z] = '\0'; 

        strcpy(input_arr[i], new_str); 

        int k = 0;
        for(k; k<2048; k++)
            new_str[k] = '\0'; 
        
    }

    return input_arr; 
}

int redirect_out(char **input_arr, int i)
{
	// Open target file
	int targetFD = open(input_arr[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (targetFD == -1) { 
		perror("target open()"); 
		exit(1); 
	}
	printf("targetFD == %d\n", targetFD); // Written to terminal
  
	// Redirect stdout to target file
	int result = dup2(targetFD, 1);
	if (result == -1) { 
		perror("target dup2()"); 
		exit(2); 
	}

    return 0; 
}


int handle_else(char **input_arr, int token_count, pid_t *pid_arr)
{
    int child_status; 

    input_arr[token_count] = NULL;

    printf("\n"); 
    fflush(stdout); 

    int j=0; 
    for(j; j<token_count; j++)
    {
        printf("%s ", input_arr[j]); 
        fflush(stdout); 
    }

    printf("\n"); 
    fflush(stdout); 




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