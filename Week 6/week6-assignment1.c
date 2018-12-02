/******************************************************************************

Title: week6-assignment1.c
Author: David Foye
Course: CT609
Assignment: Assignment 1 Week 6
Purpose: Write a program, called sort_file, which takes a text file as input 
and produces an output file which has all the original lines in alphabetical 
order. The program should do the following:
1. Prompt the user for the names of the input and output files. 
2. You can prepare an input file with appropriate unsorted data.
3. Declare and define three functions to perform the following tasks:
    i.  Read the data from the input file into an array.
    ii. Sort the array using the bubble sort algorithm.
    iii.Write the sorted data array to the output file.

*******************************************************************************/

#include <stdio.h>
#include <string.h> //for the string comparison functions

#define MAX 50         //Maximum number of words

int  readData   ( char* );  //function to read the data into the array
void bubbleSort ( int );    //function to sort the data
int  writeData   ( char[] ); //function to write the data to a file

char contents[MAX];         //array to store the contens of the file

int main() {
    char inFilename[1000], outFilename[1000]; //declare an array of chars to store the intput and output filenames
                                              //the max length of the filnames is 1000 characters
    printf("Enter Input Filename > ");
    scanf("%s", inFilename);
    //check if the file was read successfully, exit if not
    if ( readData(inFilename) ) {     
        printf("File imported successfully\n");
        bubbleSort(sizeof(contents));
        printf("Enter Output Filename > ");
        scanf("%s", outFilename);
        writeData(outFilename);
    } else {
        printf("Imported cancelled\n");
        return 1;
    }
    return 0;
}

//reads the data contained in the filename in a global array variable
int readData(char* filename) {
    int i = 0;
    char line;
    FILE *inFilePtr;
    inFilePtr = fopen(filename, "r" );
    if (inFilePtr == NULL) {
        printf( "File could not be opened. " );
        return 0;
    } else {
        line = fgetc(inFilePtr); //from 
        while (line != EOF) {    //while we're not at the end of file
            contents[i] = line; //read each line into the contents array
            i++;                //increment the array subscript
            line = fgetc(inFilePtr);  //get the next line from the file
        } //end while
    }
    fclose(inFilePtr); //destroy the file handle  to reclaim space
    return 1;
}

//writes the data from the contents array to the file
//specified by the filename parameter
int writeData(char* filename) {
    FILE *outFilePtr;
    outFilePtr = fopen(filename, "wb" ); //use the write binary mode to write the entire contents of the array at once
    fwrite(contents, sizeof(char), sizeof(contents), outFilePtr);
    fclose(outFilePtr);
    return 1;
}

//Implementation of the O(N^2) bubble sort algorithm
//we're using this in the hope it will show a large difference in performance
//against the other algorithm
//function that sorts an array with bubble sort algorithm
void bubbleSort(int dimension) {
    int i, j;
    char temp;
    for(i = 0; i < dimension; ++i) {
        for(j = i + 1; j < dimension; ++j) {
            //the strcmp() function compares two strings, if they are out of order
            //then the body of the if block is ran which swaps the values
            if(strcmp(&contents[i], &contents[j]) > 0) {
                strcpy(&temp, &contents[i]);
                strcpy(&contents[i], &contents[j]);
                strcpy(&contents[j], &temp);
            }
        }
    }
}