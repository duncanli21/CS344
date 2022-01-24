/******************************************************************************
*   The purpose of this program is to navigate directories containing files 
*   and perform a set of actions on the correct movie files.  
*
*   Author: Liam Duncan
******************************************************************************/

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <time.h>
#include <pwd.h>

#define PREFIX "movies_"

/* struct for movie information */
struct movie
{
    char *title;
    char *year;
    char languages[5][21];
    int num_langs; 
    char *rating;
    struct movie *next;
};

// declare functions
void main_menu();
void choice_1();
void specific_file(DIR* currDir); 
void smallest_file(DIR* currDir); 
void largest_file(DIR* currDir) ; 
void process_file(DIR* currDir, char *file_name, struct stat stats); 
void free_mem(struct movie *list); 
struct movie *processFile(char *filePath); 
struct movie *createMovie(char *currLine); 
void createLanguages(char *lang_str, struct movie* current_movie); 


/*******************************************************************************
 * Function Purpose: To parse the languages for each movie from the source file. 
 * 
 * Parameters: 
 *              lang_str: the token from the source file containing all 
 *                        languages for the movie. 
 *              
 *              current_movie: Current movie struct. 
 * 
 * Citation: 
 *              This code was copied from my previous assignment 1. 
*******************************************************************************/
void createLanguages(char *lang_str, struct movie* current_movie)
{
    char delimiters[] = "[];";  // make a char array of delimiters for strtok

    char *saveptr2;     // save pointer for strtok

    int i=0, j=0;   // initialize some ints 

    char *token;    // make a char string for the token

    // extract the token from 
    token = strtok_r(lang_str, delimiters, &saveptr2); 
    strcpy(current_movie->languages[i], token);  // copy token over to struct

    current_movie->num_langs = 1; // initialize number of languages to 1

    // loop through the rest of the tokens
    while(token != NULL)
    {
        i++; 
        token = strtok_r(NULL, delimiters, &saveptr2); // get next token

        // check for error 
        if(token == NULL)
        {
            continue; 
        }

        // add to struct
        strcpy(current_movie->languages[i], token);
        current_movie->num_langs++; 
    }

    return; 
}

/*******************************************************************************
 * Purpose: To parse the current line of the data file and add items to the 
 *          struct. 
 * 
 * Parameters: 
 *              currLine: this is the current line of the data file. 
 * 
 * Citation: 
 *          Date: 1/17/22
 *          Adapted from: Canvas sample file
 *          Link: https://repl.it/@cs344/studentsc#main.c
*******************************************************************************/
struct movie *createMovie(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char *saveptr;

    // The first token is the title
    char *token = strtok_r(currLine, ",", &saveptr);
    if(token == NULL)
        return currMovie; 
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // The next token is the year
    token = strtok_r(NULL, ",", &saveptr);

    currMovie->year = calloc(strlen(token)+1, sizeof(char));
    strcpy(currMovie->year, token);

    // The next token is the languages
    token = strtok_r(NULL, ",", &saveptr);
    createLanguages(token, currMovie); 

    // The last token is the rating
    token = strtok_r(NULL, "\n", &saveptr);
    currMovie->rating = calloc(strlen(token)+1, sizeof(char));
    strcpy(currMovie->rating, token);

    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;    

    return currMovie;
}



/*
* Return a linked list of movies by parsing data from
* each line of the specified file.
*/
/*******************************************************************************
 *  Purpose: To return a linked list of movies by parsing data from each line of 
 *           the specified file. 
 * 
 * Parameters:
 *              filePath: Path of the file to be processed
 * 
 * Citation: 
 *           Date: 1/17/22
 *           Adapted from: Canvas sample file
 *           Link: https://repl.it/@cs344/studentsc#main.c 
*******************************************************************************/
struct movie *processFile(char *filePath)
{
    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;

    // The head of the linked list
    struct movie *head = NULL;
    // The tail of the linked list
    struct movie *tail = NULL;

    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        // Get a new movie node corresponding to the current line
        struct movie *newNode = createMovie(currLine);

        // Is this the first node in the linked list?
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
    }

    // Need to remove the first node because that is the header 
    struct movie *temp = head; 
    
    // free all memory from header line
    head = head->next;  
    free(temp->title);
    free(temp->year);
    free(temp->rating);
    free(temp);

    free(currLine);
    fclose(movieFile);
    return head;
}


/*******************************************************************************
 * Purpose: to free all allocated memory across the program. 
 * 
 * Parameters: 
 *              list: the head of a linked list. 
*******************************************************************************/
void free_mem(struct movie *list)
{
    // make temp struct for freeing
    struct movie* temp; 

    // loop through all nodes
    while(list != NULL)
    {   
        // set temp as current node and advance iterator
        temp = list;
        list = list->next; 

        // free items 
        free(temp->title);
        free(temp->year);
        free(temp->rating);

        // free struct
        free(temp);
    }
}

/*******************************************************************************
 * Purpose: To Process a selected file 
 * 
 * Parameters: 
 *              currDir: the current directory object
 *              file_name: the name of the file as a string 
 *              stats: stat struct for the file object
*******************************************************************************/
void process_file(DIR* currDir, char *file_name, struct stat stats)
{
    // make some variables for user id 
    uid_t user_id; 
    char id_string[256]; 

    printf("\nNow processing the chosen file names %s\n", file_name); 

    // process the chosen file into a linked list
    struct movie *list = processFile(file_name);

    // generate a random number
    srand(time(NULL));
    int random_num = rand() % 100000; 

    // turn the int into a string
    /*
    *  Citation for the following 3 lines: 
    *
    *           Date: 1/23/22 
    *           Adapted from: Stack Overflow
    *           Link: https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c
    */ 
    char random_num_string[256];
    memset(random_num_string, '\0', strlen(random_num_string));
    sprintf(random_num_string, "%d", random_num);


    /*
    *  Citation for the following 4 lines: 
    *
    *           Date: 1/23/22 
    *           Adapted from: The Open Base Specifications Issue 6
    *           Link: https://pubs.opengroup.org/onlinepubs/009696799/functions/getpwuid.html
    */ 
    user_id = stats.st_uid;     // get the user id 
    struct passwd *pwd;     // make passwd struct 
    pwd = getpwuid(user_id);    // get the id from the user id number 
    memset(id_string, '\0', strlen(id_string));     // fill buffer with NULL
    strcpy(id_string, pwd->pw_name);    // copy it into our buffer

    
    // make a char array for the path name
    char path_name[256];
    memset(path_name, '\0', strlen(path_name));

    // Make the string for the middle of the directory name
    char middle[] = ".movies.";      

    // Create the string for the name of the directory
    strcat(id_string, middle); 
    strcat(id_string, random_num_string);

    // printf("\n%s\n", id_string);

    mkdir(id_string, 0750); // create the actual directory. permission -rwxr-x---

    printf("Created a directory with the name %s\n", id_string);


    // Loop through the linked list
    while(list != NULL)
    {
        char temp_path[256];    // Make a variable for the file path 
        memset(temp_path, '\0', strlen(temp_path)); // populate buffer with NULL
        strcpy(temp_path, id_string);   // copy directory name into buffer

        strcat(temp_path, "/");     // add the "/"
        strcat(temp_path, list->year);  // add the year  
        strcat(temp_path, ".txt");  // add the file extension

        // printf("%s\n", temp_path);  
        // printf("%s\n", id_string);
        

        FILE *fp; // make a file pointer for the file


        // Citation: 
        // Got info from canvas page. 
        fp = fopen(temp_path, "a+");    // open file in append+ mode 
        fprintf(fp, "%s\n", list->title);   // write the movie title into the file 
        chmod(temp_path, 0640); // change the permissions for the file to -rw-r-----
        fclose(fp);     // close the file

        list = list->next;  // move on to the next item in the list
    }

    free_mem(list);   // free all the memory allocated
    return; 
}


/*******************************************************************************
 * Function Purpose: To find the largest file in the directory and process it  
 * 
 * Parameters: 
 *              currDir: The current directory object
 *              
 * Citation: 
 *           Date: 1/24/22
 *           Adapted from: Canvas sample file
 *           Link: https://replit.com/@cs344/35statexamplec
*******************************************************************************/
void largest_file(DIR* currDir)   
{
    // Create variables
    struct dirent *aDir; 
    struct stat tempStat; 
    struct stat permStat; 
    off_t file_size; 
    int i = 0; 
    char file_name[256];

    // Begin loop through current directory 
    while((aDir = readdir(currDir)) != NULL)
    {
        // if statment will check if the movies_ prefix matches the file
        if(strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0)
        {
            // Get the data for the file and load it into struct
            stat(aDir->d_name, &tempStat); 
            
            if((i == 0) || (tempStat.st_size > file_size))
            {
                file_size = tempStat.st_size;    // update the file size
                memset(file_name, '\0', sizeof(file_name)); // fill buffer with null chars  
                strcpy(file_name, aDir->d_name);    // copy name into buffer
                permStat = tempStat; 
            }
            i++;    // advance iterator so you know you arent on the first entry
        }
    }

    // Call function to process file
    process_file(currDir, file_name, permStat); 
    return; 
}


/*******************************************************************************
 * Function Purpose: To find the smallest file in the directory and process it  
 * 
 * Parameters: 
 *              currDir: The current directory object
 *              
 * Citation: 
 *           Date: 1/24/22
 *           Adapted from: Canvas sample file
 *           Link: https://replit.com/@cs344/35statexamplec
*******************************************************************************/
void smallest_file(DIR* currDir)
{

    // Create variables
    struct dirent *aDir; 
    struct stat tempStat; 
    struct stat permStat; 
    off_t file_size; 
    int i = 0; 
    char file_name[256];

    // Begin loop through current directory 
    while((aDir = readdir(currDir)) != NULL)
    {
        // if statment will check if the movies_ prefix matches the file
        if(strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0)
        {
            // Get the data for the file and load it into struct
            stat(aDir->d_name, &tempStat); 
            
            if((i == 0) || (tempStat.st_size < file_size))
            {
                file_size = tempStat.st_size;    // update the file size
                memset(file_name, '\0', sizeof(file_name)); // fill buffer with null chars  
                strcpy(file_name, aDir->d_name);    // copy name into buffer
                permStat = tempStat; 
            }
            i++;    // advance iterator so you know you arent on the first entry
        }
    }

    // call function to process file
    process_file(currDir, file_name, permStat); 
    return; 
}



/*******************************************************************************
 * Function Purpose: To find a specific file in the directory and process it  
 * 
 * Parameters: 
 *              currDir: The current directory object
 *              
 * Citation: 
 *           Date: 1/24/22
 *           Adapted from: Canvas sample file
 *           Link: https://replit.com/@cs344/35statexamplec
*******************************************************************************/
void specific_file(DIR* currDir)
{
    // Create char array for input string
    char input[60]; 

    // Get the user input string
    printf("Enter the complete file name: ");
    scanf("%s", input);

    // Create variables for directory and files 
    struct dirent *aDir; 
    struct stat tempStat; 
    struct stat permStat; 
    off_t file_size; 
    int i = 0; 
    char file_name[256];


    // Begin loop through current directory 
    while((aDir = readdir(currDir)) != NULL)
    {
        // if statment will check if the input matches the file name 
        if(strncmp(input, aDir->d_name, strlen(input)) == 0)
        {
            // Get the data for the file and load it into struct
            stat(aDir->d_name, &tempStat); 
            memset(file_name, '\0', sizeof(file_name)); // fill buffer with null chars  
            strcpy(file_name, aDir->d_name);    // copy name into buffer
            permStat = tempStat; 
            process_file(currDir, input, permStat); 
            i++;    // advance iterator so you know you arent on the first entry
        }
            
    }

    // If the file was not found because the iterator was never advanced
    if(i==0)
    {
        printf("The file %s could not be found. Try again\n", input);
        choice_1(); // Go back to choice 1. Kinda recursive? 
    }
    
    return; 
}


/*******************************************************************************
 * Function Purpose: Secondary menu UI of the program 
 *          
 * Citation: 
 *           Date: 1/24/22
 *           Adapted from: Canvas sample file
 *           Link: https://replit.com/@cs344/35statexamplec
*******************************************************************************/
void choice_1()
{    

    DIR* currDir = opendir(".");

    // print out options
    printf("\nWhich file do you want to process?\n");
    printf("Enter 1 to pick the largest file\n");
    printf("Enter 2 to pick the smallest file\n");
    printf("Enter 3 to specify the name of the file\n"); 

    // initialize choice variable 
    int main_choice = 0; 

    // loop to get user input and validate it
    while((main_choice < 1) || (main_choice > 3))
    {
        printf("\nEnter a choice from 1 to 3: ");
        scanf("%d", &main_choice); 
    }

    // go to correct function
    if(main_choice == 1)
        largest_file(currDir);
    
    if(main_choice == 2)
        smallest_file(currDir);
    
    if(main_choice == 3)
        specific_file(currDir); 

    closedir(currDir); 

    return; 
}



/*******************************************************************************
 * Purpose: Main menu UI for program. 
*******************************************************************************/
void main_menu()
{
    while(1)
    {
        printf("\n1. Select file to process\n");
        printf("2. Exit the program"); 

        // initialize choice variable 
        int main_choice = 0; 

        // loop to get user input and validate it
        while((main_choice < 1) || (main_choice > 2))
        {
            printf("\nEnter a choice 1 or 2: ");
            scanf("%d", &main_choice); 
        }

        // exit option 
        if(main_choice == 2)
            return; 

        // go to correct function
        if(main_choice == 1)
            choice_1();
    }
}

int main(int argc, char *argv[])
{    
    main_menu(); 

    return EXIT_SUCCESS; 
}