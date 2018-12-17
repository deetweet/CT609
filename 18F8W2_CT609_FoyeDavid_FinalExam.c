/*
 *
 * Title: week7-assignment1.c
 * Author: David Foye
 * Course: CT609
 * Assignment: Exam
 * Purpose: Write a program that models a real-world loan application
 *
*/

//program includes
#include <stdio.h>      //used in printf() and scanf() functions
#include <time.h>       //for the time_t variable in the LoanRequests struct, date/time in the view_account_wallet() function
#include <string.h>     //for loading the loan offers files
#include <stdlib.h>     //for the exit() function call
#include "dirent.h"     //getting the list of loanoffer files
#include <conio.h>      //used for the getc() function to pause the output

#define LOAN_REQUESTS_PATH "D:\\Backup\\OneDrive\\Users\\David\\Scripting\\C\\Week 8\\loanrequests.txt"
#define LOAN_REQUESTS_TMP_PATH "D:\\Backup\\OneDrive\\Users\\David\\Scripting\\C\\Week 8\\loanrequests.bak"
#define LOAN_OFFERS_PATH "D:\\Backup\\OneDrive\\Users\\David\\Scripting\\C\\Week 8\\loanoffers\\"
#define VALID_LOANREQUEST_TIMESTAMP_LENGTH 10
#define VALID_LOANOFFER_TIMESTAMP_LENGTH 10
#define VALID_LOANOFFERFILE_LENGTH 31
#define WALLET_ETH_PUB_ADDR "0x23bd14810c04739b16Ee5a835114C13A8A2C6166"
#define WALLET_CCOIN_PUB_ADDR "0x80fB784B7eD66730e8b1DBd9820aFD29931aab03"

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

//function definitions
int display_menu                        ();                         //Displays the main menu, returns the selection
int load_request_data                   ( char* );                  //Loads the data from the loanrequests.txt file
int load_loan_offers                    ();                         //Loads the data from the loanoffers/ files
void view_loan_requests                 ( int );                    //Displays the loan requests data
void exit_CLDA                          ();                         //Exits the application, saves any open file pointers
void get_loan_offers_files              ( char* );                  //Returns an array of filenames in a directory
void extract_requestloan_timestamp      ( const char *, char * );   //Extracts the RequestLoan timestamp from the filename
void extract_loanoffer_timestamp        ( const char *, char * );   //Extracts the Loanoffer timestamp from the filename
void view_loan_offers                   ( int );                    //Displays the loan offers data
int request_loan                        ( int );                    //Records data and writes to the LoanRequests array
void view_account_wallet                ();                         //Displays ETH wallet  information
void sort_loan_offers                   ();                         //Sorts and displays the values in the loan offers array
void sort_loanoffer_interestrate        ( LoanOffers*, int );       //Utility function for sort_loan_offers() sorts by Interest Rate
void sort_loanoffer_requesttimestamp    ( LoanOffers*, int );       //Utility function for sort_loan_offers() sorts by Requested Timestamp
void accept_loan_offer                  ( int );                    //Displays the loan offers and allows the user to update the loan offer status
void update_loan_request                ( int );                    //Utility function for accept_loan_offer() to update the corresponding loan request given the request timestamp
void display_manual                     ();                         //Utility function to display the user manual
void save_data                          ( int, int );               //Saves the data to disk

//struct array variable definitions. Defined here to take advantage of C's pass by reference for arrays
LoanRequests lr[100];    //create an array of structs of type LoanRequest used in the load_request_data function
LoanOffers lo[100];      //create an array of structs of type LoanOffers used in the load_loan_offers function

int main() {
    int selection, lr_cursor, lo_cursor = 0;
    //load the loanrequests.txt file into the array of LoanRequests structs above for use later,
    //it returns an int representing where it is in the array so that we can append later if needed
    lr_cursor = load_request_data(LOAN_REQUESTS_PATH);
    //load the load offers data from the files in the loadoffers directory, return the number of lines
    lo_cursor = load_loan_offers();
    do {
        selection = display_menu();
        switch ( selection ) {
            case 1:     //view loan requests
                view_loan_requests(lr_cursor);
                break;
            case 2:     //request a loan
                lr_cursor = request_loan( lr_cursor );
                view_loan_requests( lr_cursor );
                break;
            case 3:     //view loan offers
                view_loan_offers( lo_cursor );
                break;
            case 4:     //sort loan offers
                sort_loan_offers( lo_cursor );
                break;    
            case 5:     //accept loan offers
                accept_loan_offer( lo_cursor );
                view_loan_offers(lo_cursor);
                break;
            case 6:     //view account wallet
                view_account_wallet();
                break;
            case 7:     //save and exit
                printf("\n\nExiting the CLDA application...\n");
                save_data( lo_cursor, lr_cursor );
                exit_CLDA();
                break;
            case 1953974144: //value of the int variable 'selection' when the letter 'h' is entered
                display_manual();
                break;
            default:
                printf("Unknown option selected\n");
                break;
        }
    } while (1);
    return 0;
}
/*
 ********************************************************************************************
 * Name:        display_menu
 * Parameters:  void
 * Returns      int - selection choosen
 * Purpose:     Displays the main menu
 *              Called by main()
 ********************************************************************************************
 */
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
    printf("Enter an option [1 - 6] (7 to exit, h for help): ");
    scanf("%d", &i);
    return i;
}
/*
 ********************************************************************************************
 * Name:        display_manual
 * Parameters:  void
 * Returns      void
 * Purpose:     Displays a small 'manual' to the user
 *              Called by main()
 ********************************************************************************************
 */
void display_manual()
{
    printf("\n==================================================\n| Welcome to the CLDA Application - User Manual! |\n==================================================\n");
    printf("\nHere is a brief description of the various functions of the program\n");
    printf("\n1. View Loan Requests:\nHere you can view the active loan requests awaiting an offer, the data is displayed as a table. Press any key on the keyboard to return to the main menu.\n");
    printf("\n2. Request a Loan:\nAllows the user to request a loan by filling in a few details.\n");
    printf("\n3. View Loan Offers:\nHere you can view the active loan offers on file, the data is displayed as a table. Press any key on the keyboard to return to the main menu.\n");
    printf("\n4. Sort Loan Offers:\nThis part of the program allows you to sort the data from the loan offers files by Requested Timestamp or by the Interest Rate fields. Press any key to return to the main menu.\n");
    printf("\n5. Accept Loan Offer:\nThis will list all the loan offers on file and prompt the user to select which record they wish to update in order to set its' status to 'Accepted'.\n");
    printf("\n6. View Account Wallet:\nThis displays the user's account wallet.\n");
    printf("\n7. Save and Exit CLDA:\nFlushes all data to the disk and exits the application.\n\n{end help}\n");
    printf("\nPress any key to return to the Main Menu...\n");  
    getch();
}
/*
 ********************************************************************************************
 * Name:        request_loan
 * Parameters:  int - number of records in the array
 * Returns      void
 * Purpose:     Requests data from the user and adds it to the loan request array
 *              Called by main()
 ********************************************************************************************
 */
int request_loan( int cursor )
{
    int loan_period, status;
    float amount_requested, amount_pledged, interest_rate;
    time_t request_timestamp;
    //prompt user to enter the required details
    //there is no error checking here as I ran out of time
    printf("Please specify loan terms:\n\n");
    printf("Amount requested (ETH): ");
    scanf("%f", &amount_requested);
    printf("Collateral Required: ");
    scanf("%f", &amount_pledged);
    printf("Enter loan period in months (30 Days): ");
    scanf("%i", &loan_period);
    printf("Max Monthly Interest (MMPR): ");
    scanf("%f", &interest_rate);
    //get the current time in unixtime
    request_timestamp = (unsigned long)time(NULL);
    ++cursor;
    //make space for the new element
    realloc(lr, (cursor+1)*sizeof(LoanRequests));
    //add the input values to the array
    lr[cursor].RequestTimeStamp = request_timestamp;
    lr[cursor].AmountRequested = amount_requested;
    lr[cursor].AmountPledged = amount_pledged;
    lr[cursor].LoanPeriod = loan_period;
    lr[cursor].Status = 0;
    return(cursor);
}
/*
 ********************************************************************************************
 * Name:        view_account_wallet
 * Parameters:  void
 * Returns      void
 * Purpose:     Displays some data around the users' wallet
 *              Called by main()
 ********************************************************************************************
 */
void view_account_wallet() 
{
    //from: https://stackoverflow.com/questions/1442116/how-to-get-the-date-and-time-values-in-a-c-program
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    printf("Date/Time(YYYY-MM-DD tt:mm:ss): %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("User's Wallet ETH Public Address: %s\n", WALLET_ETH_PUB_ADDR);
    printf("User's Wallet CT609 Coin Address: %s\n", WALLET_CCOIN_PUB_ADDR);

    //TODO
    // 4. The total amount of ETH coin held by the user.
    // 5. The total amount of CT609 (C) coin held by the user.
    // 6. The loan offers that have been accepted. 

    printf("\n\nPress any key to return to the Main Menu...\n");  
    getch();
   
}
/*
 ********************************************************************************************
 * Name:        load_request_data
 * Parameters:  char* filename - path to the loan requests file
 * Returns      int, num_lines - number of lines imported
 * Purpose:     Loads the data in loanrequests.txt into the LoansRequest struct
 *              Called by main()
 ********************************************************************************************
 */
int load_request_data( char *filename )
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
    }
    fclose(ifp); // close the file handler
    return(num_lines);
}
/*
 ********************************************************************************************
 * Name:        accept_loan_offer
 * Parameters:  int num_entries - dimension of the array
 * Returns      void
 * Purpose:     Sets the loan offer flag to accepted
 *              Called by main()
 ********************************************************************************************
 */
void accept_loan_offer( int num_entries ) 
{
    int offer_id = 0;
    printf("Please select one of the loan offers below: ");
    sort_loanoffer_requesttimestamp(lo, num_entries);
    view_loan_offers(num_entries);
    printf("Offer ID: ");
    scanf("%d", &offer_id);
    //loop through the lo array to find the entry and update the corresponding 
    //entry to 'Accepted'
    for (int i = 0; i < num_entries; i++) {
        if (i == offer_id) {
            lo[i].AcceptedFlag = 1;
            //call to the update_loan_request to update the required items in corresponding loan requests record
            update_loan_request(lo[i].RequestTimeStamp);
        }
    }
}
/*
 ********************************************************************************************
 * Name:        update_loan_request
 * Parameters:  int timestamp - Timestamp of record to update
 * Returns      void
 * Purpose:     Updates the status of the loan request. I didn't understand the remainder of 
 *              the question so that was all I could implement. Called by accept_loan_offer()
 ********************************************************************************************
 */
void update_loan_request( int timestamp )
{
    for(int i = 0; i < sizeof lr; i++) {
        if(timestamp == lr[i].RequestTimeStamp) {
            lr[i].Status = 2;
        }
    }
}
/*
 ********************************************************************************************
 * Name:        sort_loan_offers
 * Parameters:  int num_entries - dimension of the array
 * Returns      void
 * Purpose:     Prompts the user to select the sorting option
 *              Called by main()
 ********************************************************************************************
 */
void sort_loan_offers( int num_entries ) 
{
    int opt = 0;
    printf("Please select a field to sort by.\n1. Request TimeStamp\n2. Interest Rate\n\nOption: ");
    scanf("%d", &opt);
    if (opt == 1) {
        sort_loanoffer_requesttimestamp(lo, num_entries);    
        printf("\nList sorted by Request Time Stamp:\n\n");
    } else if(opt == 2) {
        sort_loanoffer_interestrate(lo, num_entries);
        printf("\nList sorted by Interest Rate:\n\n");
    }
    view_loan_offers(num_entries);
}
/*
 ********************************************************************************************
 * Name:        sort_loanoffer_requesttimestamp
 * Parameters:  LoanOffers array, int n - dimension of the array
 * Returns      void
 * Purpose:     Sorts the loan offers array by the Request Timestamp field
 *              Called by sort_loan_offers()
 ********************************************************************************************
 */
void sort_loanoffer_requesttimestamp( LoanOffers* loan_offers, int n ) 
{
    int j,i;
    for(i=1;i<n;i++) {
        for(j=0;j<n-i;j++) {
            if(loan_offers[j].RequestTimeStamp > loan_offers[j+1].RequestTimeStamp) {
                LoanOffers temp = loan_offers[j];
                loan_offers[j] = loan_offers[j+1];
                loan_offers[j+1] = temp;
            }
        }
    }
}
/*
 ********************************************************************************************
 * Name:        sort_loanoffer_interestrate
 * Parameters:  LoanOffers array, int n - dimension of the array
 * Returns      void
 * Purpose:     Sorts the loan offers array by the Interest Rate field
 *              Called by sort_loan_offers()
 ********************************************************************************************
 */
void sort_loanoffer_interestrate( LoanOffers* loan_offers, int n ) 
{
    int j,i;
    for(i=1;i<n;i++) {
        for(j=0;j<n-i;j++) {
            if(loan_offers[j].OfferRate > loan_offers[j+1].OfferRate) {
                LoanOffers temp = loan_offers[j];
                loan_offers[j] = loan_offers[j+1];
                loan_offers[j+1] = temp;
            }
        }
    }
}
/*
 ********************************************************************************************
 * Name:        load_loan_offers
 * Parameters:  void
 * Returns      int, success or failure
 * Purpose:     Loads the loan offers data into the LoanOffer struct
 *              Called by main()
 ********************************************************************************************
 */
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
    for (counter = 0; counter < 10; counter++) {
        requestloantimestamp = malloc(strlen(filenames1[counter]));
        loanoffertimestamp = malloc(strlen(filenames1[counter]));
        strncpy(requestloantimestamp, filenames1[counter], VALID_LOANREQUEST_TIMESTAMP_LENGTH);
        requestloantimestamp[VALID_LOANREQUEST_TIMESTAMP_LENGTH] = '\0';
        extract_loanoffer_timestamp(filenames1[counter], loanoffertimestamp);
        ifp = fopen(filenames[counter],"r");
        if (ifp == NULL) {
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
            lo[counter].RequestTimeStamp = atoi(requestloantimestamp);
            lo[counter].OfferTimeStamp = atoi(loanoffertimestamp);
            lo[counter].OfferAddress = malloc(strlen(token[0]) + 1); //from:https://stackoverflow.com/questions/3131319/how-to-correctly-assign-a-new-string-value
            strcpy(lo[counter].OfferAddress, token[0]);
            lo[counter].OfferRate = atof(token[1]);
            lo[counter].AcceptedFlag = atoi(token[2]);
        }
    }
    fclose(ifp); // close the file handler
    return(counter);    
}
/*
 ********************************************************************************************
 * Name:        extract_loanoffer_timestamp
 * Parameters:  filename of the loan offers file, array to store the extract timestamp
 * Returns      void
 * Purpose:     Example code provided by the facilitator, extracts the second timestamp from 
 *              the filename and stores it in the loanoffertimestamp array
 *              Called by load_loan_offers() 
 ********************************************************************************************
 */
void extract_loanoffer_timestamp( const char *filename, char *loanoffertimestamp )
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
	}

}
/*
 ********************************************************************************************
 * Name:        get_loan_offers_files
 * Parameters:  char pointer to the path containing the loan offers
 * Returns      void
 * Purpose:     Attempts to get a list of files from the loan offers path and save them to
 *              an array. I wasn't able to get this to work so went with hardcoding the 
 *              names of the files into an array
 ********************************************************************************************
 */
void get_loan_offers_files( char* path ) 
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
        ++num_files;
    }
    closedir(dr); 
}
/*
 ********************************************************************************************
 * Name:        view_loan_requests
 * Parameters:  int num_lines, number of records in the lr array
 * Returns      void
 * Purpose:     Outputs the contents of the loan requests array to the screen in an easy to 
 *              read format
 ********************************************************************************************
 */
void view_loan_requests( int num_lines ) 
{
    printf("\n\n");
    printf("+--------------------------------------------------------------------------------------------------------------+\n");
    printf("| %8s | %18s | %16s | %15s | %11s | %16s | %7s", "RecordID", "Request Timestamp", "Amount Requested", "Amount Pledged", "Loan Period", "Interest Rate", "Status |");
    printf("\n+--------------------------------------------------------------------------------------------------------------+\n");
    for (int i = 0; i < num_lines; i++) {
        printf("| %8i | %18i | %16.2lf | %15.2lf | %11i | %16.2lf | %6i |\n", i, lr[i].RequestTimeStamp, lr[i].AmountRequested, lr[i].AmountPledged, lr[i].LoanPeriod, lr[i].InterestRate, lr[i].Status);
    }
    printf("+--------------------------------------------------------------------------------------------------------------+\n\n\n");
    printf("Press any key to continue...\n");  
    getch(); //the getch() function pauses the output until a key is pressed. This is a Windows-only function from conio.h so limits the portability of the program
    //from https://stackoverflow.com/questions/29612201/implicit-declaration-of-function-getch
}
/*
 ********************************************************************************************
 * Name:        view_loan_offers
 * Parameters:  int num_lines, number of records in the lo array
 * Returns      void
 * Purpose:     Outputs the contents of the loan offers array to the screen in an easy to 
 *              read format
 ********************************************************************************************
 */
void view_loan_offers( int num_lines ) 
{
    printf("\n\n+---------------------------------------------------------------------------------------------------------------------------+\n");
    printf("| %8s | %18s | %16s | %40s | %11s | %14s\n","RecordID","Request Timestamp","Offer Timestamp","Offer Address","Offer Rate", "Accepted Flag |");
    printf("+---------------------------------------------------------------------------------------------------------------------------+\n");
    for (int i = 0; i < num_lines; i++) {
        printf("| %8i | %18i | %16i | %40s | %11.2f | %10i    |\n", i, lo[i].RequestTimeStamp, lo[i].OfferTimeStamp, lo[i].OfferAddress, lo[i].OfferRate, lo[i].AcceptedFlag);
    }
    printf("+---------------------------------------------------------------------------------------------------------------------------+\n\n\n");
    printf("Press any key to continue...\n");  
    getch(); //the getch() function pauses the output until a key is pressed. This is a Windows-only function from conio.h so limits the portability of the program
}
/*
 ********************************************************************************************
 * Name:        save_data
 * Parameters:  None
 * Returns      void
 * Purpose:     Saves the contents of the lo and lr arrays to the filesystem. I was able to
 *              save the data to the loan request array. I am stuck on how to write the data
 *              to the loan offers files
 ********************************************************************************************
 */
void save_data( int lo_num_lines, int lr_num_lines )
{
    FILE *fp = fopen(LOAN_REQUESTS_PATH, "w");
    char *filenames[10] = {"loanoffers\\1544021094_1544024694_Offer.txt", "loanoffers\\1544698794_1544702394_Offer.txt","loanoffers\\1544698794_1544716794_Offer.txt","loanoffers\\1544921094_1544939094_Offer.txt","loanoffers\\1544921094_1544946294_Offer.txt","loanoffers\\1545021094_1545028594_Offer.txt","loanoffers\\1545021094_1545035494_Offer.txt","loanoffers\\1545221094_1545224694_Offer.txt","loanoffers\\1545221094_1545229499_Offer.txt","loanoffers\\1545221094_1545239094_Offer.txt" };
    for (int i = 0; i < lr_num_lines; i++) {
        fprintf(fp, "%i %.0lf %.0lf %i %.2lf %i\n", lr[i].RequestTimeStamp, lr[i].AmountRequested, lr[i].AmountPledged, lr[i].LoanPeriod, lr[i].InterestRate, lr[i].Status);
    }
    for (int i = 0; i < 10; i++){
        char request_timestamp[50], offer_timestamp[50];
        sprintf(request_timestamp, "%d", lo[i].RequestTimeStamp);
        sprintf(offer_timestamp, "%d", lo[i].OfferTimeStamp);
        // char *file = malloc(strlen((char)lo[i].RequestTimeStamp) + strlen((char)lo[i].OfferTimeStamp) + 2);  //assign enough space for the '_' and '\0' characters
        // strcpy(file, (char)lo[i].RequestTimeStamp);
        // strcat(file, "_");
        // strcat(file, (char)lo[i].OfferTimeStamp);
        // printf("%s", file);        
    }
    fclose(fp);
}
/********************************************************************************************
 * Name:        exit_CLDA
 * Parameters:  None
 * Returns      void
 * Purpose:     This function is called whenever the user chooses to exit the system. 
 *              saves data and closes all open files before writing an exit message to the 
 *              screen and exiting the program.
 ********************************************************************************************
 */
void exit_CLDA() 
{
    exit(1);
}