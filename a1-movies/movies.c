// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 

/* struct for student information */
struct movie
{
    char *title;
    char *year;
    char **languages;
    char *rating;
    struct movie *next;
};

/* Parse the current line which is space delimited and create a
*  student struct with the data in this line
*/
struct movie *createMovies(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char *saveptr, *saveptr2;


    // The first token is the title
    char *token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // The next token is the year
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->year = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->year, token);

    // The next token is the languages
    token = strtok_r(NULL, ",", &saveptr);

    /*
    * Citation for the following 2 lines: 
    * Date 1/16/22
    * Adapted from: 
    * https://reactgo.com/c-remove-first-last-character-string/
    */
    char *without_first_last = token+1;     // remove the first [
    without_first_last[strlen(without_first_last)-1] = "\0"; // Remove the last ] 

    char *language_array[6]; 

    // allocate memory for an array of languages
    // currMovie->languages = (char**)malloc(6 * sizeof(char*)); 

    // // Make sure that malloc did not fail
    // if(currMovie->languages == NULL)
    // {
    //     printf("Memory not allocated./n");
    //     exit(0);
    // }

    // int i=0;
    // // allocate memory for each string within the array
    // for(i=0; i<6; i++)
    //     currMovie->languages[i]  = (char*)malloc(21); 
    

    language_array[0] = strtok_r(without_first_last, ";", &saveptr2);
    printf("%s ", language_array[0]);
    int idx; 
    for(idx = 1; idx<5; idx++)  // loop for max num languages 
    {
        char* lang_tok = strtok_r(NULL, ";", &saveptr2);  // Get the second language

        printf("%s ", lang_tok); 
        
        if (lang_tok == NULL)   // leave if there are no more languages
            break;  

        strcpy(language_array[idx], lang_tok);    // copy language into array
    }
    printf("\n");

    

    
    // currMovie->languages = calloc(strlen(token) + 1, sizeof(char));
    // strcpy(currMovie->languages, token);



    // The last token is the rating
    token = strtok_r(NULL, "\n", &saveptr);
    currMovie->rating = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->rating, token);

    // Set the next node to NULL in the newly created student entry
    currMovie->next = NULL;

    return currMovie;
}

/*
* Return a linked list of students by parsing data from
* each line of the specified file.
*/
struct movie *processFile(char *filePath)
{
    // Open the specified file for reading only
    FILE *studentFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;
    size_t nread;
    char *token;

    // The head of the linked list
    struct movie *head = NULL;
    // The tail of the linked list
    struct movie *tail = NULL;

    // Read the file line by line
    while ((nread = getline(&currLine, &len, studentFile)) != -1)
    {
        // Get a new student node corresponding to the current line
        struct movie *newNode = createMovies(currLine);

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
    free(currLine);
    fclose(studentFile);
    return head;
}

/*
* Print data for the given student
*/
void printMovie(struct movie* aMovie){
  printf("%s, ", aMovie->title);
  int i;


  for(i=0; i<6; i++)
  {
      if(aMovie->languages[i] == NULL)
        break; 
      printf("%s, ", aMovie->languages[i]); 
  }

  printf("%s, %s" ,aMovie->year, aMovie->rating);
}
/*
* Print the linked list of students
*/
void printMovieList(struct movie *list)
{
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
    // printf()
}

/*
*   Process the file provided as an argument to the program to
*   create a linked list of student structs and print out the list.
*   Compile the program as follows:
*       gcc --std=gnu99 -o students main.c
*/

/*
* Citation for the following function: 
* Date 1/16/22
* Adapted from: 
* https://www.geeksforgeeks.org/find-length-of-a-linked-list-iterative-and-recursive/

This function will iterativly count the length of al linked list and return the 
length as an int. 
*/
int count_list_length(struct movie *list)
{
    int count = 0; 
    struct movie* current_node = list;

    while(current_node != NULL)
    {
        count++; 
        current_node = current_node->next; 
    }

    return count; 
}

void ui_loop(struct movie *list)
{
    while(1)
    {

    }
}

free_all(struct movie *list)
{
    struct movie* current_node = list; 

    while(current_node != NULL)
    {
        int i;
        for(i=0; i<6; i++)   
            free(current_node->languages[i]); 
        
        free(current_node->languages); 

        current_node = current_node->next; 
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies_sample_1.csv\n");
        return EXIT_FAILURE;
    }
    struct movie *list = processFile(argv[1]);
    printMovieList(list);


    free_all(list); 
    return EXIT_SUCCESS;
}