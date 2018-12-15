/*

Notes on this assignment:
- Why are the example files distributed as .rar? Why aren't they all provided as a zip package?
- Too many discussion posts, I found myself hopping from one to another trying to find the right info
- Final exam was presented in a totally new way and contained item we had just touched on in the learning modules
- I did all this in my degree course so was familiar with it but I can imagine someone who has never coded having an awful time with this.
- we never had an assignment with structs and I've never had to create an array of structs, much less read them into a dynamic array

Questions:
- Are the values provided in the enum datatypes correct?

TODO:
- setup a loop in main to keep looping the menu
*/


//program includes
#include <stdio.h>
#include <time.h>       //for the time_t variable in the LoanRequests struct
#include <string.h>     //for loading the loan offers files
#include <stdlib.h>     //for the exit() function call
#include "dirent.h"

#define LOAN_REQUESTS_PATH "D:\\Backup\\OneDrive\\Users\\David\\Scripting\\C\\Week 8\\loanrequests.txt"
#define LOAN_OFFERS_PATH "D:\\Backup\\OneDrive\\Users\\David\\Scripting\\C\\Week 8\\loanoffers\\"
#define VALID_LOANREQUEST_TIMESTAMP_LENGTH 10
#define VALID_LOANOFFER_TIMESTAMP_LENGTH 10
#define VALID_LOANOFFERFILE_LENGTH 31

//function definitions
int display_menu                    ();                         //Displays the main menu, returns the selection
int load_request_data               ( char* );                  //Loads the data from the loanrequests.txt file
int load_loan_offers                ();                         //Loads the data from the loanoffers/ files
void view_loan_requests             ( int );                    //Displays the loan requests data
void exit_CLDA                      ();                         //Exits the application, saves any open file pointers
void get_loan_offers_files          ( char* );                  //Returns an array of filenames in a directory
void extract_requestloan_timestamp  ( const char *, char * );   //Extracts the RequestLoan timestamp from the filename
void extract_loanoffer_timestamp    ( const char *, char * );   //Extracts the Loanoffer timestamp from the filename
void view_loan_offers               ( int );                    //Displays the loan offers data
void request_loan                   ( int );

//struct type definitions, defined here for use throughout the program
typedef enum { Requested = 0, OffersAvailable = 1, Complete = 2 } Status;
typedef struct {
    time_t RequestTimeStamp;
    float AmountRequested;
    float AmountPledged;
    int LoanPeriod;
    float InterestRate;
    Status Status;
    
} LoanRequests;

typedef enum { Offered = 0, Accepted = 1 } AcceptedFlag;
typedef struct {
    time_t RequestTimeStamp;
    time_t OfferTimeStamp;
    char *OfferAddress;
    float OfferRate;
    AcceptedFlag AcceptedFlag;
} LoanOffers;

//struct array variable definitions. Defined here to take advantage of C's pass by reference for arrays
LoanRequests lr[50];    //create an array of structs of type LoanRequest used in the load_request_data function
LoanOffers lo[50];      //create an array of structs of type LoanOffers used in the load_loan_offers function

int main() {
    int selection, lr_cursor, lo_cursor = 0;
    //load the loanrequests.txt file into the array of LoanRequests structs above for use later,
    //it returns an int representing where it is in the array so that we can append later if needed
    lr_cursor = load_request_data(LOAN_REQUESTS_PATH);
    //load the load offers data from the files in the loadoffers directory, return the number of lines
    lo_cursor = load_loan_offers();
    printf("lr_cursor: %i\n", lr_cursor);
    printf("lo_cursor: %i\n", lo_cursor);
    exit(1);
    selection = display_menu();
    switch ( selection ) {
        case 1:     //view loan requests
            view_loan_requests(lr_cursor);
            break;
        case 2:     //request a loan
            request_loan(lr_cursor);
            break;
        case 3:     //view loan offers
            view_loan_offers(lo_cursor);
            break;
        case 4:     //sort loan offers
            printf("Sort Loan Offers\n");
            break;    
        case 5:     //accept loan offers
            printf("Accept Loan Offer\n");
            break;
        case 6:     //view account wallet
            printf("View Account Wallet\n");
            break;
        case 7:     //save and exit
            printf("\n\nExiting the CLDA application...\n");
            exit_CLDA();
            break;
        default:
            printf("Unknown option selected\n");
            break;
    }
    return 0;
}

int display_menu() 
{
    int i;
    printf("\nPlease select from one of the following options:\n\n");
    printf("1. View Loan Requests\n");
    printf("2. Request a Loan\n");
    printf("3. View Loan Offers\n");
    printf("4. Sort Loan Offers\n");
    printf("5. Accept Loan Offer\n");
    printf("6. View Account Wallet\n");
    printf("7. Save and Exit CLDA\n\n");
    printf("Enter an option [1 - 6] (7 to exit): ");
    scanf("%d", &i);
    return i;
}

void request_loan( int cursor)
{
    int loan_period, status;
    float amount_requested, amount_pledged, interest_rate;
    time_t request_timestamp;
    printf("Please specify loan terms:\n\n");
    printf("Amount requested (ETH): ");
    scanf("%f", &amount_requested);
    printf("Collateral Required: ");
    scanf("%f", &amount_pledged);
    printf("Enter loan period in months (30 Days): ");
    scanf("%i", &loan_period);
    printf("Max Monthly Interest (MMPR): ");
    scanf("%f", &interest_rate);
    request_timestamp = (unsigned long)time(NULL); //from: https://stackoverflow.com/questions/11765301/how-do-i-get-the-unix-timestamp-in-c-as-an-int
    printf("size of lr: %i\n", sizeof lr);
    printf("position in array:\n", cursor);
}

int load_request_data( char *filename)
{
    FILE *ifp; //infilepointer of type FILE*
    char line[500];
    char *token[200];
    int i, tokenCount = 0;
    int j = 0;
    int num_lines = 0;
    ifp = fopen(filename,"r"); // set that pointer to point to the file the user entered before in the main function
    if (ifp == NULL) {
        printf("Cannot open file for reading or file not found.\nCheck the value of LOAN_REQUESTS_PATH or access rights to %s\n\n", filename );
        exit(1);
    } 
    //from: https://stackoverflow.com/questions/32959253/c-using-strtok-to-parse-an-input-file
    while ( fgets( line, sizeof line, ifp ) != NULL ) {
        token[0] = strtok( line, " " );
        ++num_lines;
        //break the line into tokens based on the space character
        for ( i = 1; i < 200; i++ ) {
            if ( (token[i] = strtok( NULL, " " )) == NULL ) { break; }
        }
        tokenCount = i;
        //assign the values of each element in the token array to the appropriate member variable
        lr[j].RequestTimeStamp = atoi(token[0]);
        lr[j].AmountRequested = atoi(token[1]);
        lr[j].AmountPledged = atoi(token[2]);
        lr[j].LoanPeriod = atoi(token[3]);
        lr[j].InterestRate = atof(token[4]);
        lr[j].Status = atoi(token[5]);
        j++;
        // printf("NUMLINES: %d\n", num_lines);
    }
    fclose(ifp); // close the file handler
    return(num_lines);
}

//  TODO: Dynamically pull list of files using the get_loan_offers_files() function
int load_loan_offers() 
{
    //had to hardcode the filenames as I ran out of time to try and return the list of files from the directory
    char *filenames[10] = {"loanoffers\\1544021094_1544024694_Offer.txt", "loanoffers\\1544698794_1544702394_Offer.txt","loanoffers\\1544698794_1544716794_Offer.txt","loanoffers\\1544921094_1544939094_Offer.txt","loanoffers\\1544921094_1544946294_Offer.txt","loanoffers\\1545021094_1545028594_Offer.txt","loanoffers\\1545021094_1545035494_Offer.txt","loanoffers\\1545221094_1545224694_Offer.txt","loanoffers\\1545221094_1545229499_Offer.txt","loanoffers\\1545221094_1545239094_Offer.txt" };
    //had to hardcode the list of filenames for the extract_requestloan_timestamp and extract_loanoffer_timestamp functions due to time constraints
    char *filenames1[10] = {"1544021094_1544024694_Offer.txt", "1544698794_1544702394_Offer.txt","1544698794_1544716794_Offer.txt","1544921094_1544939094_Offer.txt","1544921094_1544946294_Offer.txt","1545021094_1545028594_Offer.txt","1545021094_1545035494_Offer.txt","1545221094_1545224694_Offer.txt","1545221094_1545229499_Offer.txt","1545221094_1545239094_Offer.txt" };
    char *requestloantimestamp;
    char *loanoffertimestamp;
    char loanrequest_timestamp[VALID_LOANREQUEST_TIMESTAMP_LENGTH + 1];
	char loanoffer_timestamp[VALID_LOANOFFER_TIMESTAMP_LENGTH + 1];
    FILE *ifp; //infilepointer of type FILE*
    char line[500];
    char file[500] = {}; //had to initialise this array at declaration as it had random data in it causing the file path to be invalid
    char *token[200];
    int i, tokenCount = 0;
    int counter = 0;
    // get_loan_offers_files(LOAN_OFFERS_PATH);
    // memset(requestloantimestamp, '\0', sizeof(requestloantimestamp));
    // for (i = 0; i < sizeof(filenames); i++) {
    //      extract_requestloan_timestamp(filenames1[i], requestloantimestamp);
    //      extract_loanoffer_timestamp(filenames1[i], loanoffertimestamp);
    // }
     printf("Size: %i\n", sizeof(filenames));
    for (counter = 0; counter < 10; counter++) {
        // printf("In for loop no: %i\n", counter);
        // printf("%s", strcat(strcat(file, LOAN_OFFERS_PATH), filenames[i]));
        // strcat(strcat(file, LOAN_OFFERS_PATH), filenames[i]); //https://stackoverflow.com/questions/308695/how-do-i-concatenate-const-literal-strings-in-c
        // printf("%i: %s\n", counter, filenames1[counter]);
        // strncpy(requestloantimestamp, filenames1[counter], VALID_LOANREQUEST_TIMESTAMP_LENGTH);
	    // requestloantimestamp[VALID_LOANREQUEST_TIMESTAMP_LENGTH] = '\0';
	    //printf("RequestLoan timestamp Extracted is: %s\n", requestloantimestamp);
        ifp = fopen(filenames[counter],"r");
        if (ifp == NULL) {
            // printf("Cannot open file for reading or file not found.\nCheck the value of LOAN_REQUESTS_PATH or access rights to %s\n\n", file);
            // exit(1);
            break;
        } 
        //from: https://stackoverflow.com/questions/32959253/c-using-strtok-to-parse-an-input-file
        while ( fgets( line, sizeof line, ifp ) != NULL ) {
            token[0] = strtok( line, " " );
            //break the line into tokens based on the space character
            for ( i = 1; i < 500; i++ ) {
                if ( (token[i] = strtok( NULL, " " )) == NULL ) { break; }
            }
            tokenCount = i;
            //assign the values of each element in the token array to the appropriate member variable
            // printf("%s\n", requestloantimestamp);
            // lo[j].RequestTimeStamp = atoi(token[0]);
            // lo[j].OfferTimeStamp = atoi(token[1]);
            lo[counter].OfferAddress = malloc(strlen(token[0]) + 1); //from:https://stackoverflow.com/questions/3131319/how-to-correctly-assign-a-new-string-value
            strcpy(lo[counter].OfferAddress, token[0]);
            lo[counter].OfferRate = atof(token[1]);
            lo[counter].AcceptedFlag = atoi(token[2]);
            // printf("%i: %s, %.2f, %i\n", counter, lo[counter].OfferAddress, lo[counter].OfferRate, lo[counter].AcceptedFlag);
        }
    }
    // printf("Out of while\n");
    fclose(ifp); // close the file handler
    return(counter);    
}
/*
* Function Name: extract_requestloan_timestamp
* Purpose: Extract the 10 chars that represent the  loanrequest timestamp
* Paramaters: char * Filename
*			  char *requestloantimestamp - address of string to store loan offertimestamp
*
* Example: 1544021094_1544024694_Offer.txt
* Total length VALID_LOANREQUEST_TIMESTAMP_LENGTH & "_" VALID_LOANOFFER_TIMESTAMP_LENGTH + strlen(_Offer.txt)+ \0
* Places 1544021094 to parameter requestloantimestamp
*/
void extract_requestloan_timestamp(const char *filename, char *requestloantimestamp)
{
    printf("In function extract_requestloan_timestamp");
	strncpy(requestloantimestamp, filename, VALID_LOANREQUEST_TIMESTAMP_LENGTH);
	requestloantimestamp[VALID_LOANREQUEST_TIMESTAMP_LENGTH] = '\0';
	printf("RequestLoan timestamp Extracted is: %s\n", requestloantimestamp);
}
/*
* Function Name : extract_loanoffer_timestampPurpose : Extract the 10 chars that represent the loanoffer timestamp
* Parameter: Valid loanofferFilename, address of string to store loan offertimestamp
*
* Example: 1544021094_1544024694_Offer.txt
* Total length VALID_LOANREQUEST_TIMESTAMP_LENGTH & "_" VALID_LOANOFFER_TIMESTAMP_LENGTH + strlen(_Offer.txt)+ \0
* Returns 1544024694 in loanoffertimestamp
*/
void extract_loanoffer_timestamp(const char *filename, char *loanoffertimestamp)
{
	char temploanofferfilename[VALID_LOANOFFERFILE_LENGTH + 1];
	strcpy(temploanofferfilename, filename);
	char * extractdash;
	extractdash = strtok(&temploanofferfilename[VALID_LOANOFFER_TIMESTAMP_LENGTH + 1], "_");

	if (extractdash != NULL)
	{
		//strncpy(loanoffertimestamp, extractdash, VALID_LOANOFFER_TIMESTAMP_LENGTH);
		strcpy(loanoffertimestamp, extractdash);
		loanoffertimestamp[VALID_LOANOFFER_TIMESTAMP_LENGTH] = '\0';
		printf("Loanoffer timestamp extracted: %s\n", loanoffertimestamp);
	}

}

//https://www.tutorialspoint.com/cprogramming/c_return_arrays_from_function.htm
void get_loan_offers_files(char* path) 
{
    struct dirent *de;  // Pointer for directory entry 
    int num_files = 0;
    char *files[1024];
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir(path); 
    if (dr == NULL) { 
        printf("Could not open current directory" ); 
        exit(1);
    }
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
    // for readdir() 
    while ((de = readdir(dr)) != NULL) {
        // strcpy(de->d_name, files[num_files]);
        // printf("de->de_name: %s\n", de->d_name) ;
        // printf("files[num_files]: %s\n", files[num_files]);
        ++num_files;
    }
    // printf("\n%i\n", num_files);
    // for (int i = 0; i < num_files; i++) {
    //     printf("%i: %s\n", i, files[i]);
    // }
    closedir(dr); 
    // return files;   
}

// TODO: Make the output prettier
void view_loan_requests(int num_lines) 
{
    for (int i = 0; i < num_lines; i++) {
        printf("Record: %i: Timestamp: %i; AmtRequested: %lf; AmtPledged: %lf;Loan Period: %i; IntRate: %lf; Status: %i\n", i, lr[i].RequestTimeStamp, lr[i].AmountRequested, lr[i].AmountPledged, lr[i].LoanPeriod, lr[i].InterestRate, lr[i].Status);
    }
}

//TODO: Still need to fix up the reader function above and change srtuct ref here, make output as above
void view_loan_offers(int num_lines) 
{
    for (int i = 0; i < num_lines; i++) {
        printf("Record: %i: Offer Address: %s; Offer Rate: %.2f; Accepted Flag: %i\n", i, lo[i].OfferAddress, lo[i].OfferRate, lo[i].AcceptedFlag);
    }
}
/*
 *
 * Name:        exit_CLDA
 * Parameters:  None
 * Returns      void
 * Purpose:     This function is called whenever the user chooses to exit the system. 
 *              saves data and closes all open files.
 *
 */
void exit_CLDA() 
{
    //save data
    //check if filehandles are open (pass as params?) and closes them
    exit(1);
}