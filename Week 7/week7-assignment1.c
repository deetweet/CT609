/******************************************************************************

Title: week7-assignment1.c
Author: David Foye
Course: CT609
Assignment: Assignment 1 Week 6
Purpose: Write a program that reads in a file containing several lines of text 
and then prints only those strings that end with the letters “ed”.
The program should do the following:
1.   Prompt the user for the name of a text file
1.i. If the file does not exist, display an error message and re-prompt
2.   Read in a series of strings from the text file and store these in an 
     array of strings, dynamically allocating memory for each element
3.   Loop through the populated string array and find elements that end with 
     the characters "ed"
4.   Display on screen a list of only those strings that end with "ed"

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for strstr()

#define MAX 1024            //Max number of words
#define FILENAME_LEN 1000   //Max length of a filename

int  readFile   ( char*, char** );  //reads data from the file into the array

int main() {
    char inFilename[FILENAME_LEN];  //array of chars to store the intput filename (max len is 1000)
    char *contents[MAX];            //array to store the contents of the file
    char *word;                     //temp variable to run string functions against
//  check if the file was read successfully. loop and prompt the user again until a valid file is provided
    do {
        printf("Enter Input Filename (ctrl + c to exit)> ");
        scanf("%s", inFilename);
    } while ( !readFile(inFilename, contents) ); //the call to readFile will return 0 if unsuccessful
    printf("Found the below words in the file ending in ed\n");
    for ( int i = 0; i < sizeof(contents); i++) {
        word = strrchr(contents[i], 'e'); //find the last instance of 'e' in the string
//      strrchr will return the string after the last instance of 'e' 
//      the test will only pass if the next character is 'd' and the returned string
//      is two characters in length. For completeness, I've also included the null character
//      appended at the end of each string
        if( strcmp(word, "ed")  && strlen(word) <= 2 ) {
            printf("%s\n", contents[i]);
        }
    }
    free(word);
    return 0;
}

//this is Dale's implementation of the readFile() function which I reused for this assignment
//from: https://nuigalway.blackboard.com/courses/1/1819-CT609/db/_256668_1/DaleCollins_Wk6_1819-CT609%20Sample%20Answer.docx
int readFile(char *fileName, char* DataArray[]){ // start a readFile function
    char buff[256]; // define a variable that will be used to hold data
    int p = 0; // define an int
    long len; // define a long variable
    FILE *FS; // allocate a pointer
    FS = fopen(fileName,"r"); // set that pointer to point to the file the user entered before in the main function
    if (FS == NULL) {
        printf("Cannot open file for reading or file not found. Try again.\n\n" );
        return 0;
    }
    while (!feof(FS)){ // check if we're at the end of the file
        fscanf(FS,"%s",buff); // read file and hold that info int the buff variable
        len = strlen(buff); // check the length of the buff variable and store that as a variable called len
        DataArray[p] = (char*)malloc(len+1); // get the size of the hold
        strcpy(DataArray[p],buff); // hold the data in the buff
        ++p; // increment the p variable
    }
    fclose(FS); // close the file
    return p; // return the value of p, either 0 or 1.
}