#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main(){
    FILE *ifp; //infilepointer of type FILE*
    char line[500];
    char *token[200];
    int i, tokenCount=0;
    char fileName[] = "loanrequests.txt";
    ifp = fopen(fileName,"r");
    while ( fgets( line, sizeof line, ifp ) != NULL ) {
        token[0] = strtok( line, " " );

    // break the line into tokens
        for ( i = 1; i < 200; i++ ) {
            if ( (token[i] = strtok( NULL, " " )) == NULL ) { break; }
        }
        tokenCount = i;
        printf( "%s %s %s %s %s %s", token[0], token[1], token[2], token[3], token[4], token[5] );
    }
}

FILE *gfopen(char *filename, char *mode) {
    FILE *fp;
    if ( ( fp = fopen ( filename , mode ) ) == NULL ) {
        fprintf(stderr, "Canot open file or file not found");
        exit(1);
    }
    return fp;
}