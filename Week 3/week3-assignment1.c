/*****************************************************************
Title: week3-assignment1.c
Author: David Foye
Course: CT609
Assignment: Assignment 1 Week 3
Purpose: Write for statements that print the following sequences of values:
1, 2, 3, 4, 5, 6, 7
3, 8, 13, 18, 23
20, 14, 8, 2, -4, -10
19, 27, 35, 43, 51
*****************************************************************/

#include <stdio.h>    //required library

int main() {

    int counter = 0;	//Initialise variables

//  Print the first range of numbers
//  Increment the counter from one to 7 in increments of 1
    for (counter = 1; counter <= 7; counter++) {
//      Formatting, if the counter is the last in sequence don’t
//      print the trailing comma.
        if (counter == 7) {
            printf ("%i\n", counter);
//	  otherwise print the current value with a trailing comma
        } else {
            printf ("%i, ", counter);
        }
    }


//  print the next range, starting from 3 to 23 in increments of 5    
    for (counter = 3; counter <= 23; counter+=5) {
//      Formatting, if the counter is the last in sequence don’t
//      print the trailing comma.
        if (counter == 23) {
            printf ("%i\n", counter);
        } else {
//	  otherwise print the current value with a trailing comma
            printf ("%i, ", counter);
        }    
    }


//  print the range from 20 to -10 in decrements of 6
    for (counter = 20; counter >= -10; counter-=6) {
//      Formatting, if the counter is the last in sequence don’t
//      print the trailing comma.
        if (counter == -10) {
            printf ("%i\n", counter);
        } else {
//	  otherwise print the current value with a trailing comma
            printf ("%i, ", counter);
        }    
    }


//  print the range from 19 to 51 in increments of 8    
    for (counter = 19; counter <= 51; counter += 8) {
//      Formatting, if the counter is the last in sequence don’t
//      print the trailing comma.
        if (counter == 51) {
            printf ("%i\n", counter);
        } else {
//	  otherwise print the current value with a trailing comma
            printf ("%i, ", counter);
        }    
    }  		//end while
    return 0;    //exit the program
} 			//end program main 
