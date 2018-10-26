/******************************************************************************
    Title: week1-assignment1.c
    
    Author: David Foye
    
    Course: CT609
    
    Assignment: Assignment 1 Week 1
    
    Purpose: Write a program that asks the user to enter two numbers, obtains 
    them from the user and prints their sum, product, difference, quotient
    and remainder.

*******************************************************************************/

#include <stdio.h>  //Use the #include preprocessor directive to include the Standard IO library.

int main(void) {
    int inputOne, inputTwo; //Declare the variable we will use and print out the user information.
    printf("This application prints the sum, product, difference, quotient and remainder\n"
    "of two numbers entered by the user.\n"
    "Please Enter two numbers\n");
    printf("First number:");  //Prompt for first input from the user with the scanf function and assign the value to the inputOne variable.
    scanf("%d", &inputOne);
    printf("Second number:"); //Prompt for second input from the user with the scanf function and assign the value to the inputTwo variable.
    scanf("%d", &inputTwo);
    
    if (inputOne < 0 || inputTwo < 0 ) {                                 //Here we use the inclusive OR to ensure
        printf("This program only accepts positive integers. Exiting."); //both inputted numbers are positive
        return 1;                                                        //Exit back to the calling environment a false return value
    }
    printf("Sum:\t\t%d\n", inputOne + inputTwo);            //Print the sum of the two numbers.
    printf("Product:\t%d\n", inputOne * inputTwo);          //Print the product of the two numbers.
    if (inputOne >= inputTwo) {                             //In inputOne is greater than or equal to inputTwo
        printf("Difference:\t%d\n", inputOne - inputTwo);   //it goes first. This is so we get the difference
        printf("Quotient:\t%d\n", inputOne / inputTwo);     //quotient and remainder of the larger number from
        printf("Remainder:\t%d\n", inputOne % inputTwo);    //the smaller.
    } else if(inputTwo > inputOne) {                        //We do the same as above but test the case that
        printf("Difference:\t%d\n", inputTwo - inputOne);   //inputTwo is greater than inputOne.
        printf("Quotient:\t%d\n", inputTwo / inputOne);     //Adjust the order so we don't end up in negative
        printf("Remainder:\t%d\n", inputTwo % inputOne);    //territory.
    }
    return 0;                                               //Return a true value back to the calling environment to
}                                                           //note successful program termination.