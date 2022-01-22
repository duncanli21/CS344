/******************************************************************************
*   The purpose of this program is to read in a file with movie data, sort it, 
*   organize it, and return data to the user. 
*
*   Author: Liam Duncan
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void largest_file()
{
    

    return; 
}

void smallest_file()
{
    return; 
}

void specific_file()
{
    return; 
}




void choice_1()
{    
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
        largest_file();
    
    if(main_choice == 2)
        smallest_file();
    
    if(main_choice == 3)
        specific_file(); 

    return; 
}

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