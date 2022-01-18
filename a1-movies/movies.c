// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct language
// {
//     char *name; 
//     struct language *next; 
// }

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

void createLanguages(char *lang_str, struct movie* current_movie)
{
    char delimiters[] = "[];";

    char *saveptr2; 

    int i=0, j=0; 

    char *token; 

    // printf("\nin\n");

    // current_movie->languages = (char**)malloc(5 * sizeof(char*));

    token = strtok_r(lang_str, delimiters, &saveptr2); 
    // printf("%s n" , token);
    // current_movie->languages[i] = calloc(strlen(token)+1, sizeof(char)); 
    strcpy(current_movie->languages[i], token);  

    // printf("language Token is : %s\n", current_movie->languages[i]);

    current_movie->num_langs = 1; 

    // printf("\nin\n");

    while(token != NULL)
    {
        i++; 
        token = strtok_r(NULL, delimiters, &saveptr2); 
        // printf("%s " , token);
        if(token == NULL)
        {
            // printf("NULL Token"); 
            continue; 
        }
        // current_movie->languages[i] = calloc(strlen(token)+1, sizeof(char)); 
        strcpy(current_movie->languages[i], token);
        current_movie->num_langs++; 
        // printf("");
    }

    // for(j=0; j<i; j++)
    // {
    //     // printf("%s, ", current_movie->languages[i]); 
    // }

    return; 
}

/* Parse the current line which is space delimited and create a
*  movie struct with the data in this line
*/
struct movie *createMovie(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char *saveptr;

    // currMovie->num_langs = NULL;

    // The first token is the title
    char *token = strtok_r(currLine, ",", &saveptr);
    if(token == NULL)
        return currMovie; 
    // printf("title token : %s\n", token);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // The next token is the year
    token = strtok_r(NULL, ",", &saveptr);
    // printf("year token : %s\n", token);
    // int year = atoi(token); 
    currMovie->year = calloc(strlen(token)+1, sizeof(char));
    strcpy(currMovie->year, token);

    // The next token is the languages
    token = strtok_r(NULL, ",", &saveptr);
    // printf("language token : %s\n", token);
    createLanguages(token, currMovie); 
    // currMovie->languages = calloc(strlen(lang_arr), sizeof(char));
    // strcpy(currMovie->languages, lang_arr);

    // The last token is the rating
    token = strtok_r(NULL, "\n", &saveptr);
    // printf("rating token : %s\n", token);
    currMovie->rating = calloc(strlen(token)+1, sizeof(char));
    strcpy(currMovie->rating, token);

    // printf("Rating Copied\n");

    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;

    // printf("about to return");
    

    return currMovie;
}



/*
* Return a linked list of movies by parsing data from
* each line of the specified file.
*/
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
void printMovie(struct movie* aMovie){
  printf("\n%s, ", aMovie->title);
  int i;

  for(i=0; i<aMovie->num_langs; i++)
  {
    //   if(aMovie->languages[i] == NULL)
    //     break; 
      printf("%s, ", aMovie->languages[i]); 
  }

  printf("%s, %s\n" ,aMovie->year, aMovie->rating);
}
/*
* Print the linked list of movies
*/
void printmovieList(struct movie *list)
{
    while (list != NULL)
    {
        // printf("going to print movie\n");
        printMovie(list);
        // printf("movie Printed\n");
        list = list->next;
        // printf("list->next\n"); 
    }
}

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


void choice_1(struct movie *list)
{
    int desired_year=0; 

    while((desired_year <= 0) || ((desired_year > 2023)))
    {
        printf("\nEnter the Year for which you want to see movies: ");
        scanf("%d", &desired_year); 
        
    }

    int movie_count = 0; 
    while(list != NULL)
    {
        if(list->year == NULL)
        {
            printf("year value is NULL");
            exit(0);
        }
        int int_year = atoi(list->year); 
        // printf("%d\n", int_year);
        if(int_year == desired_year)
        {
            printf("%s\n", list->title); 
            movie_count++;
        }
        
        list = list->next; 
    }

    if(movie_count == 0)
        printf("There is no data for movies released in the year %d\n", desired_year); 

    return; 
}

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

// void choice_3()

void first_choice(struct movie *list)
{
    while(1)
    {
    printf("\n1. Show movies released in the specified year\n");
    printf("2. Show the highest rated movie for each year\n");
    printf(
        "3. Show the title and year of the release of all movies in a specific language\n"
        ); 
    printf("4. Exit from the program\n"); 

    // printf("\nEnter a choice from 1 to 4: "); 

    int main_choice = 0; 

    while((main_choice < 1) || (main_choice > 4))
    {
        printf("\nEnter a choice from 1 to 4: ");
        scanf("%d", &main_choice); 
    }
    if(main_choice == 4)
        return; 

    if(main_choice == 1)
        choice_1(list);
    
    if(main_choice == 2)
        choice_2(list);
    
    }
}

void free_mem(struct movie *list)
{

    struct movie* temp; 
    while(list != NULL)
    {   
        temp = list;
        list = list->next; 

        free(temp->title);

        // printf("\ntitle Freed\n");

        int i=0; 

        free(temp->year);

        // printf("Year Freed\n"); 
        free(temp->rating);

        // printf("rating Freed\n");

        free(temp);

        // printf("List Freed\n");

    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movie_info1.txt\n");
        return EXIT_FAILURE;
    }
    struct movie *list = processFile(argv[1]);
    printmovieList(list);

    int num_movies = count_list_length(list); 

    printf("\nProcessed file %s and parsed data for %d movies.\n", argv[1], num_movies); 

    first_choice(list);

    free_mem(list); 
    return EXIT_SUCCESS;
} 