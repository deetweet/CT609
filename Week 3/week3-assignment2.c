/******************************************************************************
Title: week3-assignment2.c
Author: David Foye
Course: CT609
Assignment: Assignment 2 Week 3
Purpose: Write a program that first reads and stores five numbers (each between 
1 and 30). Then, for each number read, your program should print a line 
containing that number of adjacent asterisk

*******************************************************************************/
#include <stdio.h>

int main () {
    
    int counter = 1;      //counter will be the sentinel value
    int numberlist[4];    //numberlist is an array to hold 5 values (0-4)
    
    //begin the while loop for 5 repetitions
    while (counter <= 5) {    
        printf("Enter number %i> ", counter); //prompt user
        scanf("%i", &numberlist[counter]);  //read input
        //check input is within the range and prompt the user
        //if the condition is true then it will fall over the else clause and
        //counter will not be updated. That way it will prompt for the entry again
        if (numberlist[counter] < 1 || numberlist[counter] > 30){
            printf("Input must be between 1 and 30. Try again.\n");
        } else {
            //otherwise the value entered is in the range so we increment
            //the sentinel value and loop for the next prompt
            counter++;
        }
    }
    
    //this is the for loop to print the required output. We re-iinitialise the 
    //sentinel variable to re-use here. This for loop will repeat five times
    //for each entry in the numberlist array
    for (counter = 1; counter <= 5; counter++) {
        //print the entry in the array with a field width of 2 right-justified
        printf ("%2i | ", numberlist[counter]);
        //next loop over the value in that array entry and print the value of the
        //entry in asterisks
        for (int i = 0; i < numberlist[counter]; i++) {
            //this will print an asterisk up to the value of numberlist[counter]
            //leave out the new line character as we want them all on one line
            printf("*");
        }
        //print a newline character before the outer loop repeats so we can begin
        //again on the next entry
        printf("\n");
    }
    return 0;   //exit the program
}               //end function main
