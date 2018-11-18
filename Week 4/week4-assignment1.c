/******************************************************************************

Title: week4-assignment1.c
Author: David Foye
Course: CT609
Assignment: Assignment 1 Week 4
Purpose: Define a function called hypotenuse that calculates the length of the 
hypotenuse of a right triangle when the other two sides are given. The function 
should take two arguments of type double and return the hypotenuse as a double. 

*******************************************************************************/
#include <stdio.h>
#include <math.h>   //include math.h header file for sqrt and pow functions

double hypotenuse(double, double);  //function protoype

int main()
{
    double s1, s2;  //declare two local variables to store inputted sides
    printf("Enter the measurement of two sides separated by a comma: ");    //prompt the user
    scanf("%lf,%lf", &s1, &s2); //use %lf to denote a double and separate the entry by a comma
    //the below line prints out the inputted sides and the result of the function in one line
    //separate the entries by two tabs and format the output of the sides to two decimal places
    //format the result of the function to four decimal places
    printf("\nSide 1: %.2lf \t\t Side 2: %.2lf \t\t Hypotenuse: %.4lf", s1, s2, hypotenuse(s1, s2));
    return 0;   //return main function value to denote successful conclusion
}   //exit funtion main

/*
this function returns a double and takes two parameters of type double.
I chose to name these differently to the variables in the main function
in order to clearly show the scope of each variable definition and to make
the function more readable.
*/
double hypotenuse(double side1, double side2) {
    //the hypotenuse is defined as the square root of the sum of the squares
    //of each side. For this we utilise the Square root (sqrt) and Power of
    //(pow) functions from math.h
    return sqrt(pow(side1, 2) + pow(side2, 2)); //return the value to the main function
}