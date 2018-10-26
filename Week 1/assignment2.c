/******************************************************************************
    Title: week1-assignment2.c
    
    Author: David Foye
    
    Course: CT609
    
    Assignment: Assignment 2 Week 1
    
    Purpose: Write a program that reads in the radius of a circle and prints the 
    circle's diameter, circumference and area. Use the constant value 3.14159 
    for π. Perform each of these calculations inside the printf statement(s) 
    and use the conversion specifier %f.
    
    Notes:
        - Tested with input from https://www.mathsisfun.com/geometry/circle-area.html
    
*******************************************************************************/

#include <stdio.h>

int main()
{
    float fRadius = 0;                                      // Initialise the value of the radius.
    const float PI = 3.14159;                               // Set our constant PI to the required value and print our information.
    printf("This program reads in the radius of a circle and outputs the circle's\n");
    printf("diameter, circumference and area.\nEnter a value for the radius of the circle: ");
    scanf("%f", &fRadius);                                  // Read in the input value of the radius.
    if (fRadius < 0) {                                      // Handle for the case where the user inputs
        printf("Radius cannot be less than 0. Exiting.");   // a value less than zero.
        return 1;                                           // Return a false exit code back to the calling environment.
    }
    printf("Diameter:\t%f\n", fRadius * 2);                 // Print the answer: Diameter := radius x 2.
    printf("Circumference:\t%f\n", (2 * PI) * fRadius);     // Print the answer: Circumference := 2*PI * radius.
    printf("Area:\t\t%f\n", PI * pow(fRadius, 2));          // Print the answer: Area := PI x (r^2). 
                                                            // Order of precedence means that the exponent operation 
                                                            // will be carried out before the multiplication operator.
    return 0;                                               // Pass an 'execution successful' back to the calling environment
}