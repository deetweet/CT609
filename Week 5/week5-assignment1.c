/******************************************************************************

Title: week5-assignment1.c
Author: David Foye
Course: CT609
Assignment: Assignment 1 Week 5
Purpose: Select two of the sort algorithms we have discussed in class.

1. Code the selected algorithms
2. Instrument your sort algorithms to calculate efficiency by gathering data on:
3. Time for algorithm to run
4. Number of swaps
5. Number of comparisons
6. Download the Peak Wind speed at 80meter data for your birthday in 2013
    i.   Copy and paste the data into a file.
    ii.  Load the last column (wind speeds) into a one-dimensional array.
    iii. Run both algorithms again with the wind speed data you created.
    iv.  Save the sorted data in a new file to be used in Question 2.
    v.   Display results, discuss which performed better on your data set.

*******************************************************************************/
#include <stdio.h>  //for printf function
#include <time.h>   //for time measurement
#include <stdlib.h> //for randomisation

//this will be the maximum size of the generated array
//useful to change this to see the effect of larger
//array sizes on the performance of the sorting algorithms

#define MAX_SIZE 1000

//function definitions (parts 1-5)
void generateRandomArray( int[], int );
void copyArray          ( int[], int[], int );
void printArray         ( const int[], int);
void bubbleSort         ( double[], int );
void quickSort          ( double[], int, int);
//global variables so that we can return the number of comparisons and swaps
//for each sorting algorithm to the main function
int bcomparisons, bswaps, qcomparisons, qswaps;

//wind speed data from the .csv file
double windSpeed[]  = { 4.5439,4.4565,4.522,4.5439,4.522,4.675,4.5876,4.6531,4.6313,4.8061,4.5876,4.6968,4.7842,4.7187,4.5876,4.522,4.6094,4.7187,4.9372,4.7187,4.6531,4.9809,4.085,
4.0632,4.1506,4.2161,4.2161,4.4565,4.3254,4.1069,4.0195,4.2817,4.1287,4.238,4.1943,3.9976,4.3254,4.238,4.238,3.9321,4.238,4.5657,4.238,4.1506,4.4783,4.522,4.2161,4.2598,4.522,
3.8884,3.8884,3.8447,4.1943,4.1287,3.9758,3.8884,3.8228,4.0632,3.9976,4.0195,4.0632,3.9758,4.2161,4.0632,4.0632,4.3472,4.522,4.3035,4.0195,4.6313,4.3472,4.5002,4.4128,4.3691,
4.5657,4.675,4.4128,4.3254,4.3035,4.238,4.0195,3.9758,3.9758,4.3254,3.9758,4.1724,3.8884,3.8447,3.6917,4.085,3.4514,3.4732,3.2766,3.6043,3.7354,3.5606,3.8665,3.5825,3.364,3.8665,
3.5169,3.4951,3.0581,2.8614,3.1673,3.1018,3.211,2.9488,2.8833,2.8396,2.9051,2.8614,3.3421,2.8833,3.4077,2.7085,3.1892,3.4951,3.3858,3.0362,3.0362,2.6429,2.5555,2.3589,2.3589,
2.4026,2.4463,1.7908,1.9874,2.0748,1.9656,2.0311,2.1404,2.0093,1.9874,2.053,2.1404,1.7908,1.5941,1.7034,1.7689,1.9437,1.3975,1.6378,2.0311,1.8563,1.7471,1.9656,2.0093,2.0093,
1.9656,1.8126,1.7689,1.9219,2.1404,2.1841,2.3152,2.3589,2.2933,2.2059,1.9874,2.0311,2.0093,2.0093,1.9437,1.9219,1.9,1.8782,1.9437,2.1404,2.1185,2.1404,2.1841,2.1185,1.9656,2.0093,
1.6815,1.5504,1.2008,0.98235,0.76385,0.9168,0.9168,0.89495,1.5723,1.4849,1.5067,1.7689,1.7689,1.7471,1.6378,1.616,1.2227,1.3756,1.2008,2.0967,1.7252,1.7034,1.8563,1.9,1.026,1.9,
1.616,2.2059,2.5337,2.5555,1.7471,2.4463,2.4244,2.5555,3.7354,3.6043,3.7573,3.7354,4.0632,3.9321,4.3254,4.5876,5.2212,4.7405,4.7842,4.522,3.6262,4.6968,4.675,4.085,3.9539,3.364,
3.5606,3.9102,3.5606,3.5606,3.4951,3.8447,3.6917,3.5606,3.801,3.8665,3.9102,3.7791,3.6917,3.8884,3.4514,3.7354,3.7354,3.364,3.4732,3.648,3.8884,4.0413,3.1673,3.5169,3.3858,2.774,
2.7522,3.3421,3.5606,3.2547,3.7791,3.6917,3.9758,4.2161,4.3472,3.7354,4.1287,4.2161,3.648,3.648,3.8884,4.085,4.2817,4.8716,4.6094,4.1943,4.085,4.3472,4.4783,4.522,4.7842,4.4346,
4.5002,4.7405,4.5876,4.6968,4.2598,4.522,4.6531,4.5876,3.7573,4.1069,4.4346,4.4565,4.3472,4.6968,4.8498,5.4397,4.8061,5.112,5.1775,5.3086,5.4616,5.0683,5.4834,4.6313,4.1724,4.085,
4.1287,4.0195,4.3035,4.1506,4.238,4.0413,3.6699,3.6043,3.4514,3.4951,3.2766,3.2547,3.4732,3.5606,3.648,3.6917,3.4732,3.2329,3.211,3.5825,3.6262,3.5825,3.5388,3.6699,3.7136,3.7354,
3.8884,3.8884,3.8665,3.6699,3.7136,3.6917,3.7136,3.4951,3.2984,3.211,3.364,3.1236,3.0581,3.1018,3.1673,2.8177,3.1236,3.0581,2.9925,3.1892,3.1892,3.2984,3.1673,2.9488,3.1018,3.1018,
3.1455,2.7959,3.211,3.0799,3.0362,2.9925,2.8177,2.9488,3.0362,2.7959,3.1018,3.0581,2.9707,3.3858,3.211,3.1018,2.9925,3.0362,2.6648,2.7522,2.7522,2.8396,2.9488,2.9488,3.1236,3.1018,
2.7085,2.6648,3.0581,3.0144,2.774,2.5555,2.5555,2.927,2.8177,2.8614,2.5337,2.6429,2.9051,2.4681,2.4463,2.337,2.4026,2.2933,2.2933,2.4681,2.6429,2.5337,2.7303,2.7303,2.6429,2.8614,
2.9051,2.6648,2.5774,2.1185,2.2715,2.2715,2.2496,2.3152,2.2933,2.2278,2.9051,3.0799,2.6429,2.7085,2.2933,2.1404,2.3807,2.1404,2.1404,1.7908,1.7471,1.6815,1.9219,2.2715,1.9874,1.9219,
2.0093,1.8126,2.1404,1.5941,2.0093,1.616,1.3756,1.7689,1.7908,1.7689,2.49,1.9,1.7471,2.3807,2.5337,2.1185,2.2496,1.9437,2.1185,1.9874,2.2715,2.7522,2.053,1.4412,2.2059,1.4849,1.3975,
1.7034,1.5723,1.8126,1.3756,1.5504,1.4412,1.5941,1.6815,1.6597,1.9219,2.4681,2.2496,1.9219,2.4681,1.4412,1.1571,0.89495,1.1353,2.1185,1.5941,1.2445,0.89495,0.9168,0.98235,1.2227,
1.1353,1.4849,1.6378,1.5723,1.5286,1.9219,2.5774,2.8833,2.774,2.1622,1.9219,2.1185,2.2933,2.5992,2.1404,1.7908,2.1841,2.6429,2.927,2.6211,2.7085,1.5067,0.80755,1.2882,1.616,1.6597,
1.4849,1.8782,1.9437,2.0311,1.0479,1.5723,1.4849,2.0748,1.5067,1.616,2.3589,2.774,3.211,2.8177,2.5555,2.774,2.0748,2.0967,1.9219,1.6815,2.0093,2.1841,2.927,2.6211,3.3203,2.7303,2.337,
2.6648,2.7959,2.7303,1.9,3.0362,2.7959,2.927,3.7354,3.1236,2.927,2.4026,2.6429,2.7303,1.8345,1.8782,1.8563,1.6378,1.9656,2.2278,1.463,2.3807,2.3589,1.9219,1.9219,2.8614,2.927,2.4681,
2.5337,1.1353,1.5067,1.5067,1.8345,1.4412,1.5067,2.0311,2.0748,2.4463,2.1622,2.8614,2.2496,2.8396,2.9488,2.9051,3.0144,4.0195,3.0144,2.2933,3.1236,3.9321,2.2715,2.8614,3.0362,2.7303,
2.7959,2.2278,3.0144,3.1892,3.364,2.9925,1.7034,1.6378,1.7689,2.6211,2.053,2.4681,3.0581,2.8833,2.6429,2.5555,3.1892,2.8833,3.1236,2.3807,2.7085,2.6429,3.9102,3.7354,3.4077,3.3421,
3.364,3.0144,3.5388,3.1018,3.6699,2.6429,3.3421,2.6648,2.4463,3.2547,2.6648,1.9874,2.6211,2.9925,2.5118,1.9437,2.3152,1.6378,1.8563,2.6429,2.6648,2.9707,2.0967,1.4193,2.4026,1.8782,
2.337,3.0581,3.2547,2.8177,3.5169,2.6866,2.0093,2.2496,2.2496,1.7034,1.7689,3.801,3.6043,3.2329,2.49,3.5825,3.7136,3.0144,4.6531,3.1455,3.648,2.5992,2.3152,1.1353,2.1841,1.6597,2.9925,
2.6429,1.5941,2.7522,3.1892,3.648,3.1018,2.1185,1.6378,1.5941,2.5555,2.4026,2.3589,2.1841,2.0967,3.0799,4.1943,4.1943,2.6648,4.7405,4.3035,3.6043,2.2278,3.6043,3.7354,4.2161,4.0195,
5.3742,3.9321,4.4565,1.9219,2.49,2.8177,2.6429,3.1673,3.0581,3.9758,4.1287,4.3691,3.6262,3.1455,3.3858,3.5825,3.3203,3.2984,2.7959,2.1622,2.053,3.4951,3.4732,3.648,3.0799,2.2278,3.211,
2.5337,2.4244,1.7689,1.3975,2.4026,2.7522,2.49,2.6429,2.8177,1.5941,1.463,1.3975,0.50165,2.927,3.4951,3.6699,2.9051,3.4295,2.7959,3.5388,2.053,1.4193,2.5774,1.7908,3.2547,3.7573,3.0362,
2.6866,2.7303,3.9102,3.4732,3.0799,2.3807,2.5555,3.0581,2.0748,1.8345,1.7471,5.0027,3.6699,4.1069,3.801,4.238,4.2598,3.9976,3.9976,3.6917,3.211,2.7522,2.9051,3.1892,2.9051,3.6699,4.7842,
3.6043,4.4565,3.7573,2.7522,2.0748,2.2715,2.0093,3.8665,3.6917,3.364,2.6211,2.5992,3.1892,3.364,3.1018,2.7303,2.2933,3.7354,4.6313,4.9153,2.5337,3.1892,4.0195,2.49,2.2496,2.6429,2.8614,
2.2933,3.2766,3.0581,2.9925,3.5388,3.8447,4.3035,4.3035,3.1892,2.4681,2.49,2.5118,2.2278,2.3807,3.801,3.5169,3.0799,2.5337,4.0632,3.2329,2.5774,4.8716,5.1775,5.1994,4.6968,4.4565,5.2431,
4.6313,4.1287,4.1943,3.9976,3.6043,1.7471,1.6597,0.63275,1.7034,2.3807,3.2329,4.085,3.8447,2.4026,3.3421,5.112,3.364,3.6917,3.7354,5.5271,3.9102,3.9539,3.1455,3.1455,3.2766,3.0799,3.1455,
2.9707,2.5992,2.4681,2.053,3.211,2.0748,1.7689,1.9,2.1622,1.2882,4.7624,5.8112,4.4346,3.9102,4.2817,3.9539,3.2766,4.2598,2.9051,3.5825,3.2984,3.1236,3.801,3.0799,4.3472,3.8228,3.9102,3.7136,
3.6262,3.2329,2.9925,3.1018,2.5118,1.7252,2.0748,1.8345,2.4681,1.8126,2.3152,2.8396,2.8396,2.1622,1.3756,2.0967,3.3858,3.6043,4.3472,3.6043,2.7522,3.1673,2.337,2.053,2.3589,3.8228,2.9707,
2.3152,1.8345,2.7085,2.7959,3.7573,4.4783,2.8614,2.5337,3.5606,3.0144,3.9758,3.4077,3.8447,2.927,3.1455,2.49,2.0311,1.6378,2.5118,3.0362,2.4463,2.8177,2.6211,2.774,2.2715,2.8614,2.8396,
2.5774,1.7252,1.3101,2.4681,2.5992,1.9219,1.7471,1.463,0.9605,2.49,2.337,2.0967,1.7689,1.6378,2.4463,1.8563,1.8126,1.8782,1.8782,2.6429,3.6262,4.0413,2.9707,3.9758,3.0581,3.2766,2.5118,
2.2715,1.6597,3.0581,2.4026,3.6043,2.8833,3.5169,3.1455,2.49,2.337,1.8782,1.616,2.3152,2.2496,2.0093,2.053,0.89495,1.3538,1.0042,1.9656,2.2496,2.5774,2.7085,2.7085,4.3254,3.6043,3.211,
2.7959,3.2547,1.7252,2.053,2.3152,2.1404,1.4193,1.7034,2.1622,2.7959,2.6648,2.49,2.1841,2.6429,2.1622,1.8782,2.0748,1.9874,2.4463,1.9,0.80755,0.67645,0.63275,1.4849,1.2008,1.2008,2.5337,
2.9925,3.1892,3.5388,3.2766,3.2329,2.9707,3.4077,3.6917,3.8447,3.2329,2.8177,3.211,2.9707,2.9707,2.9925,3.0362,2.7085,2.6429,2.8177,2.6211,2.4463,2.4681,2.0311,2.1185,1.8345,2.2278,2.2496,
2.1185,2.2059,2.6648,2.5992,2.2933,2.1185,2.49,2.2715,2.337,2.2715,2.053,2.2278,2.2278,2.2059,2.2059,2.2059,2.2496,2.2933,2.2715,2.3152,2.2278,2.2059,2.2059,2.2278,2.2059,2.2059,2.1841,2.1622,
2.0093,1.9437,2.053,2.053,2.0311,2.1185,2.0967,2.0748,1.9874,1.9437,1.9,1.9,1.8782,1.9,1.9,1.7471,1.7034,1.7908,1.8126,1.7689,1.7689,1.6815,1.6597,1.5723,1.5723,1.616,1.5504,1.6815,1.6378,1.5941,
1.6597,1.7252,1.6815,1.6378,1.5504,1.5941,1.5941,1.5067,1.3756,1.4412,1.5504,1.6378,1.6597,1.6597,1.5067,1.5067,1.4193,1.3319,1.3319,1.2882,1.2664,1.2227,1.179,1.0916,1.0697,1.026,1.026,1.0479,
1.0479,1.0697,1.0479,1.0479,1.0042,0.9605,0.93865,0.93865,0.93865,0.9168,0.8731,0.8294,0.80755,0.85125,0.8294,0.8294,0.85125,0.89495,0.9168,0.89495,0.89495,0.8731,0.8731,0.8731,0.8294,0.76385,
0.67645,0.63275,0.6109,0.58905,0.58905,0.5672,0.5235,0.5672,0.54535,0.54535,0.5235,0.50165,0.4798,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,
0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.32685,0.32685,0.305,0.32685,0.3487,0.4798,0.6109,0.5235,0.305,0.305,0.305,0.305,
0.32685,0.305,0.305,0.50165,0.50165,0.45795,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.3487,0.6983,0.8731,0.76385,0.89495,0.8731,0.89495,0.8731,0.80755,0.72015,0.8294,0.9605,1.0916,0.93865,
0.80755,0.89495,0.8731,0.76385,0.6546,0.67645,0.85125,0.85125,0.9168,0.93865,0.6109,0.58905,0.8294,0.7857,0.72015,0.89495,0.85125,0.6109,0.45795,0.5235,1.2445,1.1571,1.1353,1.0916,0.6546,0.98235,
0.98235,1.0697,1.4849,1.4193,1.9656,1.9437,1.9437,1.6378,1.6378,1.4193,1.463,1.3756,1.2008,0.9168,0.8294,0.98235,1.0042,0.8731,0.8731,0.9168,1.0479,1.1571,1.0916,0.98235,1.2227,1.2008,1.3101,1.2882,
1.1571,1.2882,1.2664,1.2227,1.463,1.2882,1.5067,1.5723,1.8563,1.7034,1.5723,1.4412,1.463,1.179,1.0916,0.8294,0.9605,1.1134,1.4193,1.6378,1.7034,1.5723,1.5067,1.3101,1.2445,1.5723,1.8126,1.3538,
1.5504,1.4849,1.5286,0.58905,0.9168,0.9605,0.9605,0.63275,0.72015,0.6109,0.5672,1.1134,1.4412,1.3538,1.463,1.6378,1.9656,1.7908,2.0967,2.337,2.0311,2.1622,1.5723,1.5067,1.8345,1.5286,1.2445,1.2882,
1.0697,1.2664,1.3756,1.463,1.4849,1.6815,1.6378,1.4849,1.8782,1.4412,1.3101,1.4193,1.4849,1.3319,1.3101,1.5723,1.6378,1.7908,1.8126,1.8126,1.8126,1.9874,1.8563,2.053,2.0748,2.1622,2.2496,2.5337,
2.337,2.1622,2.1185,2.053,2.0748,2.0093,2.053,2.2933,2.2278,2.1185,2.053,2.0748,1.8345,1.8563,1.8782,1.5941,1.5286,1.8126,1.7034,1.8126,1.5941,1.7908,2.0093,1.8345,1.7689,1.616,1.7908,1.8126,1.8126,
1.616,1.3319,1.2445,1.463,1.8126,1.463,1.2445,1.6378,1.9437,1.4193,1.3975,1.5941,1.463,1.0916,1.5941,1.7689,1.3975,1.3975,1.2227,0.7857,1.0916,1.3538,1.0479,1.4193 };
double windSpeed1[] = { 4.5439,4.4565,4.522,4.5439,4.522,4.675,4.5876,4.6531,4.6313,4.8061,4.5876,4.6968,4.7842,4.7187,4.5876,4.522,4.6094,4.7187,4.9372,4.7187,4.6531,4.9809,4.085,
4.0632,4.1506,4.2161,4.2161,4.4565,4.3254,4.1069,4.0195,4.2817,4.1287,4.238,4.1943,3.9976,4.3254,4.238,4.238,3.9321,4.238,4.5657,4.238,4.1506,4.4783,4.522,4.2161,4.2598,4.522,
3.8884,3.8884,3.8447,4.1943,4.1287,3.9758,3.8884,3.8228,4.0632,3.9976,4.0195,4.0632,3.9758,4.2161,4.0632,4.0632,4.3472,4.522,4.3035,4.0195,4.6313,4.3472,4.5002,4.4128,4.3691,
4.5657,4.675,4.4128,4.3254,4.3035,4.238,4.0195,3.9758,3.9758,4.3254,3.9758,4.1724,3.8884,3.8447,3.6917,4.085,3.4514,3.4732,3.2766,3.6043,3.7354,3.5606,3.8665,3.5825,3.364,3.8665,
3.5169,3.4951,3.0581,2.8614,3.1673,3.1018,3.211,2.9488,2.8833,2.8396,2.9051,2.8614,3.3421,2.8833,3.4077,2.7085,3.1892,3.4951,3.3858,3.0362,3.0362,2.6429,2.5555,2.3589,2.3589,
2.4026,2.4463,1.7908,1.9874,2.0748,1.9656,2.0311,2.1404,2.0093,1.9874,2.053,2.1404,1.7908,1.5941,1.7034,1.7689,1.9437,1.3975,1.6378,2.0311,1.8563,1.7471,1.9656,2.0093,2.0093,
1.9656,1.8126,1.7689,1.9219,2.1404,2.1841,2.3152,2.3589,2.2933,2.2059,1.9874,2.0311,2.0093,2.0093,1.9437,1.9219,1.9,1.8782,1.9437,2.1404,2.1185,2.1404,2.1841,2.1185,1.9656,2.0093,
1.6815,1.5504,1.2008,0.98235,0.76385,0.9168,0.9168,0.89495,1.5723,1.4849,1.5067,1.7689,1.7689,1.7471,1.6378,1.616,1.2227,1.3756,1.2008,2.0967,1.7252,1.7034,1.8563,1.9,1.026,1.9,
1.616,2.2059,2.5337,2.5555,1.7471,2.4463,2.4244,2.5555,3.7354,3.6043,3.7573,3.7354,4.0632,3.9321,4.3254,4.5876,5.2212,4.7405,4.7842,4.522,3.6262,4.6968,4.675,4.085,3.9539,3.364,
3.5606,3.9102,3.5606,3.5606,3.4951,3.8447,3.6917,3.5606,3.801,3.8665,3.9102,3.7791,3.6917,3.8884,3.4514,3.7354,3.7354,3.364,3.4732,3.648,3.8884,4.0413,3.1673,3.5169,3.3858,2.774,
2.7522,3.3421,3.5606,3.2547,3.7791,3.6917,3.9758,4.2161,4.3472,3.7354,4.1287,4.2161,3.648,3.648,3.8884,4.085,4.2817,4.8716,4.6094,4.1943,4.085,4.3472,4.4783,4.522,4.7842,4.4346,
4.5002,4.7405,4.5876,4.6968,4.2598,4.522,4.6531,4.5876,3.7573,4.1069,4.4346,4.4565,4.3472,4.6968,4.8498,5.4397,4.8061,5.112,5.1775,5.3086,5.4616,5.0683,5.4834,4.6313,4.1724,4.085,
4.1287,4.0195,4.3035,4.1506,4.238,4.0413,3.6699,3.6043,3.4514,3.4951,3.2766,3.2547,3.4732,3.5606,3.648,3.6917,3.4732,3.2329,3.211,3.5825,3.6262,3.5825,3.5388,3.6699,3.7136,3.7354,
3.8884,3.8884,3.8665,3.6699,3.7136,3.6917,3.7136,3.4951,3.2984,3.211,3.364,3.1236,3.0581,3.1018,3.1673,2.8177,3.1236,3.0581,2.9925,3.1892,3.1892,3.2984,3.1673,2.9488,3.1018,3.1018,
3.1455,2.7959,3.211,3.0799,3.0362,2.9925,2.8177,2.9488,3.0362,2.7959,3.1018,3.0581,2.9707,3.3858,3.211,3.1018,2.9925,3.0362,2.6648,2.7522,2.7522,2.8396,2.9488,2.9488,3.1236,3.1018,
2.7085,2.6648,3.0581,3.0144,2.774,2.5555,2.5555,2.927,2.8177,2.8614,2.5337,2.6429,2.9051,2.4681,2.4463,2.337,2.4026,2.2933,2.2933,2.4681,2.6429,2.5337,2.7303,2.7303,2.6429,2.8614,
2.9051,2.6648,2.5774,2.1185,2.2715,2.2715,2.2496,2.3152,2.2933,2.2278,2.9051,3.0799,2.6429,2.7085,2.2933,2.1404,2.3807,2.1404,2.1404,1.7908,1.7471,1.6815,1.9219,2.2715,1.9874,1.9219,
2.0093,1.8126,2.1404,1.5941,2.0093,1.616,1.3756,1.7689,1.7908,1.7689,2.49,1.9,1.7471,2.3807,2.5337,2.1185,2.2496,1.9437,2.1185,1.9874,2.2715,2.7522,2.053,1.4412,2.2059,1.4849,1.3975,
1.7034,1.5723,1.8126,1.3756,1.5504,1.4412,1.5941,1.6815,1.6597,1.9219,2.4681,2.2496,1.9219,2.4681,1.4412,1.1571,0.89495,1.1353,2.1185,1.5941,1.2445,0.89495,0.9168,0.98235,1.2227,
1.1353,1.4849,1.6378,1.5723,1.5286,1.9219,2.5774,2.8833,2.774,2.1622,1.9219,2.1185,2.2933,2.5992,2.1404,1.7908,2.1841,2.6429,2.927,2.6211,2.7085,1.5067,0.80755,1.2882,1.616,1.6597,
1.4849,1.8782,1.9437,2.0311,1.0479,1.5723,1.4849,2.0748,1.5067,1.616,2.3589,2.774,3.211,2.8177,2.5555,2.774,2.0748,2.0967,1.9219,1.6815,2.0093,2.1841,2.927,2.6211,3.3203,2.7303,2.337,
2.6648,2.7959,2.7303,1.9,3.0362,2.7959,2.927,3.7354,3.1236,2.927,2.4026,2.6429,2.7303,1.8345,1.8782,1.8563,1.6378,1.9656,2.2278,1.463,2.3807,2.3589,1.9219,1.9219,2.8614,2.927,2.4681,
2.5337,1.1353,1.5067,1.5067,1.8345,1.4412,1.5067,2.0311,2.0748,2.4463,2.1622,2.8614,2.2496,2.8396,2.9488,2.9051,3.0144,4.0195,3.0144,2.2933,3.1236,3.9321,2.2715,2.8614,3.0362,2.7303,
2.7959,2.2278,3.0144,3.1892,3.364,2.9925,1.7034,1.6378,1.7689,2.6211,2.053,2.4681,3.0581,2.8833,2.6429,2.5555,3.1892,2.8833,3.1236,2.3807,2.7085,2.6429,3.9102,3.7354,3.4077,3.3421,
3.364,3.0144,3.5388,3.1018,3.6699,2.6429,3.3421,2.6648,2.4463,3.2547,2.6648,1.9874,2.6211,2.9925,2.5118,1.9437,2.3152,1.6378,1.8563,2.6429,2.6648,2.9707,2.0967,1.4193,2.4026,1.8782,
2.337,3.0581,3.2547,2.8177,3.5169,2.6866,2.0093,2.2496,2.2496,1.7034,1.7689,3.801,3.6043,3.2329,2.49,3.5825,3.7136,3.0144,4.6531,3.1455,3.648,2.5992,2.3152,1.1353,2.1841,1.6597,2.9925,
2.6429,1.5941,2.7522,3.1892,3.648,3.1018,2.1185,1.6378,1.5941,2.5555,2.4026,2.3589,2.1841,2.0967,3.0799,4.1943,4.1943,2.6648,4.7405,4.3035,3.6043,2.2278,3.6043,3.7354,4.2161,4.0195,
5.3742,3.9321,4.4565,1.9219,2.49,2.8177,2.6429,3.1673,3.0581,3.9758,4.1287,4.3691,3.6262,3.1455,3.3858,3.5825,3.3203,3.2984,2.7959,2.1622,2.053,3.4951,3.4732,3.648,3.0799,2.2278,3.211,
2.5337,2.4244,1.7689,1.3975,2.4026,2.7522,2.49,2.6429,2.8177,1.5941,1.463,1.3975,0.50165,2.927,3.4951,3.6699,2.9051,3.4295,2.7959,3.5388,2.053,1.4193,2.5774,1.7908,3.2547,3.7573,3.0362,
2.6866,2.7303,3.9102,3.4732,3.0799,2.3807,2.5555,3.0581,2.0748,1.8345,1.7471,5.0027,3.6699,4.1069,3.801,4.238,4.2598,3.9976,3.9976,3.6917,3.211,2.7522,2.9051,3.1892,2.9051,3.6699,4.7842,
3.6043,4.4565,3.7573,2.7522,2.0748,2.2715,2.0093,3.8665,3.6917,3.364,2.6211,2.5992,3.1892,3.364,3.1018,2.7303,2.2933,3.7354,4.6313,4.9153,2.5337,3.1892,4.0195,2.49,2.2496,2.6429,2.8614,
2.2933,3.2766,3.0581,2.9925,3.5388,3.8447,4.3035,4.3035,3.1892,2.4681,2.49,2.5118,2.2278,2.3807,3.801,3.5169,3.0799,2.5337,4.0632,3.2329,2.5774,4.8716,5.1775,5.1994,4.6968,4.4565,5.2431,
4.6313,4.1287,4.1943,3.9976,3.6043,1.7471,1.6597,0.63275,1.7034,2.3807,3.2329,4.085,3.8447,2.4026,3.3421,5.112,3.364,3.6917,3.7354,5.5271,3.9102,3.9539,3.1455,3.1455,3.2766,3.0799,3.1455,
2.9707,2.5992,2.4681,2.053,3.211,2.0748,1.7689,1.9,2.1622,1.2882,4.7624,5.8112,4.4346,3.9102,4.2817,3.9539,3.2766,4.2598,2.9051,3.5825,3.2984,3.1236,3.801,3.0799,4.3472,3.8228,3.9102,3.7136,
3.6262,3.2329,2.9925,3.1018,2.5118,1.7252,2.0748,1.8345,2.4681,1.8126,2.3152,2.8396,2.8396,2.1622,1.3756,2.0967,3.3858,3.6043,4.3472,3.6043,2.7522,3.1673,2.337,2.053,2.3589,3.8228,2.9707,
2.3152,1.8345,2.7085,2.7959,3.7573,4.4783,2.8614,2.5337,3.5606,3.0144,3.9758,3.4077,3.8447,2.927,3.1455,2.49,2.0311,1.6378,2.5118,3.0362,2.4463,2.8177,2.6211,2.774,2.2715,2.8614,2.8396,
2.5774,1.7252,1.3101,2.4681,2.5992,1.9219,1.7471,1.463,0.9605,2.49,2.337,2.0967,1.7689,1.6378,2.4463,1.8563,1.8126,1.8782,1.8782,2.6429,3.6262,4.0413,2.9707,3.9758,3.0581,3.2766,2.5118,
2.2715,1.6597,3.0581,2.4026,3.6043,2.8833,3.5169,3.1455,2.49,2.337,1.8782,1.616,2.3152,2.2496,2.0093,2.053,0.89495,1.3538,1.0042,1.9656,2.2496,2.5774,2.7085,2.7085,4.3254,3.6043,3.211,
2.7959,3.2547,1.7252,2.053,2.3152,2.1404,1.4193,1.7034,2.1622,2.7959,2.6648,2.49,2.1841,2.6429,2.1622,1.8782,2.0748,1.9874,2.4463,1.9,0.80755,0.67645,0.63275,1.4849,1.2008,1.2008,2.5337,
2.9925,3.1892,3.5388,3.2766,3.2329,2.9707,3.4077,3.6917,3.8447,3.2329,2.8177,3.211,2.9707,2.9707,2.9925,3.0362,2.7085,2.6429,2.8177,2.6211,2.4463,2.4681,2.0311,2.1185,1.8345,2.2278,2.2496,
2.1185,2.2059,2.6648,2.5992,2.2933,2.1185,2.49,2.2715,2.337,2.2715,2.053,2.2278,2.2278,2.2059,2.2059,2.2059,2.2496,2.2933,2.2715,2.3152,2.2278,2.2059,2.2059,2.2278,2.2059,2.2059,2.1841,2.1622,
2.0093,1.9437,2.053,2.053,2.0311,2.1185,2.0967,2.0748,1.9874,1.9437,1.9,1.9,1.8782,1.9,1.9,1.7471,1.7034,1.7908,1.8126,1.7689,1.7689,1.6815,1.6597,1.5723,1.5723,1.616,1.5504,1.6815,1.6378,1.5941,
1.6597,1.7252,1.6815,1.6378,1.5504,1.5941,1.5941,1.5067,1.3756,1.4412,1.5504,1.6378,1.6597,1.6597,1.5067,1.5067,1.4193,1.3319,1.3319,1.2882,1.2664,1.2227,1.179,1.0916,1.0697,1.026,1.026,1.0479,
1.0479,1.0697,1.0479,1.0479,1.0042,0.9605,0.93865,0.93865,0.93865,0.9168,0.8731,0.8294,0.80755,0.85125,0.8294,0.8294,0.85125,0.89495,0.9168,0.89495,0.89495,0.8731,0.8731,0.8731,0.8294,0.76385,
0.67645,0.63275,0.6109,0.58905,0.58905,0.5672,0.5235,0.5672,0.54535,0.54535,0.5235,0.50165,0.4798,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,
0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.32685,0.32685,0.305,0.32685,0.3487,0.4798,0.6109,0.5235,0.305,0.305,0.305,0.305,
0.32685,0.305,0.305,0.50165,0.50165,0.45795,0.305,0.305,0.305,0.305,0.305,0.305,0.305,0.3487,0.6983,0.8731,0.76385,0.89495,0.8731,0.89495,0.8731,0.80755,0.72015,0.8294,0.9605,1.0916,0.93865,
0.80755,0.89495,0.8731,0.76385,0.6546,0.67645,0.85125,0.85125,0.9168,0.93865,0.6109,0.58905,0.8294,0.7857,0.72015,0.89495,0.85125,0.6109,0.45795,0.5235,1.2445,1.1571,1.1353,1.0916,0.6546,0.98235,
0.98235,1.0697,1.4849,1.4193,1.9656,1.9437,1.9437,1.6378,1.6378,1.4193,1.463,1.3756,1.2008,0.9168,0.8294,0.98235,1.0042,0.8731,0.8731,0.9168,1.0479,1.1571,1.0916,0.98235,1.2227,1.2008,1.3101,1.2882,
1.1571,1.2882,1.2664,1.2227,1.463,1.2882,1.5067,1.5723,1.8563,1.7034,1.5723,1.4412,1.463,1.179,1.0916,0.8294,0.9605,1.1134,1.4193,1.6378,1.7034,1.5723,1.5067,1.3101,1.2445,1.5723,1.8126,1.3538,
1.5504,1.4849,1.5286,0.58905,0.9168,0.9605,0.9605,0.63275,0.72015,0.6109,0.5672,1.1134,1.4412,1.3538,1.463,1.6378,1.9656,1.7908,2.0967,2.337,2.0311,2.1622,1.5723,1.5067,1.8345,1.5286,1.2445,1.2882,
1.0697,1.2664,1.3756,1.463,1.4849,1.6815,1.6378,1.4849,1.8782,1.4412,1.3101,1.4193,1.4849,1.3319,1.3101,1.5723,1.6378,1.7908,1.8126,1.8126,1.8126,1.9874,1.8563,2.053,2.0748,2.1622,2.2496,2.5337,
2.337,2.1622,2.1185,2.053,2.0748,2.0093,2.053,2.2933,2.2278,2.1185,2.053,2.0748,1.8345,1.8563,1.8782,1.5941,1.5286,1.8126,1.7034,1.8126,1.5941,1.7908,2.0093,1.8345,1.7689,1.616,1.7908,1.8126,1.8126,
1.616,1.3319,1.2445,1.463,1.8126,1.463,1.2445,1.6378,1.9437,1.4193,1.3975,1.5941,1.463,1.0916,1.5941,1.7689,1.3975,1.3975,1.2227,0.7857,1.0916,1.3538,1.0479,1.4193 };

int main() {
    int data[MAX_SIZE],data1[MAX_SIZE], i, selection;
    double arrayGenTime, bubbleSortTime, quickSortTime;
    clock_t begin, end; //variables to get the CPU time to compare performance
    printf("1. Run Bubble Sort and Quick Sort for a general array.\n");
    printf("2. Run Bubble Sort and Quick Sort for the wind speed array.\n");
    printf("Enter selection (1|2) > ");
    scanf("%d", &selection);
    //give the user a chance to select which part of the question they wish to run.
    //1 - the general array sort
    //2 - the wind speed sort
    switch(selection) {
        case 1:
            //record and print time taken to create and copy the array
            begin = clock();    //start the clock
            generateRandomArray(data, MAX_SIZE); //generates the random array
            //since arrays are passed by reference the following bubbleSort function
            //will sort our array before we call the quickSort function
            //copyArray will make a new copy of the unsorted array for quickSort
            copyArray(data, data1, MAX_SIZE);
            end = clock();
            arrayGenTime = (double)(end - begin); 
            printf("Generate an array and print stats on the time it takes each algorithm to run.\n");
            printf("Array of size %i generated in time %lf\n\n", MAX_SIZE, arrayGenTime);
            begin = clock();
            bubbleSort(data, MAX_SIZE);
            end = clock();
            bubbleSortTime = (double)(end - begin);
            printf("Bubble Sort:\n");
            printf("Number of comparisons:\t\t%i\n", bcomparisons);
            printf("Number of swaps:\t\t%i\n", bswaps);
            printf("Bubble Sort ran in:\t\t%lf\n\n", bubbleSortTime);
            begin = clock();
            quickSort(data1, 0, MAX_SIZE);
            end = clock();
            quickSortTime = (double)(end - begin);
            printf("Quick Sort:\n");
            printf("Number of comparisons:\t\t%i\n", qcomparisons);
            printf("Number of swaps:\t\t%i\n", qswaps);
            printf("Quick Sort ran in:\t\t%lf\n\n", quickSortTime);
            break;
        case 2:     //for part 6 of question 1
            begin = clock();
            bubbleSort(windSpeed, sizeof(windSpeed));
            end = clock();
            bubbleSortTime = (double)(end - begin);
            printf("Bubble Sort:\n");
            printf("Number of comparisons:\t\t%i\n", bcomparisons);
            printf("Number of swaps:\t\t%i\n", bswaps);
            printf("Bubble Sort ran in:\t\t%lf\n\n", bubbleSortTime);
            begin = clock();
            quickSort(windSpeed1, 0, sizeof(windSpeed1) - 1);
            end = clock();
            quickSortTime = (double)(end - begin);
            printf("Quick Sort:\n");
            printf("Number of comparisons:\t\t%i\n", qcomparisons);
            printf("Number of swaps:\t\t%i\n", qswaps);
            printf("Quick Sort ran in:\t\t%d\n\n", quickSortTime);
            break;
        default:
            printf("Invalid selection. Exiting.");
            return 1;
    }
    return 0;
}

//from https://stackoverflow.com/questions/13802399/generate-a-random-array-in-c
//accepts an array variable passed by reference and the required size of the array
//there is no return value since it is modifying the array directly
void generateRandomArray(int a[], int arraySize) {
    int i;
    srand(time(NULL));
    for(i=0; i < arraySize; i++) {
        a[i] = (rand() % 51);
    }
}

//function to copy array a to array b
//this is used to preserve the contents of the generated array in order to 
//pass to the next sorting function
void copyArray(int a[], int b[], int arraySize) {
    for(int i = 0; i < arraySize; i++) {
        b[i] = a[i];
    }
}

//function to print the contents of the passed array
void printArray(const int a[], int dimension) {
    for(int i = 0; i < dimension; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }
        printf("%4d", a[i]);
    }
}

//Implementataion of the O(N^2) bubble sort algorithm
//we're using this in the hope it will show a large difference in performance
//against the other algorithm
//function that sorts an array with bubble sort algorithm */
void bubbleSort(double a[], int dimension) {
    int i, step, temp;
    for(step=0; step < dimension; ++step) {
        for(i=0; i < MAX_SIZE-step-1; ++i) {
            if(a[i] > a[i+1]) {
                temp = a[i];
                a[i] = a[i+1];
                a[i+1] = temp;
                ++bcomparisons;
                ++bswaps;
            }
        }
    }
}

//runs the quicksort algorithm on the input array with
//the first and last array indexes
//from https://beginnersbook.com/2015/02/quicksort-program-in-c/
void quickSort(double a[], int first, int last){
   int i, j, pivot;
   double temp;
   
   if(first < last) {
       ++qcomparisons;  //increment the comparisons variable
       pivot = first;   //use the first element as the pivot
       i = first;
       j = last;
       while(i < j) {
           ++qcomparisons;
           while(a[i] <= a[pivot] && i < last) {
               ++qcomparisons;
               i++;
           }
           while(a[j] > a[pivot]) {
               ++qcomparisons;
               j--;
           }
           if(i < j) {
               ++qcomparisons;
               ++qswaps;
               temp = a[i];
               a[i] = a[j];
               a[j] = temp;
           }
       }
      temp = a[pivot];
      a[pivot] = a[j];
      a[j] = temp;
      ++qswaps;
      quickSort(a, first, j - 1);
      quickSort(a, j + 1, last);
   }
}