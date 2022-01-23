/******************************************************************************
*   The purpose of this program is to read in a file with movie data, sort it, 
*   organize it, and return data to the user. 
*
*   Author: Liam Duncan
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/*******************************************************************************
 * Function Purpose: To parse the languages for each movie from the source file. 
 * 
 * Parameters: 
 *              lang_str: the token from the source file containing all 
 *                        languages for the movie. 
 *              
 *              current_movie: Current movie struct. 
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

/*
* Print data for the given movie
*/
/*******************************************************************************
 * Purpose: To print out the data that has been processed for a single movie. 
 * 
 * Parameters: 
 *              aMovie: a movie struct object
 * 
 * Citation: 
 *           Date: 1/17/22
 *           Adapted from: Canvas sample file
 *           Link: https://repl.it/@cs344/studentsc#main.c 
*******************************************************************************/
void printMovie(struct movie* aMovie){
    
    // Print title
    printf("\n%s, ", aMovie->title);

    int i;  // iterator for loop 

    // loop for number of languages
    for(i=0; i<aMovie->num_langs; i++)
        printf("%s, ", aMovie->languages[i]); 

    // print a line of movie data
    printf("%s, %s\n" ,aMovie->year, aMovie->rating);
}

/*******************************************************************************
 * Purpose: to print all movies in a list. 
 * 
 * Parameters: 
 *              list: the head of a linked list. 
 * 
 * Citation: 
 *           Date: 1/17/22
 *           Adapted from: Canvas sample file
 *           Link: https://repl.it/@cs344/studentsc#main.c
*******************************************************************************/
void printmovieList(struct movie *list)
{
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
}

/*******************************************************************************
 * Purpose: this function will count the number of items in a linked list. 
 * 
 * Parameters: 
 *              list: head of a linked list. 
 * 
 * Citation: 
 *          Date: 1/16/22
 *          Adapted from: 
 *          https://www.geeksforgeeks.org/find-length-of-a-linked-list-iterative-and-recursive/
*******************************************************************************/
int count_list_length(struct movie *list)
{
    int count = 0; // start counter at 0 
    struct movie* current_node = list;  // copy the input list

    // loop through whole list
    while(current_node != NULL)
    {
        // add to counter and move on to next node
        count++; 
        current_node = current_node->next; 
    }

    return count; // return the number of nodes in list
}

/*******************************************************************************
 * Purpose: this function is meant to print out all movies for a certian year 
 *          that is input by the user. 
 * 
 * Parameters: 
 *              list: head of a linked list. 
*******************************************************************************/
void choice_1(struct movie *list)
{
    int desired_year=0; // initialize the desired year variable

    // we use this while loop to both get and verify user input
    // loop will continue until user puts in a reasonable number. 
    while((desired_year <= 0) || ((desired_year > 2023)))
    {
        printf("\nEnter the Year for which you want to see movies: ");
        scanf("%d", &desired_year); // get the input here
    }

    // movie count in case there are no movies in the desired year
    int movie_count = 0; 

    // go through all nodes in linked list
    while(list != NULL)
    {
        // this was supposed to handle errors but I dont think this case will 
        // ever happen
        if(list->year == NULL)
        {
            printf("year value is NULL");
            exit(0);
        }

        // Get the year as an int 
        int int_year = atoi(list->year); 

        // Compare node year to desired year
        if(int_year == desired_year)
        {
            printf("%s\n", list->title);    // print out movies that are in the year
            movie_count++;  // iterarte
        }
        
        list = list->next;  // move on 
    }

    // handle exception
    if(movie_count == 0)
        printf("There is no data for movies released in the year %d\n", desired_year); 

    return; 
}

/*******************************************************************************
 * Purpose: the purpose of this function is to show the highest rated movie for 
 *          each year. 
 * 
 * Parameters: 
 *              list: The head of a linked list. 
*******************************************************************************/
void choice_2(struct movie *list)
{
    int num_movies = count_list_length(list); // Get the number of movies

    // make a pointer to an array to store years that have already been scanned
    int *scanned_years;    

    // allocate memory for array
    scanned_years = malloc(num_movies * sizeof(int)); 

    // initialize counter for the array
    int count = 0; 

    // loop through linked list
    while(list != NULL)
    {
        // initialize some ints
        int j, scanned = 0; 

        // get the year for this item in the list
        int year = atoi(list->year); 

        // Loop to make sure that we have not already scanned that year
        for(j=0; j<count; j++)
        {
            if(scanned_years[j] == year)
                scanned = 1; 
        }

        // if year got scanned already then move on
        if(scanned == 1)
        {
            list = list->next;
            continue; 
        }

        // get the rating as a float 
        float rating = atof(list->rating); 

        // get the name of the movie
        char *name = list->title; 

        // make a temp list to sort through again
        struct movie *temp = list; 

        // dont look at the same index again 
        temp = temp->next; 

        // another loop to find other instances of the same year
        while(temp != NULL)
        {
            // if we find the same year... 
            if(year == atoi(temp->year))
            {
                // and it has a better rating... 
                if(atof(temp->rating) > rating)
                {
                    // replace our last rating and name with this one because 
                    // it is better
                    rating = atof(temp->rating); 
                    name = temp->title; 
                }
            }
            temp = temp->next; // move on
        }

        printf("%d %.2f %s\n", year, rating, name); 
        scanned_years[count] = year; // add this year to the list of scanned years
        count++;    // advance the count
    }

    free(scanned_years);    // free array
}

/*******************************************************************************
 * Purpose: To show the title and year of all movies released in a specific 
 *          language. 
 * 
 * Parameters:
 *              list: the head of a linked list. 
*******************************************************************************/
void choice_3(struct movie *list)
{
    char input[21];     // make a char array for the user input

    // get user input
    // 
    // no error handling because we are looking for an exact match so we assume 
    // the user knows what they are typing in. 
    // 
    // Not sure if this is a great assumption normally but that is what I 
    // interpreted the assignment as. 
    printf("Enter the language for which you want to see movies: ");
    scanf("%s", input); 

    int count = 0; // seet counter to 0 

    // loop through whole list
    while(list != NULL)
    {  
        // for loop will go through number of languages for a movie
        int i=0; 
        for(i=0; i < list->num_langs; i++)
        {
            // If we get a match then print it out 
            if(strcmp(list->languages[i], input) == 0)
            {
                printf("%s %s\n", list->year, list->title);
                count++; 
            }
        }
    list= list->next;   // move on 
    }

    // handle edge case
    if(count == 0)
    printf("No data for movies released in %s\n", input);

    return; 
}

/*******************************************************************************
 * Purpose: This function will run in a loop and be the base of the UI. Function 
 *          gives user the main questions and user will return here after they 
 *          have got the info they need. 
 * 
 * Parameter: 
 *              list: head of a linked list. 
*******************************************************************************/
void first_choice(struct movie *list)
{
    // loop forever
    while(1)
    {
        // print out options
        printf("\n1. Show movies released in the specified year\n");
        printf("2. Show the highest rated movie for each year\n");
        printf(
            "3. Show the title and year of the release of all movies in a specific language\n"
            ); 
        printf("4. Exit from the program\n"); 

        // initialize choice variable 
        int main_choice = 0; 

        // loop to get user input and validate it
        while((main_choice < 1) || (main_choice > 4))
        {
            printf("\nEnter a choice from 1 to 4: ");
            scanf("%d", &main_choice); 
        }

        // exit option 
        if(main_choice == 4)
            return; 

        // go to correct function
        if(main_choice == 1)
            choice_1(list);
        
        if(main_choice == 2)
            choice_2(list);
        
        if(main_choice == 3)
            choice_3(list); 
    
    }
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
 * Purpose: this is the main function 
 * 
 * Citation: 
 *           Date: 1/17/22
 *           Adapted from: Canvas sample file
 *           Link: https://repl.it/@cs344/studentsc#main.c
*******************************************************************************/
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movie_info1.txt\n");
        return EXIT_FAILURE;
    }

    // process file
    struct movie *list = processFile(argv[1]);

    // get number of nodes
    int num_movies = count_list_length(list); 

    printf("\nProcessed file %s and parsed data for %d movies.\n", argv[1], num_movies); 

    // start option loop 
    first_choice(list);

    // free all memory
    free_mem(list); 
    return EXIT_SUCCESS;
} 






