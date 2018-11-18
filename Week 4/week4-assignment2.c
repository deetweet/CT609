/******************************************************************************

Title: week4-assignment2.c
Author: David Foye
Course: CT609
Assignment: Assignment 2 Week 4
Purpose:
1. Locate a recursive instance, code it and test it 
2. Use the factorial function to calculate the series: 
    e = 1/0! + 1/1! + 1/2! + 1/3! + … for 50 terms 
3. Write the same series calculation using an iterative loop of your choice.
4. Calculate the performance of the recursive and iterative loop methods.

*******************************************************************************/
#include <stdio.h>
#include <time.h>   //include time.h for timing functions

double fibonacci_recur( double );     //function protoype for the recursive method
double fibonacci_iter ( double );      //function protoype for the iterative method

int main() {
    int i;              //loop variable
    clock_t begin, end; //variables to get the CPU time in order to measure
    double recursive_time, iterative_time;  //store the results
    const int NUM_LOOPS = 45;                //for loop sentinel value    
    begin = clock();    //start the clock
    printf("Running the Fibonnaci recursive version for %i loops\n", NUM_LOOPS);
    for (i = 0; i < NUM_LOOPS; i++) {
            printf("%lf\n", fibonacci_recur(i));    
    }
    end = clock();
    recursive_time = (double)(end - begin);  //from: https://stackoverflow.com/questions/5248915/execution-time-of-c-program

    begin = clock();    //sart the clock
    printf("Running the Fibonnaci iterative version for %i loops\n", NUM_LOOPS);
    for (i = 0; i < NUM_LOOPS; i++) {
        printf("%lf\n", fibonacci_iter(i));
    }
    end = clock();      //end the clock
    iterative_time = (double)(end - begin); 
    printf("\n\nCPU Time for Recursive version: %lf\nCPU Time for Iterative version: %lf", recursive_time, iterative_time);
    return 0;   //return main function value to denote successful conclusion
}   //exit function main


/* Recursive version of the fibonacci function */
/**************************************************************************
 * (C) Copyright 1992-2010 by Deitel & Associates, Inc. and               *
 * Pearson Education, Inc. All Rights Reserved.                           *
 *                                                                        *
 * DISCLAIMER: The authors and publisher of this book have used their     *
 * best efforts in preparing the book. These efforts include the          *
 * development, research, and testing of the theories and programs        *
 * to determine their effectiveness. The authors and publisher make       *
 * no warranty of any kind, expressed or implied, with regard to these    *
 * programs or to the documentation contained in these books. The authors *
 * and publisher shall not be liable in any event for incidental or       *
 * consequential damages in connection with, or arising out of, the       *
 * furnishing, performance, or use of these programs.                     *
 *************************************************************************/
double fibonacci_recur(double n) {
   /* base case */
   if (n == 0 || n == 1) {
      return n;
   } /* end if */
   else { /* recursive step */ 
      return (1/fibonacci_recur( n - 1 ) + 1/fibonacci_recur( n - 2 ));
   } /* end else */
} /* end function fibonacci_recur */

/* 
Iterative version of the Fibonacci function 
From: https://gist.github.com/caninemwenja/4014485
*/

double fibonacci_iter(double n) {
    int i=0;
    double result=1, prevresult=1;  //initialise variables
	
	while (i < (n - 1)) {
		double temp = 1/result; 
		result = 1/result + 1/prevresult;
		prevresult = temp;
		i++;
	}   //end while
	
	return result;
}   //end function fibonacci_iter